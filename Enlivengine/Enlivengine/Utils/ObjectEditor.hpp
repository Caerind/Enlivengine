#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/NonCopyable.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Utils/Enums.hpp>
#include <string>

#include <vector>
#include <array>
#include <Enlivengine/Utils/Array.hpp>

namespace en
{

class ObjectEditor : private NonCopyable
{
public:
	virtual bool BeginClass(const char* name, const char* className, U32 classTypeHash) = 0;
	virtual bool EndClass() = 0;

	virtual bool IsImGuiEditor() const = 0;

	virtual bool Edit(const char* name, bool& value) = 0;
	virtual bool Edit(const char* name, char& value) = 0;
	virtual bool Edit(const char* name, I8& value) = 0;
	virtual bool Edit(const char* name, U8& value) = 0;
	virtual bool Edit(const char* name, I16& value) = 0;
	virtual bool Edit(const char* name, U16& value) = 0;
	virtual bool Edit(const char* name, I32& value) = 0;
	virtual bool Edit(const char* name, U32& value) = 0;
	virtual bool Edit(const char* name, I64& value) = 0;
	virtual bool Edit(const char* name, U64& value) = 0;
	virtual bool Edit(const char* name, F32& value) = 0;
	virtual bool Edit(const char* name, F64& value) = 0;
	virtual bool Edit(const char* name, std::string& value) = 0;
};

template <typename T> bool GenericEdit(ObjectEditor& objectEditor, const char* name, T& object);
template <typename T, std::size_t N> bool GenericEdit(ObjectEditor& objectEditor, const char* name, std::array<T, N>& object);
template <typename T> bool GenericEdit(ObjectEditor& objectEditor, const char* name, std::vector<T>& object);
template <typename T> bool GenericEdit(ObjectEditor& objectEditor, const char* name, Array<T>& object);

namespace priv
{

template <typename ArrayType, typename ObjectType> bool GenericEditArray(ObjectEditor& objectEditor, const char* name, U32 size, ArrayType& arrayObject);

} // namespace priv

} // namespace en

#include <Enlivengine/Utils/ObjectEditor.inl>