#pragma once

#include <Enlivengine/MetaData/MetaData.hpp>

#include <Enlivengine/System/Array.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/MetaData/TestClassA.hpp>

class TestClassC : public en::TestClassA
{
public:
	ENLIVE_META_CLASS_DECL(TestClassC)

	en::Array<en::U32>& GetB() { return b; }
	const en::Array<en::U32>& GetB() const { return b; }
	en::Array<en::TestClassA>& GetC() { return c; }
	const en::Array<en::TestClassA>& GetC() const { return c; }

	void SetDuration(const en::Time& t) { d = t; }
	const en::Time& GetDuration() const { return d; }

private:
	en::Array<en::U32> b;
	en::Array<en::TestClassA> c;
	en::Time d;
};

namespace en
{
	ENLIVE_META_CLASS_DEF(TestClassC)
		ENLIVE_META_CLASS_PROPERTY(U32, TestClassA::a) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(I32, TestClassA::b) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(F32, TestClassA::c) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(MyEnum, TestClassA::d) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(bool, TestClassA::e) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(Array<U32>, b) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(Array<TestClassA>, c) ENLIVE_METADATA_COMMA()
		ENLIVE_META_CLASS_PROPERTY(Time, d)
	ENLIVE_META_CLASS_DEF_END_EX(&MetaData<TestClassA>::Get())
} // namespace en
