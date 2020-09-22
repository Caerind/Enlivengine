#include <Enlivengine/Tools/ImGuiMemoryDebugger.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_DEBUG_MEMORY)

#include <imgui/imgui.h>

#include <Enlivengine/System/MemoryAllocator.hpp>

namespace en
{

ImGuiMemoryDebugger::ImGuiMemoryDebugger()
	: ImGuiTool()
{
}

ImGuiMemoryDebugger::~ImGuiMemoryDebugger()
{
}

ImGuiToolTab ImGuiMemoryDebugger::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiMemoryDebugger::GetName() const
{
	return ICON_FA_MEMORY " MemoryDebugger";
}

void ImGuiMemoryDebugger::Display()
{
	const MemoryAllocator::DebugAllocator& debugAllocator = MemoryAllocator::GetDebugAllocator();
	const U32 totalAlloc = debugAllocator.GetAllocationCount();
	const U32 totalDealloc = debugAllocator.GetDeallocationCount();
	const U32 currentAlloc = totalAlloc - totalDealloc;
	enAssert(currentAlloc == debugAllocator.GetBlockCount());
	ImGui::Text("Total allocations : %d", totalAlloc);
	ImGui::Text("Total deallocations : %d", totalDealloc);
	ImGui::Text("Maximum used size : %d", debugAllocator.GetPeakSize());
	ImGui::Separator();
	ImGui::Text("Current allocations : %d", currentAlloc);
	ImGui::Text("Current used size : %d", debugAllocator.GetUsedSize());
	if (ImGui::CollapsingHeader("Current blocks"))
	{
		ImGui::Indent();
		const auto& blocks = debugAllocator.GetBlocks();
		for (const auto& block : blocks)
		{
			ImGui::Text("%s : %d from %s:%d", block.context, block.size, block.GetFile().c_str(), block.line);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%p", block.ptr);
			}
		}
		ImGui::Unindent();
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_DEBUG_MEMORY