#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
#include <vector>
#include <filesystem>
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#include <Dyma/Dyma.hpp>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Macros.hpp>
#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/TypeTraits.hpp>

namespace en
{
	
class MemoryAllocator
{
public:
	MemoryAllocator() = delete;

	static bool InitializeEngineAllocator(dyma::Allocator* allocator);

private:
	static dyma::Allocator* sEngineAllocator;

public:
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	template <typename T>
	static T* TypedAllocate(const char* context, const char* file, U32 line, U32 count = 1)
	{
		return (T*)new (Allocate(context, file, line, count * ENLIVE_SIZE_OF(T))) T();
	}

	template <typename T = void>
	static bool TypedDeallocate(T*& ptr)
	{
		if (ptr == nullptr)
		{
			return true;
		}
		void* voidPtr = (void*)ptr;
		if constexpr (!en::Traits::IsTriviallyDestructible<T>::value)
		{
			ptr->~T();
		}
		const bool result = Deallocate(voidPtr);
		ptr = (T*)voidPtr;
		return result;
	}

	static void* Allocate(const char* context, const char* file, U32 line, U32 size)
	{
		enAssert(sDebugAllocator.GetAllocatorToDebug() != nullptr);
		sDebugAllocator.SetCurrentContext(context, file, line);
		return sDebugAllocator.Allocate(static_cast<std::size_t>(size));
	}

	static bool Deallocate(void*& ptr)
	{
		if (ptr == nullptr)
		{
			return true;
		}
		enAssert(sDebugAllocator.GetAllocatorToDebug() != nullptr);
		return sDebugAllocator.Deallocate(ptr);
	}
#else
	template <typename T>
	static T* TypedAllocate(U32 count = 1)
	{
		return (T*)new (Allocate(count * ENLIVE_SIZE_OF(T))) T();
	}

	template <typename T = void>
	static bool TypedDeallocate(T*& ptr)
	{
		void* voidPtr = (void*)ptr;
		const bool result = Deallocate(voidPtr);
		ptr = (T*)voidPtr;
		return result;
	}

	static void* Allocate(U32 size)
	{
		enAssert(sEngineAllocator != nullptr);
		return sEngineAllocator->Allocate(static_cast<std::size_t>(size));
	}

	static bool Deallocate(void*& ptr)
	{
		if (ptr == nullptr)
		{
			return true;
		}
		enAssert(sEngineAllocator != nullptr);
		return sEngineAllocator->Deallocate(ptr);
	}
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
public:
	class DebugAllocator : public dyma::Allocator
	{
	public:
		struct DebugMemoryBlock
		{
			void* ptr;
			U32 size;
			const char* context;
			const char* file;
			U32 line;

			std::string GetFile() const
			{
				return std::filesystem::path(file).filename().string();
			}
		};

		DebugAllocator();

		void SetCurrentContext(const char* context, const char* file, U32 line);
		void SetAllocatorToDebug(dyma::Allocator* allocator);
		dyma::Allocator* GetAllocatorToDebug();
		const dyma::Allocator* GetAllocatorToDebug() const;

		void* Allocate(std::size_t size) override final;
		bool Deallocate(void*& ptr) override final;
		bool Owns(const void* ptr) const override final;

		U32 GetAllocationCount() const { return mAllocationCount; }
		U32 GetDeallocationCount() const { return mDeallocationCount; }
		U32 GetUsedSize() const { return mUsedSize; }
		U32 GetPeakSize() const { return mPeakSize; }
		const std::vector<DebugMemoryBlock>& GetBlocks() const { return mBlocks; }
		U32 GetBlockCount() const { return static_cast<U32>(mBlocks.size()); }

	private:
		dyma::Allocator* mAllocator;
		const char* mContext;
		const char* mFile;
		U32 mLine;

		U32 mAllocationCount;
		U32 mDeallocationCount;

		U32 mUsedSize;
		U32 mPeakSize;

		std::vector<DebugMemoryBlock> mBlocks;
	};

	static const DebugAllocator& GetDebugAllocator() { return sDebugAllocator; }

private: 
	static DebugAllocator sDebugAllocator;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
private:
	static dyma::Mallocator sDefaultMallocator;
	static bool sDefaultMallocatorInitialized; // Trick to auto-register Mallocator
#endif // ENLIVE_ENABLE_DEFAULT_MALLOCATOR
};

} // namespace en

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
#define enNewCount(type, context, count) en::MemoryAllocator::TypedAllocate<type>(context, __FILE__, __LINE__, count)
#else
#define enNewCount(type, context, count) en::MemoryAllocator::TypedAllocate<type>(count)
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#define enNew(type, context) enNewCount(type, context, 1)
#define enDelete(type, ptr) en::MemoryAllocator::TypedDeallocate<type>(ptr)
