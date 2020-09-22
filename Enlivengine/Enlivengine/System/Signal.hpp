// Copyright (C) 2017 Jérôme Leclercq, author of Nazara Engine : https://github.com/DigitalPulseSoftware/NazaraEngine

#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <Enlivengine/System/Assert.hpp>

#define enDetailSignal(Keyword, SignalName, ...) using SignalName ## Type = ::en::Signal<__VA_ARGS__>; Keyword SignalName ## Type SignalName

#define enSignal(SignalName, ...) enDetailSignal(mutable, SignalName, __VA_ARGS__)
#define enStaticSignal(SignalName, ...) enDetailSignal(static, SignalName, __VA_ARGS__)
#define enStaticSignalImpl(Class, SignalName) Class :: SignalName ## Type Class :: SignalName

#define enSlotType(Class, SignalName) Class::SignalName ## Type::ConnectionGuard
#define enSlot(Class, SignalName, SlotName) Class::SignalName ## Type::ConnectionGuard SlotName

namespace en
{

template <typename... Args>
class Signal
{
	public:
		using Callback = std::function<void(Args...)>;
		class Connection;
		class ConnectionGuard;

		Signal();
		Signal(const Signal&);
		Signal(Signal&& signal) noexcept;
		~Signal() = default;

		void Clear();

		Connection Connect(const Callback& func);
		Connection Connect(Callback&& func);
		template <typename O> Connection Connect(O& object, void (O::*method)(Args...));
		template <typename O> Connection Connect(O* object, void (O::*method)(Args...));
		template <typename O> Connection Connect(const O& object, void (O::*method)(Args...) const);
		template <typename O> Connection Connect(const O* object, void (O::*method)(Args...) const);

		void operator()(Args... args) const;

		Signal& operator=(const Signal&);
		Signal& operator=(Signal&& signal) noexcept;

	private:
		struct Slot;

		using SlotPtr = std::shared_ptr<Slot>;
		using SlotList = std::vector<SlotPtr>;
		using SlotListIndex = typename SlotList::size_type;

		struct Slot
		{
			Slot(Signal* me) 
				: signal(me)
			{
			}

			Callback callback;
			Signal* signal;
			SlotListIndex index;
		};

		void Disconnect(const SlotPtr& slot) noexcept;

		SlotList mSlots;
		mutable SlotListIndex mSlotIterator;
};

template <typename... Args>
class Signal<Args...>::Connection
{
	using BaseClass = Signal<Args...>;
	friend BaseClass;

	public:
		Connection() = default;
		Connection(const Connection& connection) = default;
		Connection(Connection&& connection) noexcept;
		~Connection() = default;

		template <typename... ConnectArgs>
		void Connect(BaseClass& signal, ConnectArgs&&... args);
		void Disconnect() noexcept;

		bool IsConnected() const;

		Connection& operator=(const Connection& connection) = default;
		Connection& operator=(Connection&& connection) noexcept;

	private:
		Connection(const SlotPtr& slot);

		std::weak_ptr<Slot> mPtr;
};

template <typename... Args>
class Signal<Args...>::ConnectionGuard
{
	using BaseClass = Signal<Args...>;
	using Connection = typename BaseClass::Connection;

	public:
		ConnectionGuard() = default;
		ConnectionGuard(const Connection& connection);
		ConnectionGuard(const ConnectionGuard& connection) = delete;
		ConnectionGuard(Connection&& connection);
		ConnectionGuard(ConnectionGuard&& connection) noexcept = default;
		~ConnectionGuard();

		template <typename... ConnectArgs>
		void Connect(BaseClass& signal, ConnectArgs&&... args);
		void Disconnect() noexcept;

		Connection& GetConnection();

		bool IsConnected() const;

		ConnectionGuard& operator=(const Connection& connection);
		ConnectionGuard& operator=(const ConnectionGuard& connection) = delete;
		ConnectionGuard& operator=(Connection&& connection);
		ConnectionGuard& operator=(ConnectionGuard&& connection) noexcept;

	private:
		Connection mConnection;
};

template <typename... Args>
Signal<Args...>::Signal()
	: mSlotIterator(0)
{
}

template <typename... Args>
Signal<Args...>::Signal(const Signal&) 
	: Signal()
{
}

template <typename... Args>
Signal<Args...>::Signal(Signal&& signal) noexcept
{
	operator=(std::move(signal));
}

template <typename... Args>
void Signal<Args...>::Clear()
{
	mSlots.clear();
	mSlotIterator = 0;
}

template <typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::Connect(const Callback& func)
{
	return Connect(Callback(func));
}

template <typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::Connect(Callback&& func)
{
	enAssert(func);

	// Since we're incrementing the slot vector size, we need to replace our iterator at the end
	// (Except when we are iterating on the signal)
	bool resetIt = (mSlotIterator >= mSlots.size());

	auto tempPtr = std::make_shared<Slot>(this);
	tempPtr->callback = std::move(func);
	tempPtr->index = mSlots.size();

	mSlots.emplace_back(std::move(tempPtr));

	if (resetIt)
		mSlotIterator = mSlots.size(); //< Replace the iterator to the end

	return Connection(mSlots.back());
}

template <typename... Args>
template <typename O>
typename Signal<Args...>::Connection Signal<Args...>::Connect(O& object, void (O::*method) (Args...))
{
	return Connect([&object, method](Args&&... args)
	{
		return (object.*method) (std::forward<Args>(args)...);
	});
}

template <typename... Args>
template <typename O>
typename Signal<Args...>::Connection Signal<Args...>::Connect(O* object, void (O::*method)(Args...))
{
	return Connect([object, method](Args&&... args)
	{
		return (object->*method) (std::forward<Args>(args)...);
	});
}

template <typename... Args>
template <typename O>
typename Signal<Args...>::Connection Signal<Args...>::Connect(const O& object, void (O::*method) (Args...) const)
{
	return Connect([&object, method](Args&&... args)
	{
		return (object.*method) (std::forward<Args>(args)...);
	});
}

template <typename... Args>
template <typename O>
typename Signal<Args...>::Connection Signal<Args...>::Connect(const O* object, void (O::*method)(Args...) const)
{
	return Connect([object, method](Args&&... args)
	{
		return (object->*method) (std::forward<Args>(args)...);
	});
}

template <typename... Args>
void Signal<Args...>::operator()(Args... args) const
{
	for (mSlotIterator = 0; mSlotIterator < mSlots.size(); ++mSlotIterator)
	{
		mSlots[mSlotIterator]->callback(args...);
	}
}

template <typename... Args>
Signal<Args...>& Signal<Args...>::operator=(const Signal&)
{
	return *this;
}

template <typename... Args>
Signal<Args...>& Signal<Args...>::operator=(Signal&& signal) noexcept
{
	mSlots = std::move(signal.mSlots);
	mSlotIterator = signal.mSlotIterator;

	// We need to update the signal pointer inside of each slot
	for (SlotPtr& slot : mSlots)
		slot->signal = this;

	return *this;
}

template <typename... Args>
void Signal<Args...>::Disconnect(const SlotPtr& slot) noexcept
{
	enAssert(slot != nullptr);
	enAssert(slot->index < mSlots.size());
	enAssert(slot->signal == this);

	// "Swap this slot with the last one and pop" idiom
	// This will preserve slot indexes

	// Can we safely "remove" this slot?
	if (mSlotIterator >= (mSlots.size() - 1) || slot->index > mSlotIterator)
	{
		// Yes we can
		SlotPtr& newSlot = mSlots[slot->index];
		newSlot = std::move(mSlots.back());
		newSlot->index = slot->index; //< Update the moved slot index before resizing (in case it's the last one)
	}
	else
	{
		// Nope, let's be tricky
		SlotPtr& current = mSlots[mSlotIterator];
		SlotPtr& newSlot = mSlots[slot->index];

		newSlot = std::move(current);
		newSlot->index = slot->index; //< Update the moved slot index

		current = std::move(mSlots.back());
		current->index = mSlotIterator; //< Update the moved slot index

		--mSlotIterator;
	}

	// Pop the last entry (from where we moved our slot)
	mSlots.pop_back();
}

template <typename... Args>
Signal<Args...>::Connection::Connection(Connection&& connection) noexcept
	: mPtr(std::move(connection.mPtr))
{
	connection.mPtr.reset(); //< Fuck you GCC 4.9
}

template <typename... Args>
Signal<Args...>::Connection::Connection(const SlotPtr& slot) 
	: mPtr(slot)
{
}

template <typename... Args>
template <typename... ConnectArgs>
void Signal<Args...>::Connection::Connect(BaseClass& signal, ConnectArgs&&... args)
{
	operator=(signal.Connect(std::forward<ConnectArgs>(args)...));
}

template <typename... Args>
void Signal<Args...>::Connection::Disconnect() noexcept
{
	if (SlotPtr ptr = mPtr.lock())
		ptr->signal->Disconnect(ptr);
}

template <typename... Args>
bool Signal<Args...>::Connection::IsConnected() const
{
	return !mPtr.expired();
}

template <typename... Args>
typename Signal<Args...>::Connection& Signal<Args...>::Connection::operator=(Connection&& connection) noexcept
{
	mPtr = std::move(connection.mPtr);
	connection.mPtr.reset(); //< Fuck you GCC 4.9

	return *this;
}

template <typename... Args>
Signal<Args...>::ConnectionGuard::ConnectionGuard(const Connection& connection)
	: mConnection(connection)
{
}

template <typename... Args>
Signal<Args...>::ConnectionGuard::ConnectionGuard(Connection&& connection)
	: mConnection(std::move(connection))
{
}

template <typename... Args>
Signal<Args...>::ConnectionGuard::~ConnectionGuard()
{
	mConnection.Disconnect();
}

template <typename... Args>
template <typename... ConnectArgs>
void Signal<Args...>::ConnectionGuard::Connect(BaseClass& signal, ConnectArgs&&... args)
{
	mConnection.Disconnect();
	mConnection.Connect(signal, std::forward<ConnectArgs>(args)...);
}

template <typename... Args>
void Signal<Args...>::ConnectionGuard::Disconnect() noexcept
{
	mConnection.Disconnect();
}

template <typename... Args>
typename Signal<Args...>::Connection& Signal<Args...>::ConnectionGuard::GetConnection()
{
	return mConnection;
}

template <typename... Args>
bool Signal<Args...>::ConnectionGuard::IsConnected() const
{
	return mConnection.IsConnected();
}

template <typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(const Connection& connection)
{
	mConnection.Disconnect();
	mConnection = connection;

	return *this;
}

template<typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(Connection&& connection)
{
	if (&connection != this)
	{
		mConnection.Disconnect();
		mConnection = std::move(connection);
	}

	return *this;
}

template <typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(ConnectionGuard&& connection) noexcept
{
	if (&connection != this)
	{
		mConnection.Disconnect();
		mConnection = std::move(connection.mConnection);
	}

	return *this;
}

} // namespace en
