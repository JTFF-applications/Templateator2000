#include <spdlog/sinks/basic_file_sink.h>

#include "Utilities/StackWalker.h"
#include "Utilities/Log.h"

Log* Log::s_instance = new Log();
std::string Log::s_log_pattern = "[%T] (%l): %v";

Log::Log()
	: m_logger(spdlog::basic_logger_mt("console", "logs/log.log"))
{
	spdlog::set_pattern("%v");
	m_logger->set_level(spdlog::level::trace);
	m_logger->trace("Log Initialized !");
	spdlog::set_pattern(s_log_pattern);
}

Log::~Log()
{
	m_logger->set_pattern("%v");
	LOG_INFO("Exiting Application !");
	m_logger.reset();
}

Log* Log::Get()
{
	return s_instance;
}

std::shared_ptr<spdlog::logger>& Log::RawGet()
{
	return s_instance->m_logger;
}

void Log::LogStackTrace() const
{
	const auto exception_context = StackWalker::GetCurrentExceptionContext();

	StackWalker stack_walker(exception_context ? StackWalker::AfterCatch : StackWalker::NonExcept);
	auto lines = stack_walker.GetCallstack(GetCurrentThread(), exception_context);
	if (!lines.has_value())
		return;

	if (lines.value().back().empty())
		lines.value().pop_back();

	spdlog::set_pattern("%v");
	m_logger->error("-------------------- UNRECOVERABLE ERROR : SEE STACKTRACE --------------------");
	for (const std::string& line : lines.value())
		m_logger->trace(line.substr(0, line.length() - 1));
	spdlog::set_pattern(s_log_pattern);
}
