#pragma once

#include <Enlivengine/Utils/Serializer.hpp>
#include <Enlivengine/Utils/ParserXml.hpp>

namespace en
{

class XmlClassSerializer : public ClassSerializer
{
public:
	virtual bool IsOpened() const;
	virtual bool Open(const std::string& filename, Serializer::Mode mode);
	virtual bool Close();

	virtual bool BeginClass(const char* name, U32 classTypeHash);
	virtual bool EndClass();

	virtual bool Serialize(const char* name, bool& value);
	virtual bool Serialize(const char* name, char& value);
	virtual bool Serialize(const char* name, I8& value);
	virtual bool Serialize(const char* name, U8& value);
	virtual bool Serialize(const char* name, I16& value);
	virtual bool Serialize(const char* name, U16& value);
	virtual bool Serialize(const char* name, I32& value);
	virtual bool Serialize(const char* name, U32& value);
	virtual bool Serialize(const char* name, I64& value);
	virtual bool Serialize(const char* name, U64& value);
	virtual bool Serialize(const char* name, F32& value);
	virtual bool Serialize(const char* name, F64& value);
	virtual bool Serialize(const char* name, std::string& value);

private:
	ParserXml mParser;
	std::string mFilename;
};

} // namespace en