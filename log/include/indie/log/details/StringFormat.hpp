#pragma once

#include <string>

namespace indie::log::details::StringFormat
{

template <typename T>
inline std::string ToString([[maybe_unused]] T arg)
{
  return typeid(T).name();
}
template <>
inline std::string ToString<std::string>(std::string arg)
{
  return arg;
}
template <>
inline std::string ToString<char *>(char *ptr)
{
  return std::string(ptr);
}
template <>
inline std::string ToString<int>(int arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<unsigned>(unsigned arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<long>(long arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<unsigned long>(unsigned long arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<long long>(long long arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<unsigned long long>(unsigned long long arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<float>(float arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<double>(double arg)
{
  return std::to_string(arg);
}
template <>
inline std::string ToString<long double>(long double arg)
{
  return std::to_string(arg);
}

}