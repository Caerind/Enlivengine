#pragma once

#include <Enlivengine/System/String.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

namespace en
{

template <> inline std::string toString<Vector2f>(const Vector2f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y;
	return oss.str();
}

template <> inline std::string toString<Vector3f>(const Vector3f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y << "," << value.z;
	return oss.str();
}

template <> inline std::string toString<Vector4f>(const Vector4f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y << "," << value.z << "," << value.w;
	return oss.str();
}

template <> inline Vector2f fromString<Vector2f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector2f vector;
	split(str, token, ',');
	vector.x = fromString<F32>(token);
	vector.y = fromString<F32>(str);
	return vector;
}

template <> inline Vector3f fromString<Vector3f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector3f vector;
	split(str, token, ',');
	vector.x = fromString<F32>(token);
	split(str, token, ',');
	vector.y = fromString<F32>(token);
	vector.z = fromString<F32>(str);
	return vector;
}

template <> inline Vector4f fromString<Vector4f>(const std::string& string)
{
	std::string str(string);
	std::string token;
	Vector4f vector;
	split(str, token, ',');
	vector.x = fromString<F32>(token);
	split(str, token, ',');
	vector.y = fromString<F32>(token);
	split(str, token, ',');
	vector.z = fromString<F32>(token);
	vector.w = fromString<F32>(str);
	return vector;
}

} // namespace en