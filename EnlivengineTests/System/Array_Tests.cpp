#include <Enlivengine/System/Array.hpp>

#include <doctest/doctest.h>

// TODO : This should be improved a LLLLLOOOOOOTTTT

DOCTEST_TEST_CASE("U32 array")
{
	// Default ctor
	en::Array<en::U32> arr;
	DOCTEST_CHECK(arr.Size() == 0);
	DOCTEST_CHECK(arr.Capacity() == 0);
	DOCTEST_CHECK(arr.GetElementSize() == ENLIVE_SIZE_OF(en::U32));
	DOCTEST_CHECK(arr.GetElementAlignment() == ENLIVE_ALIGN_OF(en::U32));
	DOCTEST_CHECK(arr.GetArrayData() == nullptr);
	DOCTEST_CHECK(arr.Empty());

	// Add
	arr.Add(3);
	DOCTEST_CHECK(arr.Size() == 1);
	DOCTEST_CHECK(arr.Capacity() >= 1);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0] == 3);
	arr.Add(10);
	DOCTEST_CHECK(arr.Size() == 2);
	DOCTEST_CHECK(arr.Capacity() >= 2);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr[1] == 10);
	arr.Add(6);
	DOCTEST_CHECK(arr.Size() == 3);
	DOCTEST_CHECK(arr.Capacity() >= 3);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr[1] == 10);
	DOCTEST_CHECK(arr[2] == 6);

	// Front + Back
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr.Front() == 3);
	DOCTEST_CHECK(arr[2] == 6);
	DOCTEST_CHECK(arr.Back() == 6);

	// Sort + access
	arr.Sort();
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr[1] == 6);
	DOCTEST_CHECK(arr[2] == 10);
	arr.Sort([](const auto& a, const auto& b) { return a > b; });
	DOCTEST_CHECK(arr[0] == 10);
	DOCTEST_CHECK(arr[1] == 6);
	DOCTEST_CHECK(arr[2] == 3);

	// Copy + Equality
	en::Array<en::U32> arr2;
	arr2.Copy(arr);
	DOCTEST_CHECK(arr2.Size() == arr.Size());
	DOCTEST_CHECK(arr2.Size() == 3);
	DOCTEST_CHECK(arr2.Capacity() <= arr.Capacity());
	DOCTEST_CHECK(arr2.Capacity() >= 3);
	DOCTEST_CHECK(arr2 == arr);
	DOCTEST_CHECK(arr == arr2);
	DOCTEST_CHECK(arr2.Equals(arr));
	DOCTEST_CHECK(arr.Equals(arr2));
	arr2[1] = arr[1] + 1;
	DOCTEST_CHECK(!arr2.Equals(arr));
	DOCTEST_CHECK(!arr.Equals(arr2));
	arr2[1] = arr[1];
	arr2.Add(4);
	DOCTEST_CHECK(!arr2.Equals(arr));
	DOCTEST_CHECK(!arr.Equals(arr2));

	// Loops
	struct LoopInfo
	{
		en::U32 sum = 0;
		en::U32 mul = 1;
		bool operator==(const LoopInfo& other) const { return sum == other.sum && mul == other.mul; }
		bool operator!=(const LoopInfo& other) const { return !operator==(other); }
	};
	LoopInfo viaIndex;
	LoopInfo viaItr;
	LoopInfo viaCItr;
	for (en::U32 i = 0; i < arr.Size(); ++i)
	{
		viaIndex.sum += arr[i];
		viaIndex.mul *= arr[i];
	}
	en::Array<en::U32>::Iterator itr = arr.Begin();
	en::Array<en::U32>::Iterator end = arr.End();
	for (; itr != end; ++itr)
	{
		viaItr.sum += *itr;
		viaItr.mul *= *itr;
	}
	DOCTEST_CHECK(viaIndex == viaItr);
	en::Array<en::U32>::ConstIterator citr = arr.Begin();
	en::Array<en::U32>::ConstIterator cend = arr.End(); 
	for (; citr != cend; ++citr)
	{
		viaCItr.sum += *citr;
		viaCItr.mul *= *citr;
	}
	DOCTEST_CHECK(viaIndex == viaCItr);

	// Clear + Resize + Empty
	arr2.Clear();
	DOCTEST_CHECK(arr2.Size() == 0);
	DOCTEST_CHECK(arr2.Empty());
	arr2.Add(1);
	DOCTEST_CHECK(arr2.Size() == 1);
	DOCTEST_CHECK(!arr2.Empty());
	DOCTEST_CHECK(arr2.Capacity() >= 1);
	arr2.Resize(3);
	DOCTEST_CHECK(arr2.Size() == 3);
	DOCTEST_CHECK(!arr2.Empty());
	DOCTEST_CHECK(arr2.Capacity() >= 3);

	// Get NextReservedSize
	for (en::U32 i = 0; i < 1000; ++i)
	{
		DOCTEST_CHECK(en::Array<en::U32>::GetNextCapacity(i) > i);
	}
}

/*
class TestElement
{
public:
	TestElement() : mData(0), mData1(0), mData2(0) {}
	TestElement(en::U32 d) : mData(d), mData1(0), mData2(0) {}
	TestElement(const TestElement& elem) : mData(elem.mData), mData1(0), mData2(0) {}
	~TestElement() {}

	en::U32 GetData() const { return mData; }

	bool operator==(const TestElement& elem) const { return mData == elem.mData; }
	bool operator!=(const TestElement& elem) const { return mData != elem.mData; }

private:
	en::U32 mData;
	en::U32 mData1;
	en::U32 mData2;
};

DOCTEST_TEST_CASE("TestElement array")
{
	// Default ctor
	en::Array<TestElement> arr;
	DOCTEST_CHECK(arr.Size() == 0);
	DOCTEST_CHECK(arr.Capacity() == 0);
	DOCTEST_CHECK(arr.GetElementSize() == ENLIVE_SIZE_OF(TestElement));
	DOCTEST_CHECK(arr.GetElementAlignment() == ENLIVE_ALIGN_OF(TestElement));
	DOCTEST_CHECK(arr.GetArrayData() == nullptr);
	DOCTEST_CHECK(arr.Empty());

	// Add
	arr.Add(TestElement(3));
	DOCTEST_CHECK(arr.Size() == 1);
	DOCTEST_CHECK(arr.Capacity() >= 1);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0].GetData() == 3);
	arr.Add(TestElement(10));
	DOCTEST_CHECK(arr.Size() == 2);
	DOCTEST_CHECK(arr.Capacity() >= 2);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0].GetData() == 3);
	DOCTEST_CHECK(arr[1].GetData() == 10);
	arr.Add(6);
	DOCTEST_CHECK(arr.Size() == 3);
	DOCTEST_CHECK(arr.Capacity() >= 3);
	DOCTEST_CHECK(arr.GetArrayData() != nullptr);
	DOCTEST_CHECK(!arr.Empty());
	DOCTEST_CHECK(arr[0].GetData() == 3);
	DOCTEST_CHECK(arr[1].GetData() == 10);
	DOCTEST_CHECK(arr[2].GetData() == 6);

	// Front + Back
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr.Front() == 3);
	DOCTEST_CHECK(arr[2] == 6);
	DOCTEST_CHECK(arr.Back() == 6);

	// Sort + access
	arr.Sort();
	DOCTEST_CHECK(arr[0] == 3);
	DOCTEST_CHECK(arr[1] == 6);
	DOCTEST_CHECK(arr[2] == 10);
	arr.Sort([](const auto& a, const auto& b) { return a > b; });
	DOCTEST_CHECK(arr[0] == 10);
	DOCTEST_CHECK(arr[1] == 6);
	DOCTEST_CHECK(arr[2] == 3);

	// Copy + Equality
	en::Array<en::U32> arr2;
	arr2.Copy(arr);
	DOCTEST_CHECK(arr2.Size() == arr.Size());
	DOCTEST_CHECK(arr2.Size() == 3);
	DOCTEST_CHECK(arr2.Capacity() <= arr.Capacity());
	DOCTEST_CHECK(arr2.Capacity() >= 3);
	DOCTEST_CHECK(arr2 == arr);
	DOCTEST_CHECK(arr == arr2);
	DOCTEST_CHECK(arr2.Equals(arr));
	DOCTEST_CHECK(arr.Equals(arr2));
	arr2[1] = arr[1] + 1;
	DOCTEST_CHECK(!arr2.Equals(arr));
	DOCTEST_CHECK(!arr.Equals(arr2));
	arr2[1] = arr[1];
	arr2.Add(4);
	DOCTEST_CHECK(!arr2.Equals(arr));
	DOCTEST_CHECK(!arr.Equals(arr2));

	// Loops
	struct LoopInfo
	{
		en::U32 sum = 0;
		en::U32 mul = 1;
		bool operator==(const LoopInfo& other) const { return sum == other.sum && mul == other.mul; }
		bool operator!=(const LoopInfo& other) const { return !operator==(other); }
	};
	LoopInfo viaIndex;
	LoopInfo viaItr;
	LoopInfo viaCItr;
	for (en::U32 i = 0; i < arr.Size(); ++i)
	{
		viaIndex.sum += arr[i];
		viaIndex.mul *= arr[i];
	}
	en::Array<en::U32>::Iterator itr = arr.Begin();
	en::Array<en::U32>::Iterator end = arr.End();
	for (; itr != end; ++itr)
	{
		viaItr.sum += *itr;
		viaItr.mul *= *itr;
	}
	DOCTEST_CHECK(viaIndex == viaItr);
	en::Array<en::U32>::ConstIterator citr = arr.Begin();
	en::Array<en::U32>::ConstIterator cend = arr.End();
	for (; citr != cend; ++citr)
	{
		viaCItr.sum += *citr;
		viaCItr.mul *= *citr;
	}
	DOCTEST_CHECK(viaIndex == viaCItr);

	// Clear + Resize + Empty
	arr2.Clear();
	DOCTEST_CHECK(arr2.Size() == 0);
	DOCTEST_CHECK(arr2.Empty());
	arr2.Add(1);
	DOCTEST_CHECK(arr2.Size() == 1);
	DOCTEST_CHECK(!arr2.Empty());
	DOCTEST_CHECK(arr2.Capacity() >= 1);
	arr2.Resize(3);
	DOCTEST_CHECK(arr2.Size() == 3);
	DOCTEST_CHECK(!arr2.Empty());
	DOCTEST_CHECK(arr2.Capacity() >= 3);

	// Get NextReservedSize
	for (en::U32 i = 0; i < 1000; ++i)
	{
		DOCTEST_CHECK(en::Array<en::U32>::GetNextCapacity(i) > i);
	}
}
*/