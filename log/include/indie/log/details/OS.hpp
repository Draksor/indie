#pragma once

#include <string>
#include <chrono>
#include <ctime>

namespace indie::log::details::OS
{

/**
 * New line definition
 * Windows uses `\r\n` to represent new lines
 * Instead, most of Unix based system use '\n' for new lines
 */
#ifdef _WIN32
#define LOG_NL "\r\n"
#else
#define LOG_NL "\n"
#endif

/**
 * Return current date in the format Day Month Date hh:mm:ss Year
 */
inline std::string GetCurrentTime() noexcept
{
  auto tp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string time = std::ctime(&tp);

  return time.substr(0, time.size() - 1);
}
}