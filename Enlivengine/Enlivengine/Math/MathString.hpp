#pragma once

#include <Enlivengine/System/String.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

namespace en
{

template <> inline std::string ToString<Vector2f>(const Vector2f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y;
	return oss.str();
}

template <> inline std::string ToString<Vector3f>(const Vector3f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y << "," << value.z;
	return oss.str();
}

template <> inline std::string ToString<Vector4f>(const Vector4f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y << "," << value.z << "," << value.w;
	return oss.str();
}

template <> inline Vector2f FromString<Vector2f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector2f vector;
	Split(str, token, ',');
	vector.x = FromString<F32>(token);
	vector.y = FromString<F32>(str);
	return vector;
}

template <> inline Vector3f FromString<Vector3f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector3f vector;
	Split(str, token, ',');
	vector.x = FromString<F32>(token);
	Split(str, token, ',');
	vector.y = FromString<F32>(token);
	vector.z = FromString<F32>(str);
	return vector;
}

template <> inline Vector4f FromString<Vector4f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector4f vector;
	Split(str, token, ',');
	vector.x = FromString<F32>(token);
	Split(str, token, ',');
	vector.y = FromString<F32>(token);
	Split(str, token, ',');
	vector.z = FromString<F32>(token);
	vector.w = FromString<F32>(str);
	return vector;
}

} // namespace en