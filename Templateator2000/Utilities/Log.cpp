#include "Log.h"

Log* Log::s_instance = new Log();

Log::Log()
	: m_logger(spdlog::basic_logger_mt("console", "logs/log.log"))
{
	spdlog::set_pattern("%v");
	m_logger->set_level(spdlog::level::trace);
	m_logger->trace("Log Initialized !");
	spdlog::set_pattern("[%T] (%l): %v");
}

inline std::shared_ptr<spdlog::logger>& Log::Get()
{
	return s_instance->m_logger;
}
