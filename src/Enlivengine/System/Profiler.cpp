#include <Enlivengine/System/Profiler.hpp>

#ifdef ENLIVE_ENABLE_PROFILE

#include <Enlivengine/System/Assert.hpp>

#include <imgui/imgui.h>

#include <string>

namespace en
{

bool Profiler::mEnabled = true;
U32 Profiler::mFrameNumber = 0;
ProfilerFrame Profiler::mProfilerFrame;
std::vector<ProfilerDisplay*> Profiler::mProfilerDisplays;

ProfilerFunctionCall::ProfilerFunctionCall(const char* name, U32 id, U32 parent)
	: mId(id)
	, mName(name)
	, mParent(parent)
{
}

void ProfilerFunctionCall::begin(U32 level)
{
	mLevel = level;
	mBegin = Time::now();
}

void ProfilerFunctionCall::end()
{
	mEnd = Time::now();
}

U32 ProfilerFunctionCall::getId() const
{
	return mId;
}

const char* ProfilerFunctionCall::getName() const
{
	return mName;
}

U32 ProfilerFunctionCall::getParent() const
{
	return mParent;
}

const Time& ProfilerFunctionCall::getBegin() const
{
	return mBegin;
}

const Time& ProfilerFunctionCall::getEnd() const
{
	return mEnd;
}

Time ProfilerFunctionCall::getDuration() const
{
	return mEnd - mBegin;
}

F32 ProfilerFunctionCall::getPercent(const Time& frameDuration) const
{
	return (100.f * getDuration().asMicroseconds()) / (1.0f * frameDuration.asMicroseconds());
}

U32 ProfilerFunctionCall::getLevel() const
{
	return mLevel;
}

void ProfilerFunctionCall::addChild(U32 childId)
{
	mChildren.push_back(childId);
}

const std::vector<U32>& ProfilerFunctionCall::getChilren() const
{
	return mChildren;
}

bool ProfilerFunctionCall::hasChildren() const
{
	return !mChildren.empty();
}

bool ProfilerFunctionCall::isChild(U32 id) const
{
	for (U32 i = 0; i < mChildren.size(); ++i)
	{
		if (mChildren[i] == id)
		{
			return true;
		}
	}
	return false;
}

ProfilerFrame::ProfilerFrame()
{
}

void ProfilerFrame::begin(U32 frameNumber)
{
	// Clear data
	mCalls.clear();
	mCurrent = nullptr;
	mLevel = 0;
	mIdCounter = 1;
	mFrameNumber = frameNumber;

	mBegin = Time::now();
}

void ProfilerFrame::end(bool importantFrame)
{
	assert(mCurrent == nullptr); // A function hasn't been closed
	mImportant = importantFrame;
	mEnd = Time::now();
}

void ProfilerFrame::beginFunction(const char* name)
{
	if (mCurrent == nullptr)
	{
		mCalls.emplace_back(name, mIdCounter++);
		mLevel = 0;
	}
	else
	{
		U32 id = mIdCounter++;
		mCurrent->addChild(id);
		mCalls.emplace_back(name, id, mCurrent->getId());
		mLevel++;
	}

	mCurrent = &mCalls.back();

	assert(mCurrent != nullptr);

	mCurrent->begin(mLevel);
}

void ProfilerFrame::endFunction()
{
	assert(mCurrent != nullptr);

	mCurrent->end();

	U32 parentId = mCurrent->getParent();
	if (parentId == 0)
	{
		mCurrent = nullptr;
		mLevel = 0;
	}
	else
	{
		for (U32 i = 0; i < mCalls.size(); ++i)
		{
			if (mCalls[i].getId() == parentId)
			{
				mCurrent = &mCalls[i];
				mLevel--;
				return;
			}
		}
	}
}

const Time& ProfilerFrame::getBegin() const
{
	return mBegin;
}

const Time& ProfilerFrame::getEnd() const
{
	return mEnd;
}

Time ProfilerFrame::getDuration() const
{
	return mEnd - mBegin;
}

const std::vector<ProfilerFunctionCall>& ProfilerFrame::getCalls() const
{
	return mCalls;
}

U32 ProfilerFrame::getFrameNumber() const
{
	return mFrameNumber;
}

bool ProfilerFrame::isImportant() const
{
	return mImportant;
}

ProfilerDisplay::ProfilerDisplay()
	: mConnected(false)
{
	connect();
}

ProfilerDisplay::~ProfilerDisplay()
{
	if (isConnected())
	{
		disconnect();
	}
}

void ProfilerDisplay::connect()
{
	if (mConnected)
	{
		return;
	}
	mConnected = true;
	Profiler::connectDisplay(this);
}

void ProfilerDisplay::disconnect()
{
	if (!mConnected)
	{
		return;
	}
	mConnected = false;
	Profiler::disconnectDisplay(this);
}

bool ProfilerDisplay::isConnected() const
{
	return mConnected;
}

void Profiler::beginFrame()
{
	mProfilerFrame.begin(mFrameNumber++);
}

void Profiler::endFrame(bool importantFrame)
{
	mProfilerFrame.end(importantFrame);
	for (U32 i = 0; i < mProfilerDisplays.size(); ++i)
	{
		mProfilerDisplays[i]->displayFrame(mProfilerFrame);
	}
}

void Profiler::beginFunction(const char* name)
{
	mProfilerFrame.beginFunction(name);
}

void Profiler::endFunction()
{
	mProfilerFrame.endFunction();
}

void Profiler::connectDisplay(ProfilerDisplay* display)
{
	if (display == nullptr)
	{
		return;
	}
	for (U32 i = 0; i < mProfilerDisplays.size(); ++i)
	{
		if (mProfilerDisplays[i] == display)
		{
			return;
		}
	}
	mProfilerDisplays.push_back(display);
}

void Profiler::disconnectDisplay(ProfilerDisplay* display)
{
	for (auto itr = mProfilerDisplays.begin(); itr != mProfilerDisplays.end(); ++itr)
	{
		if (display == *itr)
		{
			mProfilerDisplays.erase(itr);
			return;
		}
	}
}

Profile::Profile(const char* functionName)
{
	Profiler::beginFunction(functionName);
}

Profile::~Profile()
{
	Profiler::endFunction();
}

ConsoleProfiler::ConsoleProfiler()
	: ProfilerDisplay()
{
}

ConsoleProfiler::~ConsoleProfiler()
{
	if (isConnected())
	{
		disconnect();
	}
}

void ConsoleProfiler::displayFrame(const ProfilerFrame& frame)
{
	ENLIVE_PROFILE_FUNCTION();

    #ifdef _MSC_VER
        system("cls");
	#else
        printf("\e[1;1H\e[2J"); // Should work on ANSI
	#endif

	printf("Frame %u, duration : %I64dus\n", frame.getFrameNumber(), frame.getDuration().asMicroseconds());
	for (const auto& fc : frame.getCalls())
	{
		U32 level = fc.getLevel();
		for (U32 i = 0; i < level; ++i)
		{
			printf("  ");
		}
		printf("-%s, duration : %I64dus, percent : %f\n", fc.getName(), fc.getDuration().asMicroseconds(), fc.getPercent(frame.getDuration()));
	}
}


ImGuiProfiler::ImGuiProfiler()
	: ProfilerDisplay()
	, mPaused(false)
	, mImportant(true)
{
}

ImGuiProfiler::~ImGuiProfiler()
{
	disconnect();
}

void ImGuiProfiler::displayFrame(const ProfilerFrame& frame)
{
	if (!mPaused)
	{
		if (!mImportant || (mImportant && frame.isImportant()))
		{
			mFrame = frame;
		}
	}
}

void ImGuiProfiler::pause()
{
	mPaused = true;
}

void ImGuiProfiler::play()
{
	mPaused = false;
}

void ImGuiProfiler::draw()
{
	ENLIVE_PROFILE_FUNCTION();

	if (!ImGui::Begin("Profiler"))
	{
		ImGui::End();
		return;
	}

	if (!mPaused && ImGui::Button("Pause"))
	{
		mPaused = true;
	}
	else if (mPaused && ImGui::Button("Play"))
	{
		mPaused = false;
	}
	ImGui::SameLine();
	if (!mImportant && ImGui::Button("Important frames"))
	{
		mImportant = true;
	}
	else if (mImportant && ImGui::Button("All frames"))
	{
		mImportant = false;
	}

	ImGui::Text("Frame %d, duration : %d us", mFrame.getFrameNumber(), mFrame.getDuration().asMicroseconds());

	ImGui::BeginChild("Scrolling", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	const std::vector<ProfilerFunctionCall>& calls(mFrame.getCalls());
	for (U32 i = 0; i < calls.size(); i++)
	{
		if (calls[i].getLevel() == 0)
		{
			drawFunctionCall(calls[i]);
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::End();
}

void ImGuiProfiler::drawFunctionCall(const ProfilerFunctionCall& fc)
{
	const std::vector<ProfilerFunctionCall>& calls(mFrame.getCalls());

	const std::string str = std::string(fc.getName()) + ", duration : " + std::to_string(fc.getDuration().asMicroseconds()) + "us, percent : " + std::to_string(fc.getPercent(mFrame.getDuration()));

	if (ImGui::TreeNode(str.c_str()))
	{
		if (fc.hasChildren())
		{
			for (U32 i = 0; i < calls.size(); i++)
			{
				if (calls[i].getParent() == fc.getId())
				{
					drawFunctionCall(calls[i]);
				}
			}
		}
		ImGui::TreePop();
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_PROFILE
