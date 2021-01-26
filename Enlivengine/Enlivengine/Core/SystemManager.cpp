#include <Enlivengine/Core/SystemManager.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Core/SystemFactory.hpp>

namespace en
{

SystemManager::SystemManager(World& world)
	: mWorld(world)
	, mSystems()
	, mPhysicSystems()
	, mRenderSystems()
{
}

SystemManager::~SystemManager()
{
	for (System* system : mSystems)
	{
		enDelete(System, system);
	}
}

void SystemManager::UpdatePhysic(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	for (PhysicSystemBase* system : mPhysicSystems)
	{
		ENLIVE_PROFILE_SCOPE(system->GetName());
		system->UpdatePhysic(dt);
	}
}

void SystemManager::Update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	for (System* system : mSystems)
	{
		ENLIVE_PROFILE_SCOPE(system->GetName());
		system->Update(dt);
	}
}

void SystemManager::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	for (RenderSystemBase* system : mRenderSystems)
	{
		ENLIVE_PROFILE_SCOPE(system->GetName());
		system->Render();
	}
}

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

		std::vector<U32> hasNot;

		const auto& systemInfos = SystemFactory::GetSystemInfos();
		const auto endItr = systemInfos.cend();
		for (auto itr = systemInfos.cbegin(); itr != endItr; ++itr)
		{
			const auto& si = itr->second;
			if (si.has(mWorld))
			{
				ImGui::PushID(itr->first);
				if (ImGui::Button("-"))
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

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

} // namespace en