#include <Enlivengine/Meta/ObjectEditorSpecialization_Platform.hpp>

#include <Enlivengine/Graphics/ImGuiHelper.hpp>

namespace en::ObjectEditorSpecialization
{

#ifdef ENLIVE_ENABLE_IMGUI
bool ImGuiEditor(Time& object, const char* name)
{
	static constexpr std::size_t maxSize = 256;
	static char concatName[maxSize];
#ifdef ENLIVE_COMPILER_MSVC
	strcpy_s(concatName, name);
	strcat_s(concatName, " (s)");
#else
	strcpy(concatName, name);
	strcat(concatName, " (s)");
#endif // ENLIVE_COMPILER_MSVC
	float valueFloat = static_cast<float>(object.AsSeconds());
	if (ImGui::InputFloat(concatName, &valueFloat))
	{
		object = Time::Seconds(static_cast<F32>(valueFloat));
		return true;
	}
	return false;
}
#endif // ENLIVE_ENABLE_IMGUI

} // namespace en::ObjectEditorSpecialization
