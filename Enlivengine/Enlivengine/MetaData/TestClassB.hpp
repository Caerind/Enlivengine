#pragma once

#include <Enlivengine/MetaData/MetaData.hpp>

#include <Enlivengine/MetaData/TestClassA.hpp>

namespace en
{

class TestClassB
{
public:
	ENLIVE_META_CLASS_DECL(TestClassB)

	TestClassA* GetA() const { return a; }
	F32 GetB() const { return b; }
    U32 GetC() const { return c; }
    F32* GetD() const { return d; }
    MyEnum* GetE() const { return e; }
    const TestClassA& GetF(U32 index) const { return f[index]; }
    F32* GetG(U32 index) const { return g[index]; }

	void SetA(TestClassA* v) { a = v; }
	void SetB(F32 v) { b = v; }
	void SetC(U32 v) { c = v; }
	void SetD(F32* v) { d = v; }
	void SetE(MyEnum* v) { e = v; }
    void SetF(U32 index, const TestClassA& v) { f[index] = v; }
    void SetG(U32 index, F32* v) { g[index] = v; }

private:
    TestClassA* a; // Class/Ptr
    F32 b; // Primitive
    U32 c; // Primitive
    F32* d; // Primitive/Ptr
    MyEnum* e; // Enum/Ptr
	TestClassA f[3]; // Class/Array
	F32* g[2]; // Primitive/Array/Ptr
	// TODO : U32 (*h)[]; // Primitive/ArrayPtr
	// TODO : I32* (*i)[]; // Primitive/Ptr/ArrayPtr
};

ENLIVE_META_CLASS_DEF(TestClassB)
	ENLIVE_META_CLASS_PROPERTY_TRAITS(TestClassA, a, en::TypeTraits_Pointer) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(F32, b) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY(U32, c) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY_TRAITS(F32, d, en::TypeTraits_Pointer) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY_TRAITS(MyEnum, e, en::TypeTraits_Pointer) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY_ARRAY(TestClassA, f, 3) ENLIVE_METADATA_COMMA()
	ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY(F32, g, en::TypeTraits_Pointer, 2)
	// TODO : en::MetaDataProperty(en::Hash::CRC32("TestClassB::h"), en::MetaDataPrimitiveTypes::GetType<en::U32>(), "h", ENLIVE_OFFSET_OF(en::TestClassB, h), ENLIVE_SIZE_OF_MEMBER(en::TestClassB, h), en::MetaDataPrimitiveTypes::GetType<en::U32>().GetTraits() | en::TypeTraits_ArrayPtr),
	// TODO : en::MetaDataProperty(en::Hash::CRC32("TestClassB::i"), en::MetaDataPrimitiveTypes::GetType<en::I32>(), "i", ENLIVE_OFFSET_OF(en::TestClassB, i), ENLIVE_SIZE_OF_MEMBER(en::TestClassB, i), en::MetaDataPrimitiveTypes::GetType<en::I32>().GetTraits() | en::TypeTraits_ArrayPtr | en::TypeTraits_Pointer)
ENLIVE_META_CLASS_DEF_END()

} // namespace en