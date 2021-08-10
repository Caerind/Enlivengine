#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class Entity;
class World;

class ImGuiEntityBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEntityBrowser);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;

private:
	void GetEntityName(const Entity& entity, std::string& name);
	void DisplayEntity(Entity& entity, World* world);
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
