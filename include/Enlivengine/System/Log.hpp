#pragma once

#include <cstdarg>
#include <fstream>
#include <vector>

#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

enum class LogType : U32
{
	None = 0x0,
	Info = 0x1,
	Warning = 0x2,
	Error = 0x4,
	All = 0x7
};

enum class LogChannel : U32
{
	None = 0,
	Global = 1 << 0,
	System = 1 << 1,
	Math = 1 << 2,
	Application = 1 << 3,
	Graphics = 1 << 4,
	// TODO : Add others
	All = (1 << 5) - 1,
};

class Logger
{
	public:
		Logger();
		virtual ~Logger();

		void enable(bool enable);
		bool isEnabled() const;

		void setTypeFilter(U32 typeFilter);
		U32 getTypeFilter() const;

		void setChannelFilter(U32 channelFilter);
		U32 getChannelFilter() const;

		void setImportanceFilter(U32 importanceFilter);
		U32 getImportanceFilter() const;

		bool passFilters(LogType type, LogChannel channel, U32 importance) const;

		virtual void write(LogType type, LogChannel channel, U32 verbosity, const std::string& message) = 0;
	
	protected:
		void registerLogger();
		void unregisterLogger();

	private:
		U32 mTypeFilter;
		U32 mChannelFilter;
		U32 mImportanceFilter;
		bool mEnabled;
};

class LogManager
{
	public:
		LogManager() = delete;
		~LogManager() = delete;

	public:
		static void write(LogType type, LogChannel channel, U32 importance, const char* message, ...);
		static void error(const char* message, ...);

		static void setTypeFilter(U32 typeFilter);
		static U32 getTypeFilter();

		static void setChannelFilter(U32 channelFilter);
		static U32 getChannelFilter();

		static void setImportanceFilter(U32 importanceFilter);
		static U32 getImportanceFilter();

		static U32 getLoggerCount();

		static const char* typeToString(LogType type);
		static const char* channelToString(LogChannel channel);

		static bool initialize();
		static bool uninitialize();
		static bool isInitialized();

	private:
		friend class Logger;
		static void registerLogger(Logger* logger);
		static void unregisterLogger(Logger* logger);

	private:
		static void internalWrite(LogType type, LogChannel channel, U32 importance, const char* message, va_list argList);

	private:
		static std::vector<Logger*> sLoggers;
		static Logger* sDefaultLogger;
		static U32 sTypeFilter;
		static U32 sChannelFilter;
		static U32 sImportanceFilter;
		static bool sInitialized;
};

// Try to only call these macros
#ifdef ENLIVE_ENABLE_LOG
	#define LogInfo(channel, importance, message, ...) ::en::LogManager::write(en::LogType::Info, channel, importance, message, __VA_ARGS__);
	#define LogWarning(channel, importance, message, ...) ::en::LogManager::write(en::LogType::Warning, channel, importance, message, __VA_ARGS__);
	#define LogError(channel, importance, message, ...) ::en::LogManager::write(en::LogType::Error, channel, importance, message, __VA_ARGS__);
#else
	#define LogInfo(channel, importance, message, ...)
	#define LogWarning(channel, importance, message, ...)
	#define LogError(channel, importance, message, ...)
#endif

class ConsoleLogger : public Logger
{
	public:
		ConsoleLogger();
		virtual ~ConsoleLogger();

		virtual void write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};

class FileLogger : public Logger
{
	public:
		FileLogger(const std::string& filename = "");
		virtual ~FileLogger();

		void setFilename(const std::string& filename);
		const std::string& getFilename() const;

		virtual void write(LogType type, LogChannel channel, U32 importance, const std::string& message);

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

		virtual void write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};

#endif // ENLIVE_COMPILER_MSVC

#ifdef ENLIVE_PLATFORM_WINDOWS

class MessageBoxLogger : public Logger
{
	public:
		MessageBoxLogger();
		virtual ~MessageBoxLogger();

		virtual void write(LogType type, LogChannel channel, U32 importance, const std::string& message);
};

#endif // ENLIVE_PLATFORM_WINDOWS

} // namespace en