#include <Logger.hpp>

// Singleton instance
// This is the instance that getInstance retrieves
Logger *Logger::_logger = NULL;
/**
 * Constructors / destructors
 */
Logger::Logger() : _parentPid(getpid())
{
	if (!LOG_ENABLED)
	{
		return;
	}
	std::string filePath(LOG_DEST);
	filePath.append("log.log");
	_file.open(filePath.c_str(), std::ios::out | std::ios::app);
	if (!isFileOpen())
	{
		std::string errorMsg("Error: can't open log file: ");
		errorMsg.append(strerror(errno));
		Logger::error(errorMsg);
	}
	else
	{
		restart();
	}
}

Logger::~Logger()
{
	if (isFileOpen())
	{
		_file.close();
	}
}

/**
 * Logging levels
 */
void Logger::log(std::string const &message)
{
	logToConsole("\033[1;32m[INFO]:   \033[0m", INFO, message);
	logToFile("[INFO]:   ", message);
}

void Logger::error(std::string const &message)
{
	logToConsole("\033[1;31m[ERROR]: \033[0m", ERROR, message);
	logToFile("[ERROR]:  ", message);
}

void Logger::debug(std::string const &message)
{
	logToConsole("\033[1;35m[DEBUG]: \033[0m", INFO, message);
	logToFile("[DEBUG]:  ", message);
}

// Logs restart message to logfile
void Logger::restart(void)
{
	logToFile("[RESTART]", "");
}

/**
 * Logging & logging helper functions
 */
// Formatted as: DD  HH:MM:SS
std::string Logger::getTimeStamp() const
{
	std::string timeStamp = "[";
	time_t now = time(NULL);
	std::string timeToString = ctime(&now);
	timeToString = timeToString.substr(8, 11);
	timeStamp.append(timeToString);
	timeStamp.append("]: ");
	return timeStamp;
}

std::string Logger::getPid() const
{
	char pidString[11];
	pid_t currentPid = getpid();
	if (currentPid == _parentPid)
	{
		sprintf(pidString, "[ MAIN ]: ");
		return std::string(pidString);
	}
	sprintf(pidString, "[%d ]: ", currentPid);
	return std::string(pidString);
}

void Logger::logToFile(std::string const &levelMsg, std::string const &message)
{
	if (!isFileOpen() || LOG_ENABLED == 0)
	{
		return;
	}
	std::string fullMsg = getTimeStamp();
	fullMsg.append(getPid());
	fullMsg.append(levelMsg);
	fullMsg.append(message);
	_file << fullMsg << std::endl;
}

void Logger::logToConsole(std::string const &levelMsg, ELogLevel level, std::string const &message)
{
	if (LOG_ENABLED == 0)
	{
		return;
	}
	std::string fullMsg = getTimeStamp();
	fullMsg.append(getPid());
	fullMsg.append(levelMsg);
	fullMsg.append(message);

	// Print to stdout or stderr based on level
	switch (level)
	{
	case INFO:
		std::cout << fullMsg << std::endl;
		break;
	default:
		std::cerr << fullMsg << std::endl;
		break;
	}
}

/**
 * Getters / setters
 */
Logger &Logger::getInstance()
{
	if (_logger == NULL)
	{
		_logger = new Logger();
	}
	return *_logger;
}

bool Logger::isFileOpen() const
{
	return _file.is_open();
}
