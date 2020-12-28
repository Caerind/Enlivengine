#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Meta/MetaSpecialization_Utils.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Quaternion.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Sphere.hpp>
#include <Enlivengine/Math/Transform.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

//////////////////////////////////////////////////////////////////
// en::AABB
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::AABB)
	ENLIVE_META_CLASS_MEMBER("min", &en::AABB::GetMin, &en::AABB::SetMin),
	ENLIVE_META_CLASS_MEMBER("max", &en::AABB::GetMax, &en::AABB::SetMax)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Color
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Color)
	ENLIVE_META_CLASS_MEMBER("r", &en::Color::r),
	ENLIVE_META_CLASS_MEMBER("g", &en::Color::g),
	ENLIVE_META_CLASS_MEMBER("b", &en::Color::b),
	ENLIVE_META_CLASS_MEMBER("a", &en::Color::a)
ENLIVE_META_CLASS_END()

template <>
struct HasCustomSerialization<en::Color>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Color>();
			parser.SetValue(en::ToString(object.ToRGBA()));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Color>::GetHash());
			std::string valueString;
			parser.GetValue(valueString);
			object.FromRGBA(en::FromString<en::U32>(valueString));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::Color>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Color& object, const char* name)
	{
		ImVec4 color = object.ToImGuiColor();
		if (ImGui::ColorEdit4(name, (float*)&color, ImGuiColorEditFlags_AlphaPreview))
		{
			object.FromImGuiColor(color);
			return true;
		}
		return false;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Matrix3
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix3)

template <typename T>
struct HasCustomSerialization<en::Matrix3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Matrix3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Matrix3<T>>();
			for (std::size_t i = 0; i < en::Matrix3<T>::Elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Serialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Matrix3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Matrix3<T>>::GetHash());
			for (std::size_t i = 0; i < en::Matrix3<T>::Elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Deserialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Matrix3<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Matrix3<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (en::U32 row = 0; row < en::Matrix3<T>::Rows; ++row)
			{
				en::Vector3<T> matrixRow;
				matrixRow = object.GetRow(row);
				ImGui::PushID(row);
				if (HasCustomEditor<en::Vector3<T>>::ImGuiEditor(matrixRow, ""))
				{
					object.SetRow(row, matrixRow);
					modified = true;
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Matrix4
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix4)

template <typename T>
struct HasCustomSerialization<en::Matrix4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Matrix4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Matrix4<T>>();
			for (std::size_t i = 0; i < en::Matrix4<T>::Elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Serialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Matrix4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Matrix4<T>>::GetHash());
			for (std::size_t i = 0; i < en::Matrix4<T>::Elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Deserialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Matrix4<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Matrix4<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (en::U32 row = 0; row < en::Matrix4<T>::Rows; ++row)
			{
				en::Vector4<T> matrixRow;
				matrixRow = object.GetRow(row);
				if (HasCustomEditor<en::Vector4<T>>::ImGuiEditor(matrixRow, ""))
				{
					object.SetRow(row, matrixRow);
					modified = true;
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Plane
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Plane)
	ENLIVE_META_CLASS_MEMBER("normal", &en::Plane::GetNormal, &en::Plane::SetNormal),
	ENLIVE_META_CLASS_MEMBER("constant", &en::Plane::GetConstant, &en::Plane::SetConstant)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Quaternion
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Quaternion)

template <typename T>
struct HasCustomSerialization<en::Quaternion<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Quaternion<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Quaternion<T>>();
			dataFile.Serialize_Common(object.v.x, "x");
			dataFile.Serialize_Common(object.v.y, "y");
			dataFile.Serialize_Common(object.v.z, "z");
			dataFile.Serialize_Common(object.s, "s");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Quaternion<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Quaternion<T>>::GetHash());
			dataFile.Deserialize_Common(object.v.x, "x");
			dataFile.Deserialize_Common(object.v.y, "y");
			dataFile.Deserialize_Common(object.v.z, "z");
			dataFile.Deserialize_Common(object.s, "s");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Quaternion<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Quaternion<T>& object, const char* name)
	{
		en::Vector4<T> v4(object.v, object.s);
		if (HasCustomEditor<en::Vector4<T>>::ImGuiEditor(v4, name))
		{
			object.v.Set(v4.x, v4.y, v4.z);
			object.s = v4.w;
			return true;
		}
		return false;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Ray
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Ray)
	ENLIVE_META_CLASS_MEMBER("origin", &en::Ray::GetOrigin, &en::Ray::SetOrigin),
	ENLIVE_META_CLASS_MEMBER("direction", &en::Ray::GetDirection, &en::Ray::SetDirection)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Rect
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Rect)

template <typename T>
struct HasCustomSerialization<en::Rect<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Rect<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Rect<T>>();
			dataFile.Serialize_Common(object.GetMin(), "min");
			dataFile.Serialize_Common(object.GetMax(), "max");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Rect<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Rect<T>>::GetHash());
			en::Vector2<T> min;
			dataFile.Deserialize_Common(min, "min");
			object.SetMin(min);
			en::Vector2<T> max;
			dataFile.Deserialize_Common(max, "max");
			object.SetMax(max);
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Rect<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Rect<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			en::Vector2<T> min = object.GetMin();
			if (HasCustomEditor<en::Vector2<T>>::ImGuiEditor(min, "min"))
			{
				object.SetMin(min);
				modified = true;
			}
			en::Vector2<T> size = object.GetSize();
			if (HasCustomEditor<en::Vector2<T>>::ImGuiEditor(size, "size"))
			{
				object.SetSize(size);
				modified = true;
			}
			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Sphere
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Sphere)
	ENLIVE_META_CLASS_MEMBER("center", &en::Sphere::GetCenter, &en::Sphere::SetCenter),
	ENLIVE_META_CLASS_MEMBER("radius", &en::Sphere::GetRadius, &en::Sphere::SetRadius)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Transform
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Transform)
	ENLIVE_META_CLASS_MEMBER("position", &en::Transform::GetPosition, &en::Transform::SetPosition),
	ENLIVE_META_CLASS_MEMBER("rotation", &en::Transform::GetRotation, &en::Transform::SetRotation),
	ENLIVE_META_CLASS_MEMBER("scale", &en::Transform::GetScale, &en::Transform::SetScale)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::Transform>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Transform& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			en::Vector3f position = object.GetPosition();
			en::Matrix3f mat = object.GetRotation();
			en::Vector3f rotation;
			rotation.x = en::Math::Atan2(mat(2, 2), mat(1, 2));
			rotation.y = en::Math::Atan2(en::Math::FastSqrt(mat(1, 2) * mat(1, 2) + mat(2, 2) * mat(2, 2)), -mat(0, 2));
			rotation.z = en::Math::Atan2(mat(0, 0), mat(0, 1));
			en::Vector3f scale = object.GetScale();

			if (en::ObjectEditor::ImGuiEditor(position, "position"))
			{
				object.SetPosition(position);
				modified = true;
			}

			if (en::ObjectEditor::ImGuiEditor(rotation, "rotation"))
			{
				en::Matrix3f rot[3];
				rot[0] = en::Matrix3f::RotationX(rotation[0]);
				rot[1] = en::Matrix3f::RotationY(rotation[1]);
				rot[2] = en::Matrix3f::RotationZ(rotation[2]);
				mat = rot[0] * rot[1] * rot[2];
				object.SetRotation(mat);
				modified = true;
			}

			if (en::ObjectEditor::ImGuiEditor(scale, "scale"))
			{
				object.SetScale(scale);
				modified = true;
			}

			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Vector2
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector2)

template <typename T>
struct HasCustomSerialization<en::Vector2<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector2<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector2<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector2<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector2<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Vector2<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector2<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			int vec2i[2];
			vec2i[0] = object.x;
			vec2i[1] = object.y;
			if (ImGui::InputInt2(name, vec2i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec2i[0] >= 0) ? vec2i[0] : 0);
					object.y = static_cast<T>((vec2i[1] >= 0) ? vec2i[1] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec2i[0]);
					object.y = static_cast<T>(vec2i[1]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			float vec2f[2];
			vec2f[0] = object.x;
			vec2f[1] = object.y;
			if (ImGui::InputFloat2(name, vec2f))
			{
				object.x = static_cast<T>(vec2f[0]);
				object.y = static_cast<T>(vec2f[1]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector2", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Vector3
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector3)

template <typename T>
struct HasCustomSerialization<en::Vector3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector3<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			dataFile.Serialize_Basic(object.z, "z");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector3<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			dataFile.Deserialize_Basic(object.z, "z");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Vector3<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector3<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			int vec3i[3];
			vec3i[0] = object.x;
			vec3i[1] = object.y;
			vec3i[2] = object.z;
			if (ImGui::InputInt3(name, vec3i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec3i[0] >= 0) ? vec3i[0] : 0);
					object.y = static_cast<T>((vec3i[1] >= 0) ? vec3i[1] : 0);
					object.z = static_cast<T>((vec3i[2] >= 0) ? vec3i[2] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec3i[0]);
					object.y = static_cast<T>(vec3i[1]);
					object.z = static_cast<T>(vec3i[2]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			float vec3f[3];
			vec3f[0] = object.x;
			vec3f[1] = object.y;
			vec3f[2] = object.z;
			if (ImGui::InputFloat3(name, vec3f))
			{
				object.x = static_cast<T>(vec3f[0]);
				object.y = static_cast<T>(vec3f[1]);
				object.z = static_cast<T>(vec3f[2]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector3", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Vector4
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector4)

template <typename T>
struct HasCustomSerialization<en::Vector4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Vector4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector4<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			dataFile.Serialize_Basic(object.z, "z");
			dataFile.Serialize_Basic(object.w, "w");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Vector4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector4<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			dataFile.Deserialize_Basic(object.z, "z");
			dataFile.Deserialize_Basic(object.w, "w");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::Vector4<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector4<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			int vec4i[4];
			vec4i[0] = object.x;
			vec4i[1] = object.y;
			vec4i[2] = object.z;
			vec4i[3] = object.w;
			if (ImGui::InputInt4(name, vec4i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec4i[0] >= 0) ? vec4i[0] : 0);
					object.y = static_cast<T>((vec4i[1] >= 0) ? vec4i[1] : 0);
					object.z = static_cast<T>((vec4i[2] >= 0) ? vec4i[2] : 0);
					object.w = static_cast<T>((vec4i[3] >= 0) ? vec4i[3] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec4i[0]);
					object.y = static_cast<T>(vec4i[1]);
					object.z = static_cast<T>(vec4i[2]);
					object.w = static_cast<T>(vec4i[3]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			float vec4f[4];
			vec4f[0] = object.x;
			vec4f[1] = object.y;
			vec4f[2] = object.z;
			vec4f[3] = object.w;
			if (ImGui::InputFloat4(name, vec4f))
			{
				object.x = static_cast<T>(vec4f[0]);
				object.y = static_cast<T>(vec4f[1]);
				object.z = static_cast<T>(vec4f[2]);
				object.w = static_cast<T>(vec4f[3]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector4", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};
#endif // ENLIVE_ENABLE_IMGUI