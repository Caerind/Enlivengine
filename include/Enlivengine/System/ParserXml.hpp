#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <string>
#include <pugixml/pugixml.hpp>

namespace en
{

class ParserXml
{
	public:
		ParserXml();
		ParserXml(const std::string& filename);

		void newFile();
		bool loadFromFile(const std::string& filename);
		bool saveToFile(const std::string& filename = "");

		bool readNode(const std::string& childName);
		bool nextSibling(const std::string& sibling = "");
		void closeNode();

		bool createChild(const std::string& nodeName);
		bool removeChild(const std::string& nodeName);
		
		void setAttribute(const std::string& attributeName, const std::string& value);
		void setAttribute(const std::string& attributeName, I32 value);
		void setAttribute(const std::string& attributeName, U32 value);
		void setAttribute(const std::string& attributeName, F32 value);
		void getAttribute(const std::string& attributeName, std::string& value);
		void getAttribute(const std::string& attributeName, I32& value);
		void getAttribute(const std::string& attributeName, U32& value);
		void getAttribute(const std::string& attributeName, F32& value);
		void createAttribute(const std::string& attributeName);
		void removeAttribute(const std::string& attributeName);

		void setValue(const std::string& value);
		void getValue(std::string& value);

		const std::string getNodeName() const;

		const std::string& getFilename() const;
	private:
		pugi::xml_document mDocument;
		pugi::xml_node mCurrentNode;
		std::string mFilename;
};

} // namespace en