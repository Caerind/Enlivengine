#pragma once

template <typename T>
struct CustomObjectEditor
{
	static constexpr bool value = false;
};

template <typename T>
struct CustomXmlSerialization
{
	// TODO : Maybe have a different value for serialization & deserialization
	static constexpr bool value = false;
};

template <typename T>
struct CustomComponentInitialization
{
	static constexpr bool value = false;
};