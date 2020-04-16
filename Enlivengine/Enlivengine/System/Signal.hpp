// Copyright (C) 2017 Jérôme Leclercq, author of Nazara Engine : https://github.com/DigitalPulseSoftware/NazaraEngine

#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <Enlivengine/System/Assert.hpp>

#define EnDetailSignal(Keyword, SignalName, ...) using SignalName ## Type = ::en::Signal<__VA_ARGS__>; Keyword SignalName ## Type SignalName

#define EnSignal(SignalName, ...) EnDetailSignal(mutable, SignalName, __VA_ARGS__)
#define EnStaticSignal(SignalName, ...) EnDetailSignal(static, SignalName, __VA_ARGS__)
#define EnStaticSignalImpl(Class, SignalName) Class :: SignalName ## Type Class :: SignalName

#define EnSlot(Class, SignalName, SlotName) Class::SignalName ## Type::ConnectionGuard SlotName

namespace en
{

template<typename... Args>
class Signal
{
	public:
		using Callback = std::function<void(Args...)>;
		class Connection;
		class ConnectionGuard;

		Signal();
		Signal(const Signal&) = delete;
		Signal(Signal&& signal) noexcept;
		~Signal() = default;

		void clear();

		Connection connect(const Callback& func);
		Connection connect(Callback&& func);
		template<typename O> Connection connect(O& object, void (O::*method)(Args...));
		template<typename O> Connection connect(O* object, void (O::*method)(Args...));
		template<typename O> Connection connect(const O& object, void (O::*method)(Args...) const);
		template<typename O> Connection connect(const O* object, void (O::*method)(Args...) const);

		void operator()(Args... args) const;

		Signal& operator=(const Signal&) = delete;
		Signal& operator=(Signal&& signal) noexcept;

	private:
		struct Slot;

		using SlotPtr = std::shared_ptr<Slot>;
		using SlotList = std::vector<SlotPtr>;
		using SlotListIndex = typename SlotList::size_type;

		struct Slot
		{
			Slot(Signal* me) : signal(me)
			{
			}

			Callback callback;
			Signal* signal;
			SlotListIndex index;
		};

		void disconnect(const SlotPtr& slot);

		SlotList mSlots;
		mutable SlotListIndex mSlotIterator;
};

template<typename... Args>
class Signal<Args...>::Connection
{
	using BaseClass = Signal<Args...>;
	friend BaseClass;

	public:
		Connection() = default;
		Connection(const Connection& connection) = default;
		Connection(Connection&& connection) = default;
		~Connection() = default;

		template<typename... ConnectArgs>
		void connect(BaseClass& signal, ConnectArgs&&... args);
		void disconnect();

		bool isConnected() const;

		Connection& operator=(const Connection& connection) = default;
		Connection& operator=(Connection&& connection) = default;

	private:
		Connection(const SlotPtr& slot);

		std::weak_ptr<Slot> mPtr;
};

template<typename... Args>
class Signal<Args...>::ConnectionGuard
{
	using BaseClass = Signal<Args...>;
	using Connection = typename BaseClass::Connection;

	public:
		ConnectionGuard() = default;
		ConnectionGuard(const Connection& connection);
		ConnectionGuard(const ConnectionGuard& connection) = delete;
		ConnectionGuard(Connection&& connection);
		ConnectionGuard(ConnectionGuard&& connection) = default;
		~ConnectionGuard();

		template<typename... ConnectArgs>
		void connect(BaseClass& signal, ConnectArgs&&... args);
		void disconnect();

		Connection& getConnection();

		bool isConnected() const;

		ConnectionGuard& operator=(const Connection& connection);
		ConnectionGuard& operator=(const ConnectionGuard& connection) = delete;
		ConnectionGuard& operator=(Connection&& connection);
		ConnectionGuard& operator=(ConnectionGuard&& connection);

	private:
		Connection mConnection;
};

template<typename... Args>
Signal<Args...>::Signal()
	: mSlotIterator(0)
{
}

template<typename... Args>
Signal<Args...>::Signal(Signal&& signal) noexcept
{
	operator=(std::move(signal));
}

template<typename... Args>
void Signal<Args...>::clear()
{
	mSlots.clear();
	mSlotIterator = 0;
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect(const Callback& func)
{
	return connect(Callback(func));
}

template<typename... Args>
typename Signal<Args...>::Connection Signal<Args...>::connect(Callback&& func)
{
	assert(func);

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

template<typename... Args>
template<typename O>
typename Signal<Args...>::Connection Signal<Args...>::connect(O& object, void (O::*method) (Args...))
{
	return connect([&object, method](Args&&... args)
	{
		return (object.*method) (std::forward<Args>(args)...);
	});
}

template<typename... Args>
template<typename O>
typename Signal<Args...>::Connection Signal<Args...>::connect(O* object, void (O::*method)(Args...))
{
	return connect([object, method](Args&&... args)
	{
		return (object->*method) (std::forward<Args>(args)...);
	});
}

template<typename... Args>
template<typename O>
typename Signal<Args...>::Connection Signal<Args...>::connect(const O& object, void (O::*method) (Args...) const)
{
	return connect([&object, method](Args&&... args)
	{
		return (object.*method) (std::forward<Args>(args)...);
	});
}

template<typename... Args>
template<typename O>
typename Signal<Args...>::Connection Signal<Args...>::connect(const O* object, void (O::*method)(Args...) const)
{
	return connect([object, method](Args&&... args)
	{
		return (object->*method) (std::forward<Args>(args)...);
	});
}

template<typename... Args>
void Signal<Args...>::operator()(Args... args) const
{
	for (mSlotIterator = 0; mSlotIterator < mSlots.size(); ++mSlotIterator)
	{
		mSlots[mSlotIterator]->callback(args...);
	}
}

template<typename... Args>
Signal<Args...>& Signal<Args...>::operator=(Signal&& signal) noexcept
{
	mSlots = std::move(signal.mSlots);
	mSlotIterator = signal.mSlotIterator;

	// We need to update the signal pointer inside of each slot
	for (SlotPtr& slot : mSlots)
		slot->signal = this;

	return *this;
}

template<typename... Args>
void Signal<Args...>::disconnect(const SlotPtr& slot)
{
	assert(slot != nullptr);
	assert(slot->index < mSlots.size());
	assert(slot->signal == this);

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

template<typename... Args>
Signal<Args...>::Connection::Connection(const SlotPtr& slot)
	: mPtr(slot)
{
}

template<typename... Args>
template<typename... ConnectArgs>
void Signal<Args...>::Connection::connect(BaseClass& signal, ConnectArgs&&... args)
{
	operator=(signal.connect(std::forward<ConnectArgs>(args)...));
}

template<typename... Args>
void Signal<Args...>::Connection::disconnect()
{
	if (SlotPtr ptr = mPtr.lock())
		ptr->signal->disconnect(ptr);
}

template<typename... Args>
bool Signal<Args...>::Connection::isConnected() const
{
	return !mPtr.expired();
}

template<typename... Args>
Signal<Args...>::ConnectionGuard::ConnectionGuard(const Connection& connection)
	: mConnection(connection)
{
}

template<typename... Args>
Signal<Args...>::ConnectionGuard::ConnectionGuard(Connection&& connection)
	: mConnection(std::move(connection))
{
}

template<typename... Args>
Signal<Args...>::ConnectionGuard::~ConnectionGuard()
{
	mConnection.disconnect();
}

template<typename... Args>
template<typename... ConnectArgs>
void Signal<Args...>::ConnectionGuard::connect(BaseClass& signal, ConnectArgs&&... args)
{
	mConnection.disconnect();
	mConnection.connect(signal, std::forward<ConnectArgs>(args)...);
}

template<typename... Args>
void Signal<Args...>::ConnectionGuard::disconnect()
{
	mConnection.disconnect();
}

template<typename... Args>
typename Signal<Args...>::Connection& Signal<Args...>::ConnectionGuard::getConnection()
{
	return mConnection;
}

template<typename... Args>
bool Signal<Args...>::ConnectionGuard::isConnected() const
{
	return mConnection.isConnected();
}

template<typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(const Connection& connection)
{
	mConnection.disconnect();
	mConnection = connection;

	return *this;
}

template<typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(Connection&& connection)
{
	mConnection.disconnect();
	mConnection = std::move(connection);

	return *this;
}

template<typename... Args>
typename Signal<Args...>::ConnectionGuard& Signal<Args...>::ConnectionGuard::operator=(ConnectionGuard&& connection)
{
	mConnection.disconnect();
	mConnection = std::move(connection.mConnection);

	return *this;
}

} // namespace en
