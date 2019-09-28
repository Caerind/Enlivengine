#include <Enlivengine/System/Log.hpp>

#include <Enlivengine/System/String.hpp>

#ifdef ENLIVE_PLATFORM_WINDOWS
	#include <windows.h>
#endif

namespace en
{

Logger::Logger()
	: mTypeFilter(static_cast<U32>(LogType::All))
	, mChannelFilter(static_cast<U32>(LogChannel::All))
	, mImportanceFilter(0)
	, mEnabled(true)
{
	registerLogger();
}

Logger::~Logger()
{
	unregisterLogger();
}

void Logger::enable(bool enable)
{
	mEnabled = enable;
}

bool Logger::isEnabled() const
{
	return mEnabled;
}

void Logger::setTypeFilter(U32 typeFilter)
{
	mTypeFilter = typeFilter;
}

U32 Logger::getTypeFilter() const
{
	return mTypeFilter;
}

void Logger::setChannelFilter(U32 channelFilter)
{
	mChannelFilter = channelFilter;
}

U32 Logger::getChannelFilter() const
{
	return mChannelFilter;
}

void Logger::setImportanceFilter(U32 importanceFilter)
{
	mImportanceFilter = importanceFilter;
}

U32 Logger::getImportanceFilter() const
{
	return mImportanceFilter;
}

bool Logger::passFilters(LogType type, LogChannel channel, U32 importance) const
{
	return ((U32)channel & mChannelFilter) != 0
		&& ((U32)type & mTypeFilter) != 0
		&& importance >= mImportanceFilter;
}

void Logger::registerLogger()
{
	LogManager::registerLogger(this);
}

void Logger::unregisterLogger()
{
	LogManager::unregisterLogger(this);
}

std::vector<Logger*> LogManager::sLoggers;
Logger* LogManager::sDefaultLogger = nullptr;
U32 LogManager::sTypeFilter;
U32 LogManager::sChannelFilter;
U32 LogManager::sImportanceFilter;
bool LogManager::sInitialized;

void LogManager::write(LogType type, LogChannel channel, U32 importance, const char* message, ...)
{
	va_list argList;
	va_start(argList, message);
	internalWrite(type, channel, importance, message, argList);
	va_end(argList);
}

void LogManager::error(const char * message, ...)
{
	va_list argList;
	va_start(argList, message);
	internalWrite(LogType::Error, LogChannel::Global, 10, message, argList);
	va_end(argList);
}

void LogManager::setTypeFilter(U32 typeFilter)
{
	sTypeFilter = typeFilter;
}

U32 LogManager::getTypeFilter()
{
	return sTypeFilter;
}

void LogManager::setChannelFilter(U32 channelFilter)
{
	sChannelFilter = channelFilter;
}

U32 LogManager::getChannelFilter()
{
	return sChannelFilter;
}

void LogManager::setImportanceFilter(U32 importanceFilter)
{
	sImportanceFilter = importanceFilter;
}

U32 LogManager::getImportanceFilter()
{
	return sImportanceFilter;
}

U32 LogManager::getLoggerCount()
{
	return sLoggers.size();
}

const char* LogManager::typeToString(LogType type)
{
	static const char* const logTypeStrings[] = 
	{ 
		"None", 
		"Info",  
		"Warning",
		"Error"
	};
	switch (type)
	{
		case LogType::Info: return logTypeStrings[1];
		case LogType::Warning: return logTypeStrings[2];
		case LogType::Error: return logTypeStrings[3];
	}
	return logTypeStrings[0];
}

const char* LogManager::channelToString(LogChannel channel)
{
	static const char* const logChannelStrings[] = 
	{ 
		"None",
		"Global", 
		"System", 
		"Math"
	};
	switch (channel)
	{
		case LogChannel::Global: return logChannelStrings[1];
		case LogChannel::System: return logChannelStrings[2];
		case LogChannel::Math: return logChannelStrings[3];
	}
	return logChannelStrings[0];
}

bool LogManager::initialize()
{
	if (!isInitialized())
	{
		sLoggers.clear();
		sTypeFilter = (U32)LogType::All;
		sChannelFilter = (U32)LogChannel::All;
		sImportanceFilter = 0;

		#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
			sDefaultLogger = new ConsoleLogger();
		#endif

		sInitialized = true;
	}
	return true;
}

bool LogManager::uninitialize()
{
	if (isInitialized())
	{
		#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
			delete sDefaultLogger;
			sDefaultLogger = nullptr;
		#endif

		sLoggers.clear();

		sInitialized = false;
	}
	return true;
}

bool LogManager::isInitialized()
{
	return sInitialized;
}

void LogManager::registerLogger(Logger* logger)
{
	sLoggers.push_back(logger);
}

void LogManager::unregisterLogger(Logger* logger)
{
	U32 size = sLoggers.size();
	for (U32 i = 0; i < size; i++)
	{
		if (sLoggers[i] == logger)
		{
			sLoggers.erase(sLoggers.begin() + i);
		}
	}
}

void LogManager::internalWrite(LogType type, LogChannel channel, U32 importance, const char* message, va_list argList)
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

	if (((U32)type & sTypeFilter) != 0 && ((U32)channel & sChannelFilter) != 0 && importance >= sImportanceFilter && sLoggers.size() > 0)
	{
		for (const auto& logger : sLoggers)
		{
			if (logger->isEnabled() && logger->passFilters(type, channel, importance))
			{
				logger->write(type, channel, importance, output);
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
	unregisterLogger();
}

void ConsoleLogger::write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	printf("[%s][%s][%d] %s\n", LogManager::typeToString(type), LogManager::channelToString(channel), importance, message.c_str());
}

FileLogger::FileLogger(const std::string& filename)
	: Logger()
{
	setFilename(filename);
}

FileLogger::~FileLogger()
{
	unregisterLogger();
}

void FileLogger::setFilename(const std::string& filename)
{
	if (!empty(filename))
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

const std::string& FileLogger::getFilename() const
{
	return mFilename;
}

void FileLogger::write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	if (mFile.is_open())
	{
		mFile << "[" << LogManager::typeToString(type) << "][" << LogManager::channelToString(channel) << "][" << importance << "] " << message.c_str() << '\n';
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
	unregisterLogger();
}

void VisualStudioLogger::write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	static const U32 mDebugBufferSize = 256;
	static char mDebugBuffer[mDebugBufferSize];

	sprintf_s(mDebugBuffer, "[%s][%s][%d] %s\n", LogManager::typeToString(type), LogManager::channelToString(channel), importance, message.c_str());

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
	unregisterLogger();
}

void MessageBoxLogger::write(LogType type, LogChannel channel, U32 importance, const std::string& message)
{
	static const U32 mDebugBufferSize = 256;
	static char mDebugBuffer[mDebugBufferSize];

	#ifdef ENLIVE_COMPILER_MSVC
	sprintf_s(mDebugBuffer, "NumeaEngine [%s] [%d]", LogManager::channelToString(channel), importance);
	#else
	sprintf(mDebugBuffer, "NumeaEngine [%s] [%d]", LogManager::channelToString(channel), importance);
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
