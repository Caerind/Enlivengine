#include <Enlivengine/System/ParserXml.hpp>

namespace en
{

ParserXml::ParserXml()
	: mFilename("")
{
}

ParserXml::ParserXml(const std::string& filename)
	: mFilename("")
{
	loadFromFile(filename);
}

void ParserXml::newFile()
{
	mDocument.reset();
	mCurrentNode = mDocument.root();
}

bool ParserXml::loadFromFile(const std::string& filename)
{
	if (mDocument.load_file(filename.c_str()))
	{
		mFilename = filename;
		mCurrentNode = mDocument.root();
		return true;
	}
	return false;
}

bool ParserXml::saveToFile(const std::string& filename)
{
	if (!filename.empty())
	{
		return mDocument.save_file(filename.c_str());
	}
	return mDocument.save_file(mFilename.c_str());
}

bool ParserXml::readNode(const std::string& childName)
{
	const char* n = childName.c_str();
	if (mCurrentNode.child(n))
	{
		mCurrentNode = mCurrentNode.child(n);
		return true;
	}
	return false;
}

bool ParserXml::nextSibling(const std::string& sibling)
{
	pugi::xml_node n = mCurrentNode.next_sibling(sibling.c_str());
	if (n)
	{
		mCurrentNode = n;
		return true;
	}
	return false;
}

void ParserXml::closeNode()
{
	mCurrentNode = mCurrentNode.parent();
}

bool ParserXml::createChild(const std::string& nodeName)
{
	if (mCurrentNode.append_child(nodeName.c_str()))
	{
		return true;
	}
	return false;
}

bool ParserXml::removeChild(const std::string& nodeName)
{
	return mCurrentNode.remove_child(nodeName.c_str());
}

void ParserXml::setAttribute(const std::string& attributeName, const std::string& value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		mCurrentNode.attribute(n) = value.c_str();
	}
	else
	{
		mCurrentNode.append_attribute(n) = value.c_str();
	}
}

void ParserXml::setAttribute(const std::string& attributeName, I32 value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		mCurrentNode.attribute(n) = value;
	}
	else
	{
		mCurrentNode.append_attribute(n) = value;
	}
}

void ParserXml::setAttribute(const std::string& attributeName, U32 value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		mCurrentNode.attribute(n) = value;
	}
	else
	{
		mCurrentNode.append_attribute(n) = value;
	}
}

void ParserXml::setAttribute(const std::string& attributeName, F32 value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		mCurrentNode.attribute(n) = value;
	}
	else
	{
		mCurrentNode.append_attribute(n) = value;
	}
}

void ParserXml::getAttribute(const std::string& attributeName, std::string& value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		value = mCurrentNode.attribute(n).as_string();
	}
}

void ParserXml::getAttribute(const std::string& attributeName, I32& value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		value = mCurrentNode.attribute(n).as_int();
	}
}

void ParserXml::getAttribute(const std::string& attributeName, U32& value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		value = mCurrentNode.attribute(n).as_uint();
	}
}

void ParserXml::getAttribute(const std::string& attributeName, F32& value)
{
	const char* n = attributeName.c_str();
	if (mCurrentNode.attribute(n))
	{
		value = mCurrentNode.attribute(n).as_float();
	}
}

void ParserXml::createAttribute(const std::string& attributeName)
{
	mCurrentNode.append_attribute(attributeName.c_str());
}

void ParserXml::removeAttribute(const std::string& attributeName)
{
	mCurrentNode.remove_attribute(attributeName.c_str());
}

void ParserXml::setValue(const std::string& value)
{
	mCurrentNode.text().set(value.c_str());
}

void ParserXml::getValue(std::string& value)
{
	value = mCurrentNode.text().as_string();
}

const std::string ParserXml::getNodeName() const
{
	return mCurrentNode.name();
}

const std::string& ParserXml::getFilename() const
{
	return mFilename;
}

} // namespace en