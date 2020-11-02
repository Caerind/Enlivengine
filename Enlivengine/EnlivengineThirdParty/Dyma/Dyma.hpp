#pragma once

#include <cstddef> // size_t
#include <cstdint> // uintptr_t

namespace dyma
{

// Basic functions
void* Malloc(std::size_t size);
void* Calloc(std::size_t num, std::size_t size);
void* Realloc(void* ptr, std::size_t newSize);
void Free(void* ptr);
void* AlignedMalloc(std::size_t size, std::size_t alignment);
void AlignedFree(void* ptr);
std::size_t RoundToAlignment(std::size_t size, std::size_t alignment);

// Memory source to feed an allocator with
class MemorySource
{
public:
	MemorySource() = default;

	virtual const void* GetPointer() const = 0;
	virtual std::size_t GetSize() const = 0;
	virtual std::size_t GetAlignment() const = 0;

	virtual const void* GetEndPointer() const;
	virtual bool Owns(const void* ptr) const;
	virtual bool OwnsMemory() const;
};

// Null memory
class NullMemory : public MemorySource
{
public:
	NullMemory() = default;

	const void* GetPointer() const override final;
	std::size_t GetSize() const override final;
	std::size_t GetAlignment() const override final;
	bool OwnsMemory() const override final;
};

// Memory from the stack
template <std::size_t Bytes, std::size_t Alignment = alignof(std::max_align_t)>
class StackMemory : public MemorySource
{
public:
	StackMemory() = default;

	const void* GetPointer() const override final { return reinterpret_cast<const void*>(mMemory); }
	std::size_t GetSize() const override final { return Bytes; }
	std::size_t GetAlignment() const override final { return Alignment; }

	// NonCopyable
	StackMemory(const StackMemory& other) = delete;
	StackMemory& operator=(const StackMemory& other) = delete;

	// NonMovable
	StackMemory(StackMemory&& other) = delete;
	StackMemory& operator=(StackMemory&& other) = delete;

private:
	alignas(Alignment) char mMemory[Bytes];
};

// Memory from the heap
class HeapMemory : public MemorySource
{
public:
	HeapMemory(std::size_t bytes, std::size_t alignment = 0);
	~HeapMemory();

	const void* GetPointer() const override final;
	std::size_t GetSize() const override final;
	std::size_t GetAlignment() const override final;

	// NonCopyable
	HeapMemory(const HeapMemory& other) = delete;
	HeapMemory& operator=(const HeapMemory& other) = delete;

	// NonMovable
	HeapMemory(HeapMemory&& other) = delete;
	HeapMemory& operator=(HeapMemory&& other) = delete;

private:
	void* mMemory;
	std::size_t mSize;
	std::size_t mAlignment;
	bool mAlignedByUser;
};

// Small view of a larger memory source
class MemoryView : public MemorySource
{
public:
	MemoryView(const void* begin, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));

	const void* GetPointer() const override final;
	std::size_t GetSize() const override final;
	std::size_t GetAlignment() const override final;
	bool OwnsMemory() const override final;

private:
	const void* mBegin;
	std::size_t mSize;
	std::size_t mAlignment;
};

// Allocator
class Allocator
{
public:
	Allocator() = default;

	virtual void* Allocate(std::size_t size) = 0;
	virtual bool Deallocate(void*& ptr) = 0;
	virtual bool Owns(const void* ptr) const = 0;

	// NonCopyable
	Allocator(const Allocator& other) = delete;
	Allocator& operator=(const Allocator& other) = delete;
};

// Null allocator
class NullAllocator : public Allocator
{
public:
	NullAllocator() = default;

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;
};

// Forbidden allocator : Every allocation/deallocation will assert()
class ForbiddenAllocator : public Allocator
{
public:
	ForbiddenAllocator() = default;

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;
};

// Mallocator : Malloc/Free allocator
// Owns() will always return false
class Mallocator : public Allocator
{
public:
	Mallocator() = default;

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;
};

// StackAllocator : Allocator with a stack mechanism
// Every deallocation should be in the correct order
class StackAllocator : public Allocator
{
public:
	StackAllocator(MemorySource& source);

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;

	void DeallocateAll();

	std::size_t GetUsedSize() const;
	std::size_t GetRemainingSize() const;
	std::size_t GetSize() const;
	std::size_t GetAlignment() const;

protected:
	MemorySource& mSource;
	std::uintptr_t mPointer;
};

// PoolAllocator : Allocator specialized for same sized-blocks
// The block size should be greater than or equals to the size of a pointer
class PoolAllocator : public Allocator
{
public:
	PoolAllocator(MemorySource& source, std::size_t blockSize);

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;

	std::size_t GetBlockSize() const;
	std::size_t GetBlockCount() const;
	std::size_t GetSize() const;

protected:
	struct Node
	{
		Node* next;
	};

	MemorySource& mSource;
	Node* mRootNode;
	std::size_t mBlockSize;
};

// FallbackAllocator : Try the primary allocator, then the secondary if the primary failed
class FallbackAllocator : public Allocator
{
public:
	FallbackAllocator(Allocator& primaryAllocator, Allocator& secondaryAllocator);

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;

protected:
	Allocator& mPrimary;
	Allocator& mSecondary;
};

// SegregatorAllocator : Splits allocations between allocators depending on the size of the block
class SegregatorAllocator : public Allocator
{
public:
	SegregatorAllocator(std::size_t threshold, Allocator& smaller, Allocator& larger);

	void* Allocate(std::size_t size) override;
	bool Deallocate(void*& ptr) override;
	bool Owns(const void* ptr) const override;

	std::size_t GetThreshold() const;

private:
	Allocator& mSmallerAllocator;
	Allocator& mLargerAllocator;
	std::size_t mThreshold;
};

} // namespace dyma
