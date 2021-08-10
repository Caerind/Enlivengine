#include <Enlivengine/Math/Color.hpp>

namespace en
{

bool Color::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.IsReading())
	{
		U32 rgba;
		const bool ret = serializer.Serialize(name, rgba);
		if (ret)
		{
			FromRGBA(rgba);
		}
		return ret;
	}
	else if (serializer.IsWriting())
	{
		U32 rgba = ToRGBA();
		return serializer.Serialize(name, rgba);
	}
	else
	{
		return false;
	}
}

bool Color::Edit(ObjectEditor& objectEditor, const char* name)
{
#ifdef ENLIVE_ENABLE_IMGUI
	if (objectEditor.IsImGuiEditor())
	{
		ImVec4 color = ToImGuiColor();
		if (ImGui::ColorEdit3(name, (float*)&color))
		{
			FromImGuiColor(color);
			return true;
		}
		return false;
	}
	else
#endif // ENLIVE_ENABLE_IMGUI
	{
		bool ret = false; 
		if (objectEditor.BeginClass(name, TypeInfo<Color>::GetName(), TypeInfo<Color>::GetHash()))
		{
			ret = GenericEdit(objectEditor, "R", r) || ret;
			ret = GenericEdit(objectEditor, "G", g) || ret;
			ret = GenericEdit(objectEditor, "B", b) || ret;
			ret = GenericEdit(objectEditor, "A", a) || ret;
			objectEditor.EndClass();
		}
		return ret;
	}
}

} // namespace en