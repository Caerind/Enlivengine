#pragma once

#include <Enlivengine/System/Config.hpp>

#ifdef ENLIVE_ENABLE_PROFILE

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/Time.hpp>

#include <vector>

namespace en
{

class ProfilerFunctionCall
{
	public:
		ProfilerFunctionCall(const char* name, U32 id, U32 parent = 0);

		void begin(U32 level);
		void end();

		U32 getId() const;
		const char* getName() const;
		U32 getParent() const;
		const Time& getBegin() const;
		const Time& getEnd() const;
		Time getDuration() const;
		F32 getPercent(const Time& frameDuration) const;
		U32 getLevel() const;

		void addChild(U32 childId);
		const std::vector<U32>& getChilren() const;
		bool hasChildren() const;
		bool isChild(U32 id) const;

	private:
		U32 mId;
	    const char* mName;
		U32 mParent;
		Time mBegin;
		Time mEnd;
		U32 mLevel;
		std::vector<U32> mChildren;
};

class ProfilerFrame
{
	public:
		ProfilerFrame();

		void begin(U32 frameNumber);
		void end(bool important);

		void beginFunction(const char* name);
		void endFunction();

		const Time& getBegin() const;
		const Time& getEnd() const;
		Time getDuration() const;
		const std::vector<ProfilerFunctionCall>& getCalls() const;
		U32 getFrameNumber() const;
		bool isImportant() const;

	private:
		Time mBegin;
		Time mEnd;
		U32 mLevel;
		U32 mFrameNumber;
		bool mImportant;

		U32 mIdCounter;

		ProfilerFunctionCall* mCurrent;
		std::vector<ProfilerFunctionCall> mCalls;
};

class ProfilerDisplay
{
	public:
		ProfilerDisplay();
		virtual ~ProfilerDisplay();

		void connect();
		void disconnect();
		bool isConnected() const;

		virtual void displayFrame(const ProfilerFrame& frame) = 0;

	private:
		bool mConnected;
};

class Profiler
{
	public:
		Profiler() = delete;
		~Profiler() = delete; 

		static void beginFrame();
		static void endFrame(bool keepFrame = true);

		static void enable() { mEnabled = true; }
		static void disable() { mEnabled = false; }
		static bool isEnabled() { return mEnabled; }

		static void beginFunction(const char* name);
		static void endFunction();

		static void connectDisplay(ProfilerDisplay* display);
		static void disconnectDisplay(ProfilerDisplay* display);

	private:
		static bool mEnabled;
		static U32 mFrameNumber;
		static ProfilerFrame mProfilerFrame;
		static std::vector<ProfilerDisplay*> mProfilerDisplays;
};

class Profile
{
	public:
		Profile(const char* functionName);
		~Profile();
};

#define ENLIVE_PROFILE_FUNCTION() en::Profile functionProfile(ENLIVE_FUNCTION);

// TODO : Separate to another thread because it slow the run quite a lot
class ConsoleProfiler : public ProfilerDisplay
{
public:
	ConsoleProfiler();
	virtual ~ConsoleProfiler();

	virtual void displayFrame(const ProfilerFrame& frame);
};

#ifdef ENLIVE_ENABLE_IMGUI
class ImGuiProfiler : public ProfilerDisplay
{
public:
	ImGuiProfiler();
	virtual ~ImGuiProfiler();

	virtual void displayFrame(const ProfilerFrame& frame);

	void pause();
	void play();

	void draw();

private:
	void drawFunctionCall(const ProfilerFunctionCall& fc);

private:
	ProfilerFrame mFrame;
	bool mPaused;
	bool mImportant;
};
#endif // ENLIVE_ENABLE_IMGUI

} // namespace en

#endif // ENLIVE_ENABLE_PROFILE