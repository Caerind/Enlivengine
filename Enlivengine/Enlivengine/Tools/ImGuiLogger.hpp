#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_LOG)

#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Utils/Log.hpp>

namespace en
{

class ImGuiLogger : public ImGuiTool, public Logger
{
	ENLIVE_SINGLETON(ImGuiLogger);
	~ImGuiLogger();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	virtual void Write(const LogMessage& message);

	static constexpr U32 kDefaultMaxSize{ 128 };
	void SetMaxSize(U32 size);
	U32 GetMaxSize() const;
	U32 GetCurrentSize() const;

private:
	std::vector<LogMessage> mMessages;
	U32 mMaxSize;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_LOG
#endif // ENLIVE_MODULE_TOOLS
