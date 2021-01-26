#pragma once

#include <Enlivengine/Utils/Serializer.hpp>
#include <Enlivengine/Utils/ParserXml.hpp>

namespace en
{

class XmlSerializer : public Serializer
{
public:
	bool IsOpened() const override;
	bool Open(const std::string& filename, Serializer::Mode mode) override;
	bool Close() override;

	bool BeginClass(const char* name, const char* className, U32 classTypeHash) override;
	bool EndClass() override;

	bool HasNode(const char* name) override;

	bool Serialize(const char* name, bool& value) override;
	bool Serialize(const char* name, char& value) override;
	bool Serialize(const char* name, I8& value) override;
	bool Serialize(const char* name, U8& value) override;
	bool Serialize(const char* name, I16& value) override;
	bool Serialize(const char* name, U16& value) override;
	bool Serialize(const char* name, I32& value) override;
	bool Serialize(const char* name, U32& value) override;
	bool Serialize(const char* name, I64& value) override;
	bool Serialize(const char* name, U64& value) override;
	bool Serialize(const char* name, F32& value) override;
	bool Serialize(const char* name, F64& value) override;
	bool Serialize(const char* name, std::string& value) override;

private:
	ParserXml mParser;
	std::string mFilename;
};

} // namespace en