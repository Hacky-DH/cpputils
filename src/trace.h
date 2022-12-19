#pragma once

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <regex>

namespace utils {

void PrintTrace(void) {
  void* array[50];
  int size = backtrace(array, 50);
  // backtrace_symbols_fd(array, size, STDERR_FILENO);
  char** messages = backtrace_symbols(array, size);
  if (messages != nullptr) {
    fprintf(stderr, "You Got an exception, here is stacktrace(%d):\n", size);
    /* skip first stack frame (points here) */
    for (int i = 1; i < size; ++i) {
#ifndef NDEBUG
      fprintf(stderr, "#%d\t %s\n", i, ResolveTrace(messages[i]).c_str());
#else
      fprintf(stderr, "#%d\t %s\n", i, messages[i]);
#endif
    }
    free(messages);
  }
}

std::string CallOutput(const char* cmd) {
  std::array<char, 1024> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

//  use `addr2line -f -C -e` to show source code filename+line number
std::string ResolveTrace(const std::string& trace) {
  static std::string pattern = R"raw((.*)\(\+(0x[0-9a-f]+)\).\[(.*)\])raw";
  static std::regex express(pattern);
  std::smatch m;
  auto res = std::regex_match(trace, m, express);
  if (res and m.size() > 3) {
    std::string cmd = "addr2line -f -C -e ";
    cmd.append(m[1]).push_back(' ');
    cmd.append(m[2]);
    std::string outs = CallOutput(cmd.c_str());
    for (size_t i = 0; i < outs.size(); i++) {
      if (outs[i] == '\n' or outs[i] == '\r') {
        outs[i] = '\t';
      }
    }
    std::ostringstream oss;
    oss << "Object " << m[1] << ", at " << m[2] << ", in " << outs << "["
        << m[3] << "]";
    return oss.str();
  }
  return trace;
}

void _handler(int sig) {
  signal(sig, SIG_DFL);
  PrintTrace();
  exit(1);
}

void SetupSignals() {
  signal(SIGSEGV, _handler);
  signal(SIGABRT, _handler);
}

}  // namespace utils
