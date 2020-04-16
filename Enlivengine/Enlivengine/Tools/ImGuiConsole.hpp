#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/System/Console.hpp>

namespace en
{

class ImGuiConsole : public ImGuiTool, public ConsoleInstance
{
	ENLIVE_SINGLETON(ImGuiConsole);
	~ImGuiConsole();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	virtual void AddLine(const std::string& line);

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
