#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_LOG

#include <fstream>
#include <vector>
#include <string>

#include <fmt/format.h>

#include <Enlivengine/System/Singleton.hpp>
#include <Enlivengine/System/MetaEnum.hpp>

namespace en
{

enum class LogType
{
	Info = 0,
	Warning,
	Error,
	Fatal
};

enum class LogChannel
{
	Global = 0,
	System,
	Math,
	Application,
	Graphics,
	Map,
	Animation,
	Core,
	Physic,

	// TODO : Add others

	Max
};

/*
Register your own LogChannels :

enum class LogChannelClient
{
	GameplayChannel1 = static_cast<en::U32>(en::LogChannel::Max),
	GameplayChannel2
	...
};

Then use LogManager::InitializeClientChannels<LogChannelClient>() in your main and check that the returned value is true :)
*/

struct LogMessage
{
	std::string message; // TODO : Might this be transformed to string_view ?
	U32 channel; 
	LogType type;

	std::string_view GetTypeString() const;
	std::string_view GetChannelString() const;
	std::string_view GetMessageString() const;
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

	void SetChannelFilter(U64 channelFilter);
	U64 GetChannelFilter() const;

	bool PassFilters(LogType type, U32 channelID) const;

	virtual void Write(const LogMessage& message) = 0;

protected:
	void RegisterLogger();
	void UnregisterLogger();

private:
	U64 mChannelFilter;
	U32 mTypeFilter;
	bool mEnabled;
};

class LogManager
{
	ENLIVE_SINGLETON(LogManager);

public:
	template <typename... Args>
	void Write(LogType type, U32 channelID, Args&& ...args);

	void SetTypeFilter(U32 typeFilter);
	U32 GetTypeFilter() const;

	void SetChannelFilter(U64 channelFilter);
	U64 GetChannelFilter() const;

	bool PassFilters(LogType type, U32 channelID) const;

	U32 GetLoggerCount() const;

	template <typename EnumClient>
	bool InitializeClientChannels();
	std::string_view GetChannelName(U32 channelID) const;

private:
	friend class Logger;
	void RegisterLogger(Logger* logger);
	void UnregisterLogger(Logger* logger);
	bool IsRegistered(const Logger* logger) const;

private:
	std::vector<Logger*> mLoggers;
	std::vector<std::string_view> mClientChannelNames;
	U64 mChannelFilter;
	U32 mTypeFilter;
};

template <typename... Args>
void LogManager::Write(LogType type, U32 channelID, Args&& ...args)
{
	if (PassFilters(type, channelID))
	{
		LogMessage message;
		message.message = fmt::format(std::forward<Args>(args)...);
		message.type = type;
		message.channel = channelID;

		for (const auto& logger : mLoggers)
		{
			if (logger->IsEnabled() && logger->PassFilters(type, channelID))
			{
				logger->Write(message);
			}
		}
	}
}

template <typename EnumClient>
bool LogManager::InitializeClientChannels()
{
	const auto enumValues = Meta::GetEnumValues<EnumClient>();
	mClientChannelNames.clear();
	if (static_cast<U32>(enumValues[0]) == static_cast<U32>(LogChannel::Max))
	{
		mClientChannelNames.reserve(enumValues.size());
		for (const auto& enumValue : enumValues)
		{
			mClientChannelNames.push_back(Meta::GetEnumName(enumValue));
		}
		return true;
	}
	else
	{
		return false;
	}
}

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

	virtual void Write(const LogMessage& message);

#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
private:
	static ConsoleLogger sConsoleLogger;
#endif // ENLIVE_ENABLE_DEFAULT_LOGGER
};

class FileLogger : public Logger
{
public:
	FileLogger(const std::string& filename = "");
	virtual ~FileLogger();

	void SetFilename(const std::string& filename);
	const std::string& GetFilename() const;

	virtual void Write(const LogMessage& message);

private:
	std::ofstream mFile;
	std::string mFilename;
};

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
class VisualStudioLogger : public Logger
{
public:
	VisualStudioLogger();
	virtual ~VisualStudioLogger();

	virtual void Write(const LogMessage& message);

#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
private:
	static VisualStudioLogger sVisualStudioLogger;
#endif // ENLIVE_ENABLE_DEFAULT_LOGGER
};
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)

} // namespace en

#define enLogInfo(channel, ...) ::en::LogManager::GetInstance().Write(::en::LogType::Info, static_cast<::en::U32>(channel), __VA_ARGS__);
#define enLogWarning(channel, ...) ::en::LogManager::GetInstance().Write(::en::LogType::Warning, static_cast<::en::U32>(channel), __VA_ARGS__);
#define enLogError(channel, ...) ::en::LogManager::GetInstance().Write(::en::LogType::Error, static_cast<::en::U32>(channel), __VA_ARGS__);
#define enLogFatal(channel, ...) ::en::LogManager::GetInstance().Write(::en::LogType::Fatal, static_cast<::en::U32>(channel), __VA_ARGS__);

#else

#define enLogInfo(channel, message, ...) 
#define enLogWarning(channel, message, ...) 
#define enLogError(channel, message, ...) 
#define enLogFatal(channel, message, ...) 

#endif // ENLIVE_ENABLE_LOG
