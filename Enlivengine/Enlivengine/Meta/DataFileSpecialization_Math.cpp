#include <Enlivengine/Meta/DataFileSpecialization_Math.hpp>

namespace en::DataFileSpecialization
{

//////////////////////////////////////////////////////////////////
// Color
//////////////////////////////////////////////////////////////////
bool Serialize(DataFile& dataFile, const Color& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Color>();
		parser.SetValue(ToString(object.ToRGBA()));
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Color& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Color>::GetHash());
		std::string valueString;
		parser.GetValue(valueString);
		object.FromRGBA(FromString<U32>(valueString));
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Matrix3f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Matrix3f>();
		for (U32 i = 0; i < Matrix3f::Elements; ++i)
		{
			std::string childName(std::to_string(i));
			dataFile.Serialize(object[i], childName.c_str());
		}
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Matrix3f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Matrix3f>::GetHash());
		for (U32 i = 0; i < Matrix3f::Elements; ++i)
		{
			std::string childName(std::to_string(i));
			dataFile.Deserialize(object[i], childName.c_str());
		}
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Matrix4f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Matrix4f>();
		for (U32 i = 0; i < Matrix4f::Elements; ++i)
		{
			std::string childName(std::to_string(i));
			dataFile.Serialize(object[i], childName.c_str());
		}
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Matrix4f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Matrix4f>::GetHash());
		for (U32 i = 0; i < Matrix4f::Elements; ++i)
		{
			std::string childName(std::to_string(i));
			dataFile.Deserialize(object[i], childName.c_str());
		}
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Quaternionf& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Quaternionf>();
		dataFile.Serialize(object.v.x, "x");
		dataFile.Serialize(object.v.y, "y");
		dataFile.Serialize(object.v.z, "z");
		dataFile.Serialize(object.s, "s");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Quaternionf& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Quaternionf>::GetHash());
		dataFile.Deserialize(object.v.x, "x");
		dataFile.Deserialize(object.v.y, "y");
		dataFile.Deserialize(object.v.z, "z");
		dataFile.Deserialize(object.s, "s");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////
// Rect
//////////////////////////////////////////////////////////////////
bool Serialize(DataFile& dataFile, const Recti& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Recti>();
		dataFile.Serialize(object.GetMin(), "min");
		dataFile.Serialize(object.GetMax(), "max");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Recti& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Recti>::GetHash());
		Vector2i min;
		dataFile.Deserialize(min, "min");
		object.SetMin(min);
		Vector2i max;
		dataFile.Deserialize(max, "max");
		object.SetMax(max);
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Rectf& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Rectf>();
		dataFile.Serialize(object.GetMin(), "min");
		dataFile.Serialize(object.GetMax(), "max");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Rectf& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Rectf>::GetHash());
		Vector2f min;
		dataFile.Deserialize(min, "min");
		object.SetMin(min);
		Vector2f max;
		dataFile.Deserialize(max, "max");
		object.SetMax(max);
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Rectu& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Rectu>();
		dataFile.Serialize(object.GetMin(), "min");
		dataFile.Serialize(object.GetMax(), "max");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, Rectu& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Rectu>::GetHash());
		Vector2u min;
		dataFile.Deserialize(min, "min");
		object.SetMin(min);
		Vector2u max;
		dataFile.Deserialize(max, "max");
		object.SetMax(max);
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////
// Vector2
//////////////////////////////////////////////////////////////////
bool Serialize(DataFile& dataFile, const Vector2i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector2i>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector2i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector2i>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector2f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector2f>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector2f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector2f>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector2u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector2u>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector2u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector2u>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////
// Vector3
//////////////////////////////////////////////////////////////////
bool Serialize(DataFile& dataFile, const Vector3i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector3i>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector3i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector3i>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector3f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector3f>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector3f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector3f>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector3u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector3u>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector3u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector3u>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////
// Vector4
//////////////////////////////////////////////////////////////////
bool Serialize(DataFile& dataFile, const Vector4i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector4i>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		dataFile.Serialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector4i& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector4i>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		dataFile.Deserialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector4f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector4f>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		dataFile.Serialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector4f& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector4f>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		dataFile.Deserialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const Vector4u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Vector4u>();
		dataFile.Serialize(object.x, "x");
		dataFile.Serialize(object.y, "y");
		dataFile.Serialize(object.z, "z");
		dataFile.Serialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}
bool Deserialize(DataFile& dataFile, Vector4u& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Vector4u>::GetHash());
		dataFile.Deserialize(object.x, "x");
		dataFile.Deserialize(object.y, "y");
		dataFile.Deserialize(object.z, "z");
		dataFile.Deserialize(object.w, "w");
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

} // namespace en::DataFileSpecialization