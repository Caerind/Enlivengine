#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Utils/ObjectEditor.hpp>

namespace en
{

class ImGuiObjectEditor : public ObjectEditor
{
public:
	bool BeginClass(const char* name, const char* className, U32 classTypeHash) override;
	bool EndClass() override;

	bool IsImGuiEditor() const override;

	bool Edit(const char* name, bool& value) override;
	bool Edit(const char* name, char& value) override;
	bool Edit(const char* name, I8& value) override;
	bool Edit(const char* name, U8& value) override;
	bool Edit(const char* name, I16& value) override;
	bool Edit(const char* name, U16& value) override;
	bool Edit(const char* name, I32& value) override;
	bool Edit(const char* name, U32& value) override;
	bool Edit(const char* name, I64& value) override;
	bool Edit(const char* name, U64& value) override;
	bool Edit(const char* name, F32& value) override;
	bool Edit(const char* name, F64& value) override;
	bool Edit(const char* name, std::string& value) override;
};


} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
