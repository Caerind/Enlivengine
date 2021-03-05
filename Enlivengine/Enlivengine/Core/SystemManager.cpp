#include <Enlivengine/Core/SystemManager.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Core/SystemFactory.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

SystemManager::SystemManager(World& world)
	: mWorld(world)
	, mSystems()
	, mPhysicSystem(nullptr)
	, mRenderSystem(nullptr)
{
}

SystemManager::~SystemManager()
{
	for (System* system : mSystems)
	{
		enDelete(System, system);
	}
}

bool SystemManager::HasPhysicSystem() const
{
	return mPhysicSystem != nullptr;
}

PhysicSystemBase* SystemManager::GetPhysicSystem()
{
	return mPhysicSystem;
}

const PhysicSystemBase* SystemManager::GetPhysicSystem() const
{
	return mPhysicSystem;
}

bool SystemManager::HasRenderSystem() const
{
	return mRenderSystem != nullptr;
}

RenderSystemBase* SystemManager::GetRenderSystem()
{
	return mRenderSystem;
}

const RenderSystemBase* SystemManager::GetRenderSystem() const
{
	return mRenderSystem;
}

void SystemManager::UpdatePhysic()
{
	ENLIVE_PROFILE_FUNCTION();
	if (mPhysicSystem != nullptr)
	{
		mPhysicSystem->UpdatePhysic();
	}
}

void SystemManager::Update()
{
	ENLIVE_PROFILE_FUNCTION();
	for (System* system : mSystems)
	{
		ENLIVE_PROFILE_SCOPE(system->GetName());
		system->Update();
	}
}
  
void SystemManager::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	if (mRenderSystem != nullptr)
	{
		mRenderSystem->Render();
	}
}

#ifdef ENLIVE_TOOL
void SystemManager::UpdateTool()
{
	ENLIVE_PROFILE_FUNCTION();
	for (System* system : mSystems)
	{
		if (system->IsUpdateOnTool())
		{
			ENLIVE_PROFILE_SCOPE(system->GetName());
			system->Update();
		}
	}
}
#endif // ENLIVE_TOOL

bool SystemManager::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<SystemManager>::GetName(), TypeInfo<SystemManager>::GetHash()))
	{
		bool ret = true;

		if (serializer.IsReading())
		{
			const auto& systemInfos = SystemFactory::GetSystemInfos();
			const auto endItr = systemInfos.cend();
			for (auto itr = systemInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& si = itr->second;
				if (serializer.HasNode(si.name))
				{
					si.add(mWorld);
					ret = si.serialize(serializer, mWorld) && ret;
				}
			}
		}
		else if (serializer.IsWriting())
		{
			const auto& systemInfos = SystemFactory::GetSystemInfos();
			const auto endItr = systemInfos.cend();
			for (auto itr = systemInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& si = itr->second;
				if (si.has(mWorld))
				{
					ret = si.serialize(serializer, mWorld) && ret;
				}
			}
		}
		else
		{
			enAssert(false);
			ret = false;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool SystemManager::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<SystemManager>::GetName(), TypeInfo<SystemManager>::GetHash()))
	{
		bool ret = false;

		if (objectEditor.IsImGuiEditor())
		{
#ifdef ENLIVE_ENABLE_IMGUI
			std::vector<U32> hasNot;

			const auto& systemInfos = SystemFactory::GetSystemInfos();
			const auto endItr = systemInfos.cend();
			for (auto itr = systemInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& si = itr->second;
				if (si.has(mWorld))
				{
					ImGui::PushID(itr->first);
					const bool buttonPressed = ImGui::Button("X");
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("Remove");
					}
					if (buttonPressed)
					{
						si.remove(mWorld);
						ret = true;
					}
					else
					{
						ImGui::SameLine();
						if (si.editor(objectEditor, mWorld))
						{
							ret = true;
						}
					}
					ImGui::PopID();
				}
				else
				{
					hasNot.push_back(itr->first);
				}
			}

			if (!hasNot.empty())
			{
				if (ImGui::Button("+ Add System"))
				{
					ImGui::OpenPopup("Add System");
				}
				if (ImGui::BeginPopup("Add System"))
				{
					ImGui::TextUnformatted("Available:");
					ImGui::Separator();
					for (auto systemHash : hasNot)
					{
						const auto& si = systemInfos.at(systemHash);
						ImGui::PushID(systemHash);
						if (ImGui::Selectable(si.name))
						{
							si.add(mWorld);
							ret = true;
						}
						ImGui::PopID();
					}
					ImGui::EndPopup();
				}
			}
#endif // ENLIVE_ENABLE_IMGUI
		}

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

} // namespace en