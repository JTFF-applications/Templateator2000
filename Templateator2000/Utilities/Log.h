#pragma once

#include <spdlog/spdlog.h>

#include <memory>

class Log
{
public:
	Log();
	~Log() = default;

	static std::shared_ptr<spdlog::logger>& Get();

private:
	static Log* s_instance;
	std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_TRACE(...)	::Log::Get()->trace(__VA_ARGS__)
#define LOG_INFO(...)	::Log::Get()->info(__VA_ARGS__)
#define LOG_WARN(...)	::Log::Get()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	::Log::Get()->error(__VA_ARGS__)
