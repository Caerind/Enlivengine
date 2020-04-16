#include <Enlivengine/System/Log.hpp>

#ifdef ENLIVE_ENABLE_LOG

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/String.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
	#include <imgui/imgui.h>
#endif

#ifdef ENLIVE_PLATFORM_WINDOWS
	#include <windows.h>
#endif

namespace en
{

const char* LogTypeToString(LogType type)
{
	switch (type)
	{
	case LogType::Info: return "Info";
	case LogType::Warning: return "Warning";
	case LogType::Error: return "Error";
	default: break;
	}
	assert(false);
	return nullptr;
}

const char* LogChannelToString(LogChannel channel)
{
	switch (channel)
	{
	case LogChannel::Global: return "Global";
	case LogChannel::System: return "System";
	case LogChannel::Math: return "Math";
	case LogChannel::Application: return "Application";
	case LogChannel::Graphics: return "Graphics";
	case LogChannel::Map: return "Map";
	case LogChannel::Animation: return "Animation";
	default: break;
	}
	return "None";
}

Logger::Logger()
	: mTypeFilter(static_cast<U32>(LogType::All))
	, mChannelFilter(static_cast<U32>(LogChannel::All))
	, mImportanceFilter(0)
	, mEnabled(true)
{
	RegisterLogger();
}

Logger::~Logger()
{
	UnregisterLogger();
}

void Logger::Enable(bool enable)
{
	mEnabled = enable;
}

bool Logger::IsEnabled() const
{
	return mEnabled;
}

void Logger::SetTypeFilter(U32 typeFilter)
{
	mTypeFilter = typeFilter;
}

U32 Logger::GetTypeFilter() const
{
	return mTypeFilter;
}

void Logger::SetChannelFilter(U32 channelFilter)
{
	mChannelFilter = channelFilter;
}

U32 Logger::GetChannelFilter() const
{
	return mChannelFilter;
}

void Logger::SetImportanceFilter(U32 importanceFilter)
{
	mImportanceFilter = importanceFilter;
}

U32 Logger::GetImportanceFilter() const
{
	return mImportanceFilter;
}

bool Logger::PassFilters(LogType type, LogChannel channel, U32 importance) const
{
	return ((U32)channel & mChannelFilter) != 0
		&& ((U32)type & mTypeFilter) != 0
		&& importance >= mImportanceFilter;
}

bool Logger::IsRegistered() const
{
	return LogManager::GetInstance().IsRegistered(this);
}

void Logger::RegisterLogger()
{
	LogManager::GetInstance().RegisterLogger(this);
}

void Logger::UnregisterLogger()
{
	LogManager::GetInstance().UnregisterLogger(this);
}

LogManager::LogManager()
	: mLoggers()
	, mDefaultLogger(nullptr)
	, mTypeFilter()
	, mChannelFilter()
	, mImportanceFilter()
	, mInitialized(false)
{
}

void LogManager::Write(LogType type, LogChannel channel, U32 importance, const char* message, ...)
{
	va_list argList;
	va_start(argList, message);
	InternalWrite(type, channel, importance, message, argList);
	va_end(argList);
}

void LogManager::Error(const char * message, ...)
{
	va_list argList;
	va_start(argList, message);
	InternalWrite(LogType::Error, LogChannel::Global, 10, message, argList);
	va_end(argList);
}

void LogManager::SetTypeFilter(U32 typeFilter)
{
	mTypeFilter = typeFilter;
}

U32 LogManager::GetTypeFilter() const
{
	return mTypeFilter;
}

void LogManager::SetChannelFilter(U32 channelFilter)
{
	mChannelFilter = channelFilter;
}

U32 LogManager::GetChannelFilter() const
{
	return mChannelFilter;
}

void LogManager::SetImportanceFilter(U32 importanceFilter)
{
	mImportanceFilter = importanceFilter;
}

U32 LogManager::GetImportanceFilter() const
{
	return mImportanceFilter;
}

U32 LogManager::GetLoggerCount() const
{
	return static_cast<U32>(mLoggers.size());
}

bool LogManager::Initialize()
{
	if (!IsInitialized())
	{
		mLoggers.clear();
		mTypeFilter = static_cast<U32>(LogType::All);
		mChannelFilter = static_cast<U32>(LogChannel::All);
		mImportanceFilter = 0;

#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
		mDefaultLogger = new ConsoleLogger();
#endif

		mInitialized = true;
	}
	return true;
}

bool LogManager::Uninitialize()
{
	if (IsInitialized())
	{
		#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
			delete mDefaultLogger;
			mDefaultLogger = nullptr;
		#endif

		mLoggers.clear();

		mInitialized = false;
	}
	return true;
}

bool LogManager::IsInitialized() const
{
	return mInitialized;
}

void LogManager::RegisterLogger(Logger* logger)
{
	if (logger != nullptr && !logger->IsRegistered())
	{
		mLoggers.push_back(logger);
	}
}

void LogManager::UnregisterLogger(Logger* logger)
{
	const U32 size = GetLoggerCount();
	for (U32 i = 0; i < size; i++)
	{
		if (mLoggers[i] == logger)
		{
			mLoggers.erase(mLoggers.begin() + i);
			return;
		}
	}
}

bool LogManager::IsRegistered(const Logger* logger) const
{
	const U32 size = GetLoggerCount();
	for (U32 i = 0; i < size; i++)
	{
		if (mLoggers[i] == logger)
		{
			return true;
		}
	}
	return false;
}

void LogManager::InternalWrite(LogType type, LogChannel channel, U32 importance, const char* message, va_list argList)
{
	static const U32 mBufferSize = 256;
	static char mBuffer[mBufferSize];

	std::string output;
	U32 size = std::vsnprintf(mBuffer, mBufferSize, message, argList);
	if (size < mBufferSize)
	{
		output = { mBuffer, size };
	}
	else
	{
		output = { mBuffer, mBufferSize };
	}

	if (((U32)type & mTypeFilter) != 0 && ((U32)channel & mChannelFilter) != 0 && importance >= mImportanceFilter && mLoggers.size() > 0)
	{
		for (const auto& logger : mLoggers)
		{
			if (logger->IsEnabled() && logger->PassFilters(type, channel, importance))
			{
				logger->Write(type, channel, importance, output);
			}
		}
	}
}

ConsoleLogger::ConsoleLogger()
	: Logger()
{
}

ConsoleLogger::~ConsoleLogger()
{
	UnregisterLogger();
}

void ConsoleLogger::Write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	printf("[%s][%s][%d] %s\n", LogTypeToString(type), LogChannelToString(channel), importance, message.c_str());
}

FileLogger::FileLogger(const std::string& filename)
	: Logger()
{
	SetFilename(filename);
}

FileLogger::~FileLogger()
{
	UnregisterLogger();
}

void FileLogger::SetFilename(const std::string& filename)
{
	if (filename.size() > 0)
	{
		if (mFile.is_open())
		{
			mFile.close();
		}

		mFile.open(filename);

		if (mFile.is_open())
		{
			mFilename = filename;
		}
		else
		{
			mFilename.clear();
		}
	}
}

const std::string& FileLogger::GetFilename() const
{
	return mFilename;
}

void FileLogger::Write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	if (mFile.is_open())
	{
		mFile << "[" << LogTypeToString(type) << "][" << LogChannelToString(channel) << "][" << importance << "] " << message.c_str() << '\n';
		mFile.flush();
	}
}

#ifdef ENLIVE_COMPILER_MSVC

VisualStudioLogger::VisualStudioLogger()
	: Logger()
{
}

VisualStudioLogger::~VisualStudioLogger()
{
	UnregisterLogger();
}

void VisualStudioLogger::Write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	static const U32 mDebugBufferSize = 256;
	static char mDebugBuffer[mDebugBufferSize];

	sprintf_s(mDebugBuffer, "[%s][%s][%d] %s\n", LogTypeToString(type), LogChannelToString(channel), importance, message.c_str());

	OutputDebugStringA(mDebugBuffer);
}

#endif // ENLIVE_COMPILER_MSVC

#ifdef ENLIVE_PLATFORM_WINDOWS

MessageBoxLogger::MessageBoxLogger()
	: Logger()
{
}

MessageBoxLogger::~MessageBoxLogger()
{
	UnregisterLogger();
}

void MessageBoxLogger::Write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	static const U32 mDebugBufferSize = 256;
	static char mDebugBuffer[mDebugBufferSize];

#ifdef ENLIVE_COMPILER_MSVC
	sprintf_s(mDebugBuffer, "NumeaEngine [%s] [%d]", LogChannelToString(channel), importance);
#else
	sprintf(mDebugBuffer, "NumeaEngine [%s] [%d]", LogChannelToString(channel), importance);
#endif

	U32 mbOption;
	switch (type)
	{
		case LogType::Error: mbOption = MB_ICONERROR; break;
		case LogType::Warning: mbOption = MB_ICONWARNING; break;
		case LogType::Info: mbOption = MB_ICONINFORMATION; break;
		default: mbOption = MB_OK; break;
	}

	MessageBox(nullptr, message.c_str(), mDebugBuffer, mbOption);
}

#endif // ENLIVE_PLATFORM_WINDOWS

} // namespace en

#endif // ENLIVE_ENABLE_LOG