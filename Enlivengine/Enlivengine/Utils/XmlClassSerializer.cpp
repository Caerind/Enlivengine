#include <Enlivengine/Utils/XmlClassSerializer.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/String.hpp>

namespace en
{

bool XmlClassSerializer::IsOpened() const
{
	return mMode != Mode::None;
}

bool XmlClassSerializer::Open(const std::string& filename, Serializer::Mode mode)
{
	if (IsOpened())
		Close();

	enAssert(!IsOpened());
	if (mode == Mode::Read)
	{
		if (mParser.LoadFromFile(filename))
		{
			mMode = Mode::Read;
			mFilename = filename;
		}
		else
		{
			return false;
		}
	}
	else if (mode == Mode::Write)
	{
		mParser.NewFile();
		mMode = Mode::Write;
		mFilename = filename;
	}
	else
	{
		enAssert(false);
		return false;
	}

	return true;
}

bool XmlClassSerializer::Close()
{
	if (mMode == Mode::Write)
	{
		const bool ret = mParser.SaveToFile(mFilename);
		mFilename = "";
		mMode = Mode::None;
		return ret;

	}
	else if (mMode == Mode::Read)
	{
		mFilename = "";
		mMode = Mode::None;
		return true;
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::BeginClass(const char* name, U32 classTypeHash)
{
	enAssert(IsOpened());
	if (mMode == Serializer::Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			U32 nodeTypeAttribute = 0;
			mParser.GetAttribute("type", nodeTypeAttribute);
			return nodeTypeAttribute == classTypeHash;
		}
	}
	else if (mMode == Serializer::Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetAttribute("type", classTypeHash);
			return true;
		}
	}
	return false;
}

bool XmlClassSerializer::EndClass()
{
	enAssert(IsOpened());
	mParser.CloseNode();
	return true;
}

bool XmlClassSerializer::Serialize(const char* name, bool& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp; 
			mParser.GetValue(temp);
			value = FromBoolString(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToBoolString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, char& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<char>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, I8& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<I8>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, U8& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<U8>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, I16& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<I16>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, U16& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<U16>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, I32& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<I32>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, U32& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<U32>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, I64& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<I64>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, U64& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<U64>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, F32& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<F32>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, F64& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			std::string temp;
			mParser.GetValue(temp);
			value = FromString<F64>(temp);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(ToString(value));
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

bool XmlClassSerializer::Serialize(const char* name, std::string& value)
{
	enAssert(IsOpened());
	if (mMode == Mode::Read)
	{
		if (mParser.ReadNode(name))
		{
			mParser.GetValue(value);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (mMode == Mode::Write)
	{
		if (mParser.CreateNode(name))
		{
			mParser.SetValue(value);
			mParser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		enAssert(false);
		return false;
	}
}

} // namespace en