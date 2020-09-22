#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MetaEnum.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/Array.hpp>

#include <array>
#include <vector>

#include <Enlivengine/Tools/ImGuiHelper.hpp>

#include <Enlivengine/Core/CustomTraits.hpp>

namespace en
{

class ObjectEditor
{
public:
	template <typename T>
	static bool ImGuiEditorWindow(const char* windowName, T& object, const char* name)
	{
		enAssert(windowName != nullptr);
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::Begin(windowName))
		{
			modified = ImGuiEditor_Common(object, name);
			ImGui::End();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditorWindow(const char* windowName, T* object, const char* name)
	{
		enAssert(windowName != nullptr);
		enAssert(object != nullptr);
		enAssert(name != nullptr);
		return ImGuiEditorWindow(windowName, *object, name);
	}

	template <typename T>
	static bool ImGuiEditor(T& object, const char* name)
	{
		enAssert(name != nullptr);
		return ImGuiEditor_Common(object, name);
	}

	template <typename T>
	static bool ImGuiEditor(T* object, const char* name)
	{
		enAssert(name != nullptr);
		enAssert(object != nullptr);
		return ImGuiEditor_Common(*object, name);
	}

	template <typename T>
	static bool ImGuiEditor_Registered(T& object, const char* name)
	{
		static_assert(Meta::IsRegistered<T>());
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			Meta::ForEachMember<T>([&object, &modified](const auto& member)
			{
				if (member.HasEditor())
				{
					ImGui::PushID(member.GetHash());
					if (member.CanGetRef())
					{
						modified = ImGuiEditor_Common(member.GetRef(object), member.GetName()) || modified;
					}
					else if (member.CanGetCopy())
					{
						using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
						if constexpr (Traits::IsCopyAssignable<MemberType>::value)
						{
							MemberType memberCopy = member.GetCopy(object);
							if (ImGuiEditor_Common(memberCopy, member.GetName()))
							{
								modified = true;
								if (member.CanSet())
								{
									member.Set(object, memberCopy);
								}
							}
						}
						else
						{
							enAssert(false);
						}
					}
					else
					{
						enAssert(false);
					}
					ImGui::PopID();
				}
			});
			ImGui::Unindent();
		}
		return modified;
	}

private:
	template <typename T>
	static bool ImGuiEditor_Common(T& object, const char* name)
	{
		if constexpr (CustomObjectEditor<T>::value)
		{
			return CustomObjectEditor<T>::ImGuiEditor(object, name);
		}
		else if constexpr (Meta::IsRegistered<T>())
		{
			return ImGuiEditor_Registered(object, name);
		}
		else
		{
			return ImGuiEditor_Basic(object, name);
		}
	}

	template <typename T>
	static bool ImGuiEditor_Basic(T& object, const char* name)
	{
		enAssert(name != nullptr);
		
		using Type = typename Traits::Decay<T>::type;

		if constexpr (Traits::IsSame<Type, bool>::value)
		{
			return ImGui::Checkbox(name, &object);
		}
		else if constexpr (Traits::IsSame<Type, char>::value)
		{
			static char c;
			c = object;
			if (ImGui::InputText(name, &c, 2))
			{
				object = c;
				return true;
			}
			return false;
		}
		else if constexpr (Traits::IsEnum<T>::value)
		{
			static bool initializedEnum = false;
			static constexpr U32 enumCount = Meta::GetEnumCount<T>();
			static std::string stringsStorage[enumCount];
			static const char* stringsImGui[enumCount];
			if (!initializedEnum)
			{
				auto enumNames = Meta::GetEnumNames<T>();
				for (U32 i = 0; i < enumCount; ++i)
				{
					stringsStorage[i] = enumNames[i];
					stringsImGui[i] = stringsStorage[i].c_str();
				}
				initializedEnum = true;
			}
			int index = static_cast<int>(Meta::GetEnumIndex(object));
			if (ImGui::Combo(name, &index, stringsImGui, IM_ARRAYSIZE(stringsImGui)))
			{
				object = Meta::GetEnumFromIndex<T>(static_cast<U32>(index));
				return true;
			}
			return false;
		}
		else if constexpr (Traits::IsIntegral<T>::value)
		{
			int value = static_cast<int>(object);
			if (ImGui::InputInt(name, &value))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (Traits::IsUnsigned<T>::value)
				{
					object = static_cast<T>((value >= 0) ? value : 0);
				}
				else
				{
					object = static_cast<T>(value);
				}
				return true;
			}
			return false;
		}
		else if constexpr (Traits::IsFloatingPoint<T>::value)
		{
			float value = static_cast<float>(object);
			if (ImGui::InputFloat(name, &value))
			{
				object = static_cast<T>(value);
				return true;
			}
			return false;
		}
		else if constexpr (Traits::IsSame<Type, std::string>::value)
		{
			static constexpr std::size_t maxSize = 512;
			static char inputBuffer[maxSize];
			if (strcmp(inputBuffer, object.c_str()) != 0)
			{
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(inputBuffer, object.c_str());
#else
				strcpy(inputBuffer, object.c_str());
#endif // ENLIVE_COMPILER_MSVC
			}
			if (ImGui::InputText(name, inputBuffer, maxSize))
			{
				object = inputBuffer;
				return true;
			}
			return false;
		}
		else
		{
			ImGui::Text("%s type is not implemented for imgui for %s", TypeInfo<T>::GetName(), name);
			return false;
		}
	}

	template <typename T>
	static bool ImGuiEditor_Basic(Array<T>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (U32 i = 0; i < object.Size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				ImGui::PushID(i);
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(std::vector<T>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				ImGui::PushID(i);
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T, std::size_t N>
	static bool ImGuiEditor_Basic(std::array<T, N>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				ImGui::PushID(i);
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(Array<T*>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (U32 i = 0; i < object.Size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					ImGui::PushID(i);
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
					ImGui::PopID();
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(std::vector<T*>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					ImGui::PushID(i);
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
					ImGui::PopID();
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T, std::size_t N>
	static bool ImGuiEditor_Basic(std::array<T*, N>& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					ImGui::PushID(i);
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
					ImGui::PopID();
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}
};

} // namespace en

#include "ObjectEditorSpecialization.inl"
