#include "Dyma.hpp"

#include <cstdlib> // malloc/calloc/realloc/free
#include <cassert> // assert

namespace dyma
{

void* Malloc(std::size_t size)
{
	return (size > 0) ? std::malloc(size) : nullptr;
}

void* Calloc(std::size_t num, std::size_t size)
{
	return (num > 0 && size > 0) ? std::calloc(num, size) : nullptr;
}

void* Realloc(void* ptr, std::size_t newSize)
{
	return (newSize > 0) ? std::realloc(ptr, newSize) : nullptr;
}

void Free(void* ptr)
{
	if (ptr != nullptr)
	{
		std::free(ptr);
	}
}

void* AlignedMalloc(std::size_t size, std::size_t alignment)
{
	assert(alignment >= 1);
	assert(alignment <= 128);
	assert((alignment & (alignment - 1)) == 0);
	const std::size_t requestSize = size + alignment;
	const void* rawMemory = Malloc(requestSize);
	if (rawMemory == nullptr)
	{
		return nullptr;
	}
	const std::uintptr_t rawAddress = reinterpret_cast<std::uintptr_t>(rawMemory);
	const std::uintptr_t misalignment = (rawAddress & (alignment - 1));
	const std::ptrdiff_t adjustment = alignment - misalignment;
	const std::uintptr_t alignedAddress = rawAddress + adjustment;
	assert(adjustment < 256);
	std::uint8_t* pAlignedMemory = reinterpret_cast<std::uint8_t*>(alignedAddress);
	pAlignedMemory[-1] = static_cast<std::uint8_t>(adjustment);
	return static_cast<void*>(pAlignedMemory);
}

void AlignedFree(void* ptr)
{
	if (ptr != nullptr)
	{
		const std::uint8_t* pAlignedMemory = reinterpret_cast<std::uint8_t*>(ptr);
		const std::uintptr_t alignedAddress = reinterpret_cast<std::uintptr_t>(ptr);
		const std::ptrdiff_t adjustment = static_cast<std::ptrdiff_t>(pAlignedMemory[-1]);
		const std::uintptr_t rawAddress = alignedAddress - adjustment;
		Free(reinterpret_cast<void*>(rawAddress));
	}
}

std::size_t RoundToAlignment(std::size_t size, std::size_t alignment)
{
	return (size + (alignment - 1)) & -alignment;
}

const void* MemorySource::GetEndPointer() const
{
	return reinterpret_cast<const void*>(reinterpret_cast<std::uintptr_t>(GetPointer()) + GetSize());
}

bool MemorySource::Owns(const void* ptr) const
{
	return GetPointer() <= ptr && ptr < GetEndPointer();
}

bool MemorySource::OwnsMemory() const
{
	return true;
}

const void* NullMemory::GetPointer() const 
{
	return nullptr;
}

std::size_t NullMemory::GetSize() const
{
	return 0;
}

std::size_t NullMemory::GetAlignment() const
{
	return 0;
}

bool NullMemory::OwnsMemory() const
{
	return false;
}

HeapMemory::HeapMemory(std::size_t bytes, std::size_t alignment /*= 0*/)
	: mMemory((alignment == 0) ? Malloc(bytes) : AlignedMalloc(bytes, alignment))
	, mSize((mMemory != nullptr) ? bytes : 0)
	, mAlignment((alignment == 0) ? 16 : alignment)
	, mAlignedByUser(alignment != 0)
{
}

HeapMemory::~HeapMemory()
{
	if (!mAlignedByUser)
	{
		Free(mMemory);
	}
	else
	{
		AlignedFree(mMemory);
	}
}

const void* HeapMemory::GetPointer() const
{
	return mMemory;
}

std::size_t HeapMemory::GetSize() const
{
	return mSize;
}

std::size_t HeapMemory::GetAlignment() const
{
	return mAlignment;
}

MemoryView::MemoryView(const void* begin, std::size_t bytes, std::size_t alignment /*= alignof(std::max_align_t)*/)
	: mBegin(begin)
	, mSize(bytes)
	, mAlignment(alignment)
{
}

const void* MemoryView::GetPointer() const
{
	return mBegin;
}

std::size_t MemoryView::GetSize() const
{
	return mSize;
}

std::size_t MemoryView::GetAlignment() const
{
	return mAlignment;
}

bool MemoryView::OwnsMemory() const
{
	return false;
}

void* NullAllocator::Allocate(std::size_t size)
{
	return nullptr;
}

bool NullAllocator::Deallocate(void*& ptr)
{
	return false;
}

bool NullAllocator::Owns(const void* ptr) const
{
	return false;
}

void* ForbiddenAllocator::Allocate(std::size_t size)
{
	assert(false);
	return nullptr;
}

bool ForbiddenAllocator::Deallocate(void*& ptr)
{
	assert(false);
	return false;
}

bool ForbiddenAllocator::Owns(const void* ptr) const
{
	return false;
}

void* Mallocator::Allocate(std::size_t size)
{
	void* ptr = nullptr;
	if (size > 0)
	{
		ptr = Malloc(size);
	}
	return ptr;
}

bool Mallocator::Deallocate(void*& ptr)
{
	if (ptr != nullptr)
	{
		Free(ptr);
		ptr = nullptr;
		return true;
	}
	return false;
}

bool Mallocator::Owns(const void* ptr) const
{
	return false;
}

StackAllocator::StackAllocator(MemorySource& source)
	: mSource(source)
	, mPointer(reinterpret_cast<std::uintptr_t>(mSource.GetPointer()))
{
}

void* StackAllocator::Allocate(std::size_t size)
{
	void* ptr = nullptr;
	const std::size_t alignedSize = RoundToAlignment(size, GetAlignment());
	if (size > 0 && alignedSize <= GetRemainingSize())
	{
		ptr = reinterpret_cast<void*>(mPointer);
		mPointer += alignedSize;
	}
	return ptr;
}

bool StackAllocator::Deallocate(void*& ptr)
{
	// You should only deallocate the last allocated block
	if (ptr != nullptr)
	{
		mPointer = reinterpret_cast<std::uintptr_t>(ptr);
		ptr = nullptr;
		return true;
	}
	return false;
}

bool StackAllocator::Owns(const void* ptr) const
{
	return mSource.Owns(ptr);
}

void StackAllocator::DeallocateAll()
{
	mPointer = reinterpret_cast<std::uintptr_t>(mSource.GetPointer());
}

std::size_t StackAllocator::GetUsedSize() const
{
	return mPointer - reinterpret_cast<std::uintptr_t>(mSource.GetPointer());
}

std::size_t StackAllocator::GetRemainingSize() const
{
	return (reinterpret_cast<std::uintptr_t>(mSource.GetPointer()) + mSource.GetSize()) - mPointer;
}

std::size_t StackAllocator::GetSize() const
{
	return mSource.GetSize();
}

std::size_t StackAllocator::GetAlignment() const
{
	return mSource.GetAlignment();
}

PoolAllocator::PoolAllocator(MemorySource& source, std::size_t blockSize)
	: mSource(source)
	, mRootNode((Node*)mSource.GetPointer())
	, mBlockSize(blockSize)
{
	assert(mBlockSize > 0);
	assert(mBlockSize >= sizeof(void*));
	assert(mSource.GetSize() % mBlockSize == 0);

	Node* nodePtr = mRootNode;
	for (std::size_t i = 1; i < GetBlockCount() - 1; ++i)
	{
		Node* node = nodePtr;
		node->next = (Node*)(reinterpret_cast<std::uintptr_t>(mSource.GetPointer()) + i * mBlockSize);
		nodePtr = node->next;
	}
	nodePtr->next = nullptr;
}

void* PoolAllocator::Allocate(std::size_t size)
{
	void* ptr = nullptr;
	if (size == mBlockSize && mRootNode != nullptr)
	{
		ptr = (void*)mRootNode;
		mRootNode = mRootNode->next;
	}
	return ptr;
}

bool PoolAllocator::Deallocate(void*& ptr)
{
	if (mSource.Owns(ptr))
	{
		Node* node = (Node*)ptr;
		node->next = mRootNode;
		mRootNode = node;
		ptr = nullptr;
		return true;
	}
	return false;
}

bool PoolAllocator::Owns(const void* ptr) const
{
	return mSource.Owns(ptr);
}

std::size_t PoolAllocator::GetBlockSize() const
{
	return mBlockSize;
}

std::size_t PoolAllocator::GetBlockCount() const
{
	return mSource.GetSize() / mBlockSize;
}

std::size_t PoolAllocator::GetSize() const
{
	return mSource.GetSize();
}

FallbackAllocator::FallbackAllocator(Allocator& primaryAllocator, Allocator& secondaryAllocator)
	: mPrimary(primaryAllocator)
	, mSecondary(secondaryAllocator)
{
}

void* FallbackAllocator::Allocate(std::size_t size)
{
	void* ptr = mPrimary.Allocate(size);
	if (ptr == nullptr)
	{
		ptr = mSecondary.Allocate(size);
	}
	return ptr;
}

bool FallbackAllocator::Deallocate(void*& ptr)
{
	if (mPrimary.Owns(ptr))
	{
		return mPrimary.Deallocate(ptr);
	}
	else
	{
		return mSecondary.Deallocate(ptr);
	}
}

bool FallbackAllocator::Owns(const void* ptr) const
{
	return mPrimary.Owns(ptr) || mSecondary.Owns(ptr);
}

SegregatorAllocator::SegregatorAllocator(std::size_t threshold, Allocator& smaller, Allocator& larger)
	: mSmallerAllocator(smaller)
	, mLargerAllocator(larger)
	, mThreshold(threshold)
{
}

void* SegregatorAllocator::Allocate(std::size_t size)
{
	if (size <= mThreshold)
	{
		return mSmallerAllocator.Allocate(size);
	}
	else
	{
		return mLargerAllocator.Allocate(size);
	}
}

bool SegregatorAllocator::Deallocate(void*& ptr)
{
	if (mSmallerAllocator.Owns(ptr))
	{
		return mSmallerAllocator.Deallocate(ptr);
	}
	else
	{
		return mLargerAllocator.Deallocate(ptr);
	}
}

bool SegregatorAllocator::Owns(const void* ptr) const
{
	return mSmallerAllocator.Owns(ptr) || mLargerAllocator.Owns(ptr);
}

std::size_t SegregatorAllocator::GetThreshold() const
{
	return mThreshold;
}

} // namespace dyma