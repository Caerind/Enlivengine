#pragma once

#include <algorithm> // sort
#include <cstdlib> // memcmp, memcpy

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/Macros.hpp>
#include <Enlivengine/System/MemoryAllocator.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/TypeInfo.hpp>

namespace en
{

template <typename T>
class Array
{
public:
	using Iterator = T*;
	using ConstIterator = const T*;

	constexpr Array() 
		: mArray(nullptr)
		, mSize(0)
		, mCapacity(0)
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
		, mDebugMemoryContext(TypeInfo<Array<T>>::GetName())
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
	{
	}

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	constexpr Array(const char* debugMemoryContext) 
		: mArray(nullptr)
		, mSize(0)
		, mCapacity(0)
		, mDebugMemoryContext(debugMemoryContext)
	{
	}
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

	constexpr Array(const Array& other) 
		: mArray(nullptr)
		, mSize(0)
		, mCapacity(0)
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
		, mDebugMemoryContext(TypeInfo<Array<T>>::GetName())
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
	{ 
		Copy(other); 
	}
	constexpr Array(Array&& other) 
		: mArray(other.mArray)
		, mSize(other.mSize)
		, mCapacity(other.mCapacity)
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
		, mDebugMemoryContext(other.mDebugMemoryContext)
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
	{  
		other.mArray = nullptr;
		other.mSize = 0;
		other.mCapacity = 0;
	}
	~Array() 
	{
		Free();
	}

	constexpr Array& operator=(const Array& other) { Copy(other); return *this; }
	constexpr Array& operator=(Array&& other) { Move(other); return *this; }

	constexpr bool operator==(const Array& other) const { return Equals(other); }
	constexpr bool operator!=(const Array& other) const { return !Equals(other); }
	constexpr bool Equals(const Array& other) const 
	{ 
		if (mSize != other.mSize)
		{
			return false;
		}
		else
		{
			for (U32 i = 0; i < mSize; ++i)
			{
				if (mArray[i] != other.mArray[i])
				{
					return false;
				}
			}
			return true;
		}
	}

	Iterator Begin() { return (Iterator)mArray; }
	ConstIterator Begin() const { return (ConstIterator)mArray; }
	Iterator End() { return (Iterator)((U8*)mArray + static_cast<U64>(mSize) * ENLIVE_SIZE_OF(T)); }
	ConstIterator End() const { return (ConstIterator)((U8*)mArray + static_cast<U64>(mSize) * ENLIVE_SIZE_OF(T)); }

	Iterator begin() { return Begin(); }
	ConstIterator begin() const { return Begin(); }
	Iterator end() { return End(); }
	ConstIterator end() const { return End(); }

	constexpr void Copy(const Array& other)
	{
		enAssert(this != &other);
		Clear();
		if (other.mSize > 0)
		{
			if (mCapacity < other.mSize)
			{
				Reserve(other.mSize);
			}
			for (U32 i = 0; i < other.mSize; ++i)
			{
				Add(other.mArray[i]);
			}
			mSize = other.mSize;
		}
	}

	constexpr void Move(Array&& other)
	{
		mArray = other.mArray;
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		other.mArray = nullptr;
		other.mSize = 0;
		other.mCapacity = 0;
	}

	void Resize(U32 newSize)
	{
		if (mCapacity < newSize)
		{
			Reserve(newSize);
		}
		if constexpr (Traits::IsPOD<T>::value)
		{
			mSize = newSize;
		}
		else
		{
			if (newSize > mSize)
			{
				Iterator itr = Begin() + mSize;
				const Iterator end = Begin() + newSize;
				for (; itr != end; ++itr)
				{
					new(itr) T;
				}
				mSize = newSize;
			}
			else if (newSize < mSize)
			{
				if constexpr (!Traits::IsTriviallyDestructible<T>::value)
				{
					Iterator itr = Begin() + mSize - newSize;
					const Iterator end = End();
					for (; itr != end; ++itr)
					{
						itr->~T();
					}
				}
				mSize = newSize;
			}
		}
	}

	void Reserve(U32 newCapacity)
	{
		enAssert(newCapacity > mCapacity);
		if (newCapacity > mCapacity)
		{
			Realloc(newCapacity);
		}
	}

	void Shrink(U32 newCapacity)
	{
		enAssert(newCapacity >= mSize);
		enAssert(newCapacity < mCapacity);
		if (newCapacity < mCapacity)
		{
			Realloc(newCapacity);
		}
	}

	void Free()
	{
		Clear();
		Realloc(0);
	}

	constexpr void Clear()
	{
		if constexpr (!Traits::IsTriviallyDestructible<T>::value)
		{
			Iterator itr = Begin();
			const Iterator end = End();
			for (; itr != end; ++itr)
			{
				itr->~T();
			}
		}
		mSize = 0;
	}

	constexpr void Sort() { std::sort(begin(), end()); }
	template <typename Predicate> constexpr void Sort(Predicate predicate) { std::sort(begin(), end(), predicate); }

	constexpr T& operator[](U32 index) { enAssert(index < mSize); return mArray[index]; }
	constexpr const T& operator[](U32 index) const { enAssert(index < mSize); return mArray[index]; }

	constexpr U32 Size() const { return mSize; }
	constexpr U32 GetSize() const { return mSize; }
	constexpr bool Empty() const { return mSize == 0; }

	constexpr U32 GetElementSize() const { return ENLIVE_SIZE_OF(T); }
	constexpr U32 GetElementAlignment() const { return ENLIVE_ALIGN_OF(T); }

	constexpr U32 Capacity() const { return mCapacity; }
	constexpr U32 GetCapacity() const { return mCapacity; }
	static constexpr U32 GetNextCapacity(U32 currentCapacity)
	{ 
		if (currentCapacity < 4)
		{
			return 4;
		}
		else
		{
			currentCapacity |= currentCapacity >> 1;
			currentCapacity |= currentCapacity >> 2;
			currentCapacity |= currentCapacity >> 4;
			currentCapacity |= currentCapacity >> 8;
			currentCapacity |= currentCapacity >> 16;
			currentCapacity++;
			return currentCapacity;
		}
	}

	constexpr U8* GetArrayData() { return (U8*)mArray; }
	constexpr const U8* GetArrayData() const { return (U8*)mArray; }
	constexpr U32 GetArrayDataSize() const { return mSize * ENLIVE_SIZE_OF(T); }

	constexpr T& Front() { enAssert(mSize > 0); return mArray[0]; }
	constexpr const T& Front() const { enAssert(mSize > 0); return mArray[0]; }
	constexpr T& Back() { enAssert(mSize > 0); return mArray[mSize - 1]; }
	constexpr const T& Back() const { enAssert(mSize > 0); return mArray[mSize - 1]; }

	constexpr Iterator Find(const T& value)
	{ 
		if (mSize == 0) 
		{ 
			return nullptr; 
		}
		Iterator itr = Begin();
		const Iterator end = End();
		for (; itr != end; ++itr)
		{
			if (*itr == value)
			{
				return itr;
			}
		}
		return nullptr;
	}
	constexpr ConstIterator Find(const T& value) const
	{
		return const_cast<ConstIterator>(const_cast<Array>(*this).Find(value));
	}
	constexpr U32 IndexFromIterator(ConstIterator itr) const { return (U32)(std::size_t)(itr - Begin()); }
	constexpr U32 IndexOf(const T& value) const { return IndexFromIterator(&value); }
	constexpr U32 FindIndex(const T& value) const 
	{ 
		if (auto itr = Find(value))
		{
			return IndexFromIterator(itr);
		}
		return U32_Max;
	}

	void Add(const T& value)
	{
		if (mSize + 1 > mCapacity)
		{
			Reserve(GetNextCapacity(mSize + 1));
		}
		mSize++;
		Iterator itr = Begin() + mSize - 1;
		if constexpr (!Traits::IsTriviallyDefaultConstructible<T>::value || !Traits::IsTriviallyCopyConstructible<T>::value)
		{
			new(itr) T(value);
		}
		else
		{
			*itr = value;
		}
	}

	bool AddUnique(const T& value)
	{
		if (Find(value) == nullptr)
		{
			Add(value);
			return true;
		}
		return false;
	}

	bool Remove(const T& value)
	{
		for (U32 i = 0; i < mSize; ++i)
		{
			if (mArray[i] == value)
			{
				RemoveAtIndex(i);
				return true;
			}
		}
		return false;
	}

	bool RemoveAll(const T& value)
	{
		bool result = false;
		for (U32 i = 0; i < mSize; )
		{
			if (mArray[i] == value)
			{
				RemoveAtIndex(i);
				result = true;
			}
			else
			{
				++i;
			}
		}
		return result;
	}

	void RemoveAtIndex(U32 index)
	{
		enAssert(index < mSize);
		if constexpr (!Traits::IsTriviallyDestructible<T>::value)
		{
			mArray[index].~T();
		}
		if (index + 1 < mSize)
		{
			mArray[index] = mArray[mSize - 1];
		}
		mSize--;
	}

	void DeleteAll()
	{
		if constexpr (Traits::IsPointer<T>::value)
		{
			for (U32 i = 0; i < mSize; ++i)
			{
				enDelete(Traits::RemovePointer<T>::type, mArray[i]);
			}
		}
	}

	//constexpr bool Exists(const T& value) const { return false; }
	//template <typename Predicate>
	//constexpr bool Any(Predicate p) const { return false; }

	//constexpr T& Pop(T& element) {}
	//constexpr T Pop() {}

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	const char* GetDebugMemoryContext() const { return mDebugMemoryContext; }
	void SetDebugMemoryContext(const char* context) { mDebugMemoryContext = context; }
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

private:
	// To ensure the user knows what he is doing, Realloc is private
	// Use Reserve/Shrink/ShrinkToFit instead
	void Realloc(U32 newCapacity)
	{
		mCapacity = newCapacity;
		if (mArray != nullptr)
		{
			const bool result = enDelete(T, mArray);
			enAssert(result && mArray == nullptr);
		}
		if (mCapacity > 0)
		{
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
			mArray = enNewCount(T, mDebugMemoryContext, mCapacity);
#else
			mArray = enNewCount(T, "Array", mCapacity);
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
			enAssert(mArray != nullptr);
		}
	}

private:
	T* mArray;
	U32 mSize;
	U32 mCapacity;

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	const char* mDebugMemoryContext;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Array)
