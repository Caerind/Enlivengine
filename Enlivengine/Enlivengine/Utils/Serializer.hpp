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

class Serializer : private NonCopyable
{
public:
	enum class Mode
	{
		None,
		Read,
		Write
	};
	Mode GetMode() const { return mMode; }
	bool IsReading() const { return mMode == Mode::Read; }
	bool IsWriting() const { return mMode == Mode::Write; }

	virtual bool Serialize(const char* name, bool& value) = 0;
	virtual bool Serialize(const char* name, char& value) = 0;
	virtual bool Serialize(const char* name, I8& value) = 0;
	virtual bool Serialize(const char* name, U8& value) = 0;
	virtual bool Serialize(const char* name, I16& value) = 0;
	virtual bool Serialize(const char* name, U16& value) = 0;
	virtual bool Serialize(const char* name, I32& value) = 0;
	virtual bool Serialize(const char* name, U32& value) = 0;
	virtual bool Serialize(const char* name, I64& value) = 0;
	virtual bool Serialize(const char* name, U64& value) = 0;
	virtual bool Serialize(const char* name, F32& value) = 0;
	virtual bool Serialize(const char* name, F64& value) = 0;
	virtual bool Serialize(const char* name, std::string& value) = 0;

protected:
	Mode mMode{ Mode::None };
};

class ClassSerializer : public Serializer
{
public:
	virtual bool IsOpened() const = 0;
	virtual bool Open(const std::string& filename, Serializer::Mode mode) = 0;
	virtual bool Close() = 0;

	virtual bool BeginClass(const char* name, U32 classTypeHash) = 0;
	virtual bool EndClass() = 0;

	virtual bool HasNode(const char* name) = 0;
};

template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, T& object);
template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, const T& object);

template <typename T, std::size_t N> bool GenericSerialization(ClassSerializer& serializer, const char* name, std::array<T, N>& object);
template <typename T, std::size_t N> bool GenericSerialization(ClassSerializer& serializer, const char* name, const std::array<T, N>& object);

template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, std::vector<T>& object);
template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, const std::vector<T>& object);

template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, Array<T>& object);
template <typename T> bool GenericSerialization(ClassSerializer& serializer, const char* name, const Array<T>& object);

} // namespace en

#include <Enlivengine/Utils/Serializer.inl>