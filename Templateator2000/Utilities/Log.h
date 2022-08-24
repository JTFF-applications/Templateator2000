#pragma once

#include <spdlog/spdlog.h>

#include <memory>

class Log
{
public:
	Log();
	~Log() = default;

	Log(const Log& other) = delete;
	Log(const Log&& other) = delete;
	Log& operator=(const Log& other) = delete;
	Log& operator=(const Log&& other) = delete;

	static Log* Get();
	static std::shared_ptr<spdlog::logger>& RawGet();

	void LogStackTrace() const;

private:
	static Log* s_instance;
	static std::string s_log_pattern;
	std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_TRACE(...)	::Log::RawGet()->trace(__VA_ARGS__)
#define LOG_INFO(...)	::Log::RawGet()->info(__VA_ARGS__)
#define LOG_WARN(...)	::Log::RawGet()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	::Log::RawGet()->error(__VA_ARGS__); \
						::Log::Get()->LogStackTrace()
