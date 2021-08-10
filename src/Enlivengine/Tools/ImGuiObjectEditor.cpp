#include <Enlivengine/Tools/ImGuiObjectEditor.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

namespace en
{

bool ImGuiObjectEditor::BeginClass(const char* name, const char* className, U32 classTypeHash)
{
	ENLIVE_UNUSED(className);
	ENLIVE_UNUSED(classTypeHash);
	const bool open = ImGui::CollapsingHeader(name);
	if (open)
	{
		ImGui::Indent();
	}
	return open;
}

bool ImGuiObjectEditor::EndClass()
{
	ImGui::Unindent();
	return true;
}

bool ImGuiObjectEditor::IsImGuiEditor() const
{
	return true;
}

bool ImGuiObjectEditor::Edit(const char* name, bool& value)
{
	return ImGui::Checkbox(name, &value);
}

bool ImGuiObjectEditor::Edit(const char* name, char& value)
{
	char tmp = value;
	if (ImGui::InputText(name, &tmp, 2))
	{
		value = tmp;
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, I8& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<I8>(value);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, U8& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<U8>((tmp >= 0) ? tmp : 0);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, I16& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<I16>(value);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, U16& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<U16>((tmp >= 0) ? tmp : 0);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, I32& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<I32>(value);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, U32& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<U32>((tmp >= 0) ? tmp : 0);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, I64& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<I64>(value);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, U64& value)
{
	int tmp = static_cast<int>(value);
	if (ImGui::InputInt(name, &tmp))
	{
		// TODO : NumericLimits<T> Max
		value = static_cast<U64>((tmp >= 0) ? tmp : 0);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, F32& value)
{
	float tmp = static_cast<float>(value);
	if (ImGui::InputFloat(name, &tmp))
	{
		value = static_cast<F32>(tmp);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, F64& value)
{
	float tmp = static_cast<float>(value);
	if (ImGui::InputFloat(name, &tmp))
	{
		value = static_cast<F64>(tmp);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImGuiObjectEditor::Edit(const char* name, std::string& value)
{
	static constexpr std::size_t maxSize = 512;
	static char inputBuffer[maxSize];
	if (strcmp(inputBuffer, value.c_str()) != 0)
	{
#ifdef ENLIVE_COMPILER_MSVC
		strcpy_s(inputBuffer, value.c_str());
#else
		strcpy(inputBuffer, value.c_str());
#endif // ENLIVE_COMPILER_MSVC
	}
	if (ImGui::InputText(name, inputBuffer, maxSize))
	{
		value = inputBuffer;
		return true;
	}
	else
	{
		return false;
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI