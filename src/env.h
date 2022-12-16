#pragma once

#include <string>

namespace utils {

template <typename T = std::string>
T GetEnv(const char* key, T default_value) {
  const char* value = std::getenv(key);
  if (value == nullptr) {
    return default_value;
  }
  return T(value);
}

template <>
inline int GetEnv<int>(const char* key, int default_value) {
  const char* value = std::getenv(key);
  if (value == nullptr) {
    return default_value;
  }
  try {
    return std::stoi(value);
  } catch (...) {
    return default_value;
  }
}

template <>
inline size_t GetEnv<size_t>(const char* key, size_t default_value) {
  const char* value = std::getenv(key);
  if (value == nullptr) {
    return default_value;
  }
  try {
    return std::stoul(value);
  } catch (...) {
    return default_value;
  }
}

template <>
inline float GetEnv<float>(const char* key, float default_value) {
  const char* value = std::getenv(key);
  if (value == nullptr) {
    return default_value;
  }
  try {
    return std::stof(value);
  } catch (...) {
    return default_value;
  }
}

}  // namespace utils
