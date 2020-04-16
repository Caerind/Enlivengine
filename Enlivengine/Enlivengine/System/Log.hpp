#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_LOG

#include <cstdarg>
#include <fstream>
#include <vector>

#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/Singleton.hpp>

// TODO : Encapsulate info in class Message for 'post-filter' (now its filtered out on write())

namespace en
{

enum class LogType : U32
{
	None = 0,
	Info = 1 << 0,
	Warning = 1 << 1,
	Error = 1 << 2,
	All = (1 << 3) - 1
};
const char* LogTypeToString(LogType type);

enum class LogChannel : U32
{
	None = 0,
	Global = 1 << 0,
	System = 1 << 1,
	Math = 1 << 2,
	Application = 1 << 3,
	Graphics = 1 << 4,
    Map = 1 << 5,
	Animation = 1 << 6,
	// TODO : Add others
	All = (1 << 7) - 1,
};
const char* LogChannelToString(LogChannel type);

struct LogMessage
{
	LogType type;
	LogChannel channel;
	U32 verbosity;
	std::string message;
};

class Logger
{
	public:
		Logger();
		virtual ~Logger();

		void Enable(bool enable);
		bool IsEnabled() const;

		void SetTypeFilter(U32 typeFilter);
		U32 GetTypeFilter() const;

		void SetChannelFilter(U32 channelFilter);
		U32 GetChannelFilter() const;

		void SetImportanceFilter(U32 importanceFilter);
		U32 GetImportanceFilter() const;

		bool PassFilters(LogType type, LogChannel channel, U32 importance) const;

		virtual void Write(LogType type, LogChannel channel, U32 verbosity, const std::string& message) = 0;
	
		bool IsRegistered() const;

	protected:
		void RegisterLogger();
		void UnregisterLogger();

	private:
		U32 mTypeFilter;
		U32 mChannelFilter;
		U32 mImportanceFilter;
		bool mEnabled;
};

class LogManager
{
	ENLIVE_SINGLETON(LogManager);

	public:
		void Write(LogType type, LogChannel channel, U32 importance, const char* message, ...);
		void Error(const char* message, ...);

		void SetTypeFilter(U32 typeFilter);
		U32 GetTypeFilter() const;

		void SetChannelFilter(U32 channelFilter);
		U32 GetChannelFilter() const;

		void SetImportanceFilter(U32 importanceFilter);
		U32 GetImportanceFilter() const;

		U32 GetLoggerCount() const;

		bool Initialize();
		bool Uninitialize();
		bool IsInitialized() const;

	private:
		friend class Logger;
		void RegisterLogger(Logger* logger);
		void UnregisterLogger(Logger* logger);
		bool IsRegistered(const Logger* logger) const;

	private:
		void InternalWrite(LogType type, LogChannel channel, U32 importance, const char* message, va_list argList);

	private:
		std::vector<Logger*> mLoggers;
		Logger* mDefaultLogger;
		U32 mTypeFilter;
		U32 mChannelFilter;
		U32 mImportanceFilter;
		bool mInitialized;
};

class ConsoleLogger : public Logger
{
	public:
		ConsoleLogger();
		virtual ~ConsoleLogger();

		virtual void Write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};

class FileLogger : public Logger
{
	public:
		FileLogger(const std::string& filename = "");
		virtual ~FileLogger();

		void SetFilename(const std::string& filename);
		const std::string& GetFilename() const;

		virtual void Write(LogType type, LogChannel channel, U32 importance, const std::string& message);

	private:
		std::ofstream mFile;
		std::string mFilename;
};

#ifdef ENLIVE_COMPILER_MSVC
class VisualStudioLogger : public Logger
{
	public:
		VisualStudioLogger();
		virtual ~VisualStudioLogger();

		virtual void Write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};
#endif // ENLIVE_COMPILER_MSVC

#ifdef ENLIVE_PLATFORM_WINDOWS
class MessageBoxLogger : public Logger
{
	public:
		MessageBoxLogger();
		virtual ~MessageBoxLogger();

		virtual void Write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};
#endif // ENLIVE_PLATFORM_WINDOWS

} // namespace en

#define LogInfo(channel, importance, message, ...) ::en::LogManager::GetInstance().Write(en::LogType::Info, channel, importance, message, __VA_ARGS__);
#define LogWarning(channel, importance, message, ...) ::en::LogManager::GetInstance().Write(en::LogType::Warning, channel, importance, message, __VA_ARGS__);
#define LogError(channel, importance, message, ...) ::en::LogManager::GetInstance().Write(en::LogType::Error, channel, importance, message, __VA_ARGS__);

#else

#define LogInfo(channel, importance, message, ...)
#define LogWarning(channel, importance, message, ...)
#define LogError(channel, importance, message, ...)

#endif // ENLIVE_ENABLE_LOG
