#include <Enlivengine/System/ParserXml.hpp>

namespace en
{

ParserXml::ParserXml()
	: mFilename("")
{
}

void ParserXml::NewFile()
{
	mDocument.reset();
	mCurrentNode = mDocument.root();
}

bool ParserXml::LoadFromFile(const std::string& filename)
{
	if (mDocument.load_file(filename.c_str()))
	{
		mFilename = filename;
		mCurrentNode = mDocument.root();
		return true;
	}
	return false;
}

bool ParserXml::SaveToFile(const std::string& filename)
{
	return mDocument.save_file(!filename.empty() ? filename.c_str() : mFilename.c_str());
}

bool ParserXml::HasNode(const std::string& nodeName) const
{
	return mCurrentNode.child(nodeName.c_str());
}

bool ParserXml::ReadNode(const std::string& nodeName)
{
	if (auto node = mCurrentNode.child(nodeName.c_str()))
	{
		mCurrentNode = node;
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::ReadFirstNode()
{
	if (auto n = mCurrentNode.first_child())
	{
		mCurrentNode = n;
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::NextSibling(const std::string& sibling)
{
	if (auto n = ((sibling == "") ? mCurrentNode.next_sibling() : mCurrentNode.next_sibling(sibling.c_str())))
	{
		mCurrentNode = n;
		return true;
	}
	else
	{
		return false;
	}
}

void ParserXml::CloseNode()
{
	mCurrentNode = mCurrentNode.parent();
}

bool ParserXml::CreateNode(const std::string& nodeName)
{
	if (auto n = mCurrentNode.append_child(nodeName.c_str()))
	{
		mCurrentNode = n;
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::RemoveNode(const std::string& nodeName)
{
	return mCurrentNode.remove_child(nodeName.c_str());
}

bool ParserXml::HasAttribute(const std::string& attributeName) const
{
	if (mCurrentNode.attribute(attributeName.c_str()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ParserXml::SetAttribute(const std::string& attributeName, const std::string& value)
{
	const char* val = value.c_str();
	const char* attrName = attributeName.c_str();
	if (auto attr = mCurrentNode.attribute(attrName))
	{
		attr = val;
	}
	else
	{
		mCurrentNode.append_attribute(attrName) = val;
	}
}

void ParserXml::SetAttribute(const std::string& attributeName, I32 value)
{
	const char* attrName = attributeName.c_str();
	if (auto attr = mCurrentNode.attribute(attrName))
	{
		attr = value;
	}
	else
	{
		mCurrentNode.append_attribute(attrName) = value;
	}
}

void ParserXml::SetAttribute(const std::string& attributeName, U32 value)
{
	const char* attrName = attributeName.c_str();
	if (auto attr = mCurrentNode.attribute(attrName))
	{
		attr = value;
	}
	else
	{
		mCurrentNode.append_attribute(attrName) = value;
	}
}

void ParserXml::SetAttribute(const std::string& attributeName, F32 value)
{
	const char* attrName = attributeName.c_str();
	if (auto attr = mCurrentNode.attribute(attrName))
	{
		attr = value;
	}
	else
	{
		mCurrentNode.append_attribute(attrName) = value;
	}
}

bool ParserXml::GetAttribute(const std::string& attributeName, std::string& value) const
{
	if (auto attr = mCurrentNode.attribute(attributeName.c_str()))
	{
		value = attr.as_string();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::GetAttribute(const std::string& attributeName, I32& value) const
{
	if (auto attr = mCurrentNode.attribute(attributeName.c_str()))
	{
		value = attr.as_int();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::GetAttribute(const std::string& attributeName, U32& value) const
{
	if (auto attr = mCurrentNode.attribute(attributeName.c_str()))
	{
		value = attr.as_uint();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParserXml::GetAttribute(const std::string& attributeName, F32& value) const
{
	if (auto attr = mCurrentNode.attribute(attributeName.c_str()))
	{
		value = attr.as_float();
		return true;
	}
	else
	{
		return false;
	}
}

void ParserXml::CreateAttribute(const std::string& attributeName)
{
	mCurrentNode.append_attribute(attributeName.c_str());
}

void ParserXml::RemoveAttribute(const std::string& attributeName)
{
	mCurrentNode.remove_attribute(attributeName.c_str());
}

void ParserXml::SetValue(const std::string& value)
{
	mCurrentNode.text().set(value.c_str());
}

void ParserXml::GetValue(std::string& value) const
{
	value = mCurrentNode.text().as_string();
}

const std::string ParserXml::GetNodeName() const
{
	return mCurrentNode.name();
}

const std::string& ParserXml::GetFilename() const
{
	return mFilename;
}

} // namespace en