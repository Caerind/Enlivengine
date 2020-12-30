#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Quaternion.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

namespace en::DataFileSpecialization
{
	// Color
	bool Serialize(DataFile& dataFile, const Color& object, const char* name);
	bool Deserialize(DataFile& dataFile, Color& object, const char* name);

	// Matrix3f
	bool Serialize(DataFile& dataFile, const Matrix3f& object, const char* name);
	bool Deserialize(DataFile& dataFile, Matrix3f& object, const char* name);

	// Matrix4f
	bool Serialize(DataFile& dataFile, const Matrix4f& object, const char* name);
	bool Deserialize(DataFile& dataFile, Matrix4f& object, const char* name);

	// Quaternionf
	bool Serialize(DataFile& dataFile, const Quaternionf& object, const char* name);
	bool Deserialize(DataFile& dataFile, Quaternionf& object, const char* name);

	// Recti
	bool Serialize(DataFile& dataFile, const Recti& object, const char* name);
	bool Deserialize(DataFile& dataFile, Recti& object, const char* name);

	// Rectf
	bool Serialize(DataFile& dataFile, const Rectf& object, const char* name);
	bool Deserialize(DataFile& dataFile, Rectf& object, const char* name);

	// Rectu
	bool Serialize(DataFile& dataFile, const Rectu& object, const char* name);
	bool Deserialize(DataFile& dataFile, Rectu& object, const char* name);

	// Vector2i
	bool Serialize(DataFile& dataFile, const Vector2i& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector2i& object, const char* name);

	// Vector2f
	bool Serialize(DataFile& dataFile, const Vector2f& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector2f& object, const char* name);

	// Vector2u
	bool Serialize(DataFile& dataFile, const Vector2u& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector2u& object, const char* name);

	// Vector3i
	bool Serialize(DataFile& dataFile, const Vector3i& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector3i& object, const char* name);

	// Vector3f
	bool Serialize(DataFile& dataFile, const Vector3f& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector3f& object, const char* name);

	// Vector3u
	bool Serialize(DataFile& dataFile, const Vector2u& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector2u& object, const char* name);

	// Vector4i
	bool Serialize(DataFile& dataFile, const Vector4i& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector4i& object, const char* name);

	// Vector4f
	bool Serialize(DataFile& dataFile, const Vector4f& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector4f& object, const char* name);

	// Vector4u
	bool Serialize(DataFile& dataFile, const Vector4u& object, const char* name);
	bool Deserialize(DataFile& dataFile, Vector4u& object, const char* name);

}

//////////////////////////////////////////////////////////////////
// Color
//////////////////////////////////////////////////////////////////
template <>
struct HasCustomSerialization<en::Color>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Color& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Color& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Matrix3
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Matrix3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Matrix3<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Matrix3<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Matrix4
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Matrix4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Matrix4<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Matrix4<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Quaternion
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Quaternion<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Quaternion<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Quaternion<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Rect
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Rect<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Rect<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Rect<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Vector2
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Vector2<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector2<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector2<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Vector3
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Vector3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector3<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector3<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};

//////////////////////////////////////////////////////////////////
// Vector4
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::Vector4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector4<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Serialize(dataFile, object, name);
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector4<T>& object, const char* name)
	{
		return en::DataFileSpecialization::Deserialize(dataFile, object, name);
	}
};
