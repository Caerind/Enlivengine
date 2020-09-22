#pragma once

#include <string>

#include <pugixml/pugixml.hpp>

#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

class ParserXml
{
public:
	ParserXml();

	void NewFile();
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename = "");

	bool HasNode(const std::string& nodeName) const;
	bool ReadNode(const std::string& nodeName);
	bool ReadFirstNode();
	bool NextSibling(const std::string& sibling = "");
	void CloseNode();
	bool CreateNode(const std::string& nodeName);
	bool RemoveNode(const std::string& nodeName);

	bool HasAttribute(const std::string& attributeName) const;
	void SetAttribute(const std::string& attributeName, const std::string& value);
	void SetAttribute(const std::string& attributeName, I32 value);
	void SetAttribute(const std::string& attributeName, U32 value);
	void SetAttribute(const std::string& attributeName, F32 value);
	bool GetAttribute(const std::string& attributeName, std::string& value) const;
	bool GetAttribute(const std::string& attributeName, I32& value) const;
	bool GetAttribute(const std::string& attributeName, U32& value) const;
	bool GetAttribute(const std::string& attributeName, F32& value) const;
	void CreateAttribute(const std::string& attributeName);
	void RemoveAttribute(const std::string& attributeName);

	void SetValue(const std::string& value);
	void GetValue(std::string& value) const;

	const std::string GetNodeName() const;
	const std::string& GetFilename() const;

private:
	pugi::xml_document mDocument;
	pugi::xml_node mCurrentNode;
	std::string mFilename;
};

} // namespace en