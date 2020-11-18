#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

// TODO : The old Console class from System didn't belong there + was too dirty
// Maybe something can be done directly here using C++17, tuple, ...
class ImGuiConsole : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiConsole);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;

	// Only display
	void AddLine(const std::string& line);

	static constexpr U32 kMaxInputBufSize{ 256 };
	static constexpr U32 kDefaultMaxSize{ 20 };
	void SetMaxSize(U32 size);
	U32 GetMaxSize() const;
	U32 GetCurrentSize() const;

private:
	char mInputBuffer[kMaxInputBufSize];
	std::vector<std::string> mLines;
	U32 mMaxSize;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
