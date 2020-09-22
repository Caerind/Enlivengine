#include <Enlivengine/System/MemoryAllocator.hpp>

#include <Enlivengine/System/Assert.hpp>

namespace en
{

dyma::Allocator* MemoryAllocator::sEngineAllocator = nullptr;

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
MemoryAllocator::DebugAllocator MemoryAllocator::sDebugAllocator;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
dyma::Mallocator MemoryAllocator::sDefaultMallocator;
bool MemoryAllocator::sDefaultMallocatorInitialized = MemoryAllocator::InitializeEngineAllocator(&sDefaultMallocator);
#endif // ENLIVE_ENABLE_DEFAULT_MALLOCATOR

bool MemoryAllocator::InitializeEngineAllocator(dyma::Allocator* allocator)
{
	if (allocator == nullptr)
	{
#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
		allocator = &sDefaultMallocator;
#else
		return false;
#endif // ENLIVE_ENABLE_DEFAULT_MALLOCATOR
	}

	sEngineAllocator = allocator;

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	sDebugAllocator.SetAllocatorToDebug(allocator);
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
	sDefaultMallocatorInitialized = (allocator == &sDefaultMallocator);
#endif // ENLIVE_ENABLE_DEFAULT_MALLOCATOR

	return true;
}

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
MemoryAllocator::DebugAllocator::DebugAllocator()
	: mAllocator(nullptr)
	, mContext(nullptr)
	, mFile(nullptr)
	, mLine(0)
	, mAllocationCount(0)
	, mDeallocationCount(0)
	, mUsedSize(0)
	, mPeakSize(0)
	, mBlocks()
{
}

void MemoryAllocator::DebugAllocator::SetCurrentContext(const char* context, const char* file, U32 line)
{
	mContext = context;
	mFile = file;
	mLine = line;
}

void MemoryAllocator::DebugAllocator::SetAllocatorToDebug(dyma::Allocator* allocator)
{
	enAssert(allocator != nullptr);

	mAllocator = allocator;
	mAllocationCount = 0;
	mDeallocationCount = 0;
	mUsedSize = 0;
	mPeakSize = 0;
	mBlocks.clear();
}

dyma::Allocator* MemoryAllocator::DebugAllocator::GetAllocatorToDebug()
{
	return mAllocator;
}

const dyma::Allocator* MemoryAllocator::DebugAllocator::GetAllocatorToDebug() const
{
	return mAllocator;
}

void* MemoryAllocator::DebugAllocator::Allocate(std::size_t size)
{
	enAssert(mAllocator != nullptr);

	void* ptr = mAllocator->Allocate(size);
	if (ptr != nullptr)
	{
		mAllocationCount++;

		DebugMemoryBlock block;
		block.ptr = ptr;
		block.size = static_cast<U32>(size);
		block.context = mContext;
		block.file = mFile;
		block.line = mLine;
		mBlocks.push_back(block);

		mUsedSize += static_cast<U32>(size);
		if (mUsedSize > mPeakSize)
		{
			mPeakSize = mUsedSize;
		}
	}
	return ptr;
}

bool MemoryAllocator::DebugAllocator::Deallocate(void*& ptr)
{
	enAssert(mAllocator != nullptr);

	const void* ptrBeforeDeallocation = ptr;
	const bool result = mAllocator->Deallocate(ptr);
	if (result)
	{
		mDeallocationCount++;

		U32 deallocationSize = 0;
		const std::size_t blockCount = mBlocks.size();
		for (std::size_t i = 0; i < blockCount; ++i)
		{
			if (ptrBeforeDeallocation == mBlocks[i].ptr)
			{
				deallocationSize = mBlocks[i].size;
				mBlocks.erase(mBlocks.begin() + i);
				break;
			}
		}

		mUsedSize -= deallocationSize;
	}
	return result;
}

bool MemoryAllocator::DebugAllocator::Owns(const void* ptr) const
{
	enAssert(mAllocator != nullptr);

	return mAllocator->Owns(ptr);
}
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

} // namespace en