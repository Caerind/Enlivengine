#pragma once

#include <Enlivengine/MetaData/MetaData.hpp>

namespace en
{

ENLIVE_META_ENUM(MyEnum)
{
    A,
    B,
    C
};
ENLIVE_META_ENUM_DEF(MyEnum)
	ENLIVE_META_ENUM_VALUE(A) ENLIVE_METADATA_COMMA()
	ENLIVE_META_ENUM_VALUE(B) ENLIVE_METADATA_COMMA()
	ENLIVE_META_ENUM_VALUE(C)
ENLIVE_META_ENUM_DEF_END()

class TestClassA
{
public:
	ENLIVE_META_CLASS_DECL(TestClassA);

	U32 GetA() const { return a; }
	I32 GetB() const { return b; }
	F32 GetC() const { return c; }
    MyEnum GetD() const { return d; }
    bool GetE() const { return e; }

	void SetA(U32 v) { a = v; }
	void SetB(I32 v) { b = v; }
	void SetC(F32 v) { c = v; }
    void SetD(MyEnum v) { d = v; }
    void SetE(bool v) { e = v; }

protected:
	U32 a;
	I32 b;
	F32 c;
    MyEnum d;
    bool e;
};

ENLIVE_META_CLASS_DEF(TestClassA)
	ENLIVE_META_CLASS_PROPERTY(U32, a) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(I32, b) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(F32, c) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(MyEnum, d) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(bool, e)
ENLIVE_META_CLASS_DEF_END()

} // namespace en