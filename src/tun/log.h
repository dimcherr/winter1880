#pragma once
#include <format>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "tunstring.h"

namespace tun {

class LogState {
public:
    String cerrLogPath {};
    String stderrLogPath {};
    std::ofstream logFile {};
    std::ofstream errorFile {};

    static LogState& Get() {
        static LogState state;
        return state;
    }

    LogState(const LogState&) = delete;
    LogState& operator=(const LogState&) = delete;

private:
    LogState() {}
};

class redirect_outputs
{
    std::ostream& myStream;
    std::streambuf *const myBuffer;
public:
    redirect_outputs ( std::ostream& lhs, std::ostream& rhs=std::cout )
        : myStream(rhs), myBuffer(myStream.rdbuf())
    {
        myStream.rdbuf(lhs.rdbuf());
    }

    ~redirect_outputs () {
        myStream.rdbuf(myBuffer);
    }
};

class capture_outputs
{
    std::ostringstream myContents;
    const redirect_outputs myRedirect;
public:
    capture_outputs ( std::ostream& stream=std::cout )
        : myContents(), myRedirect(myContents, stream)
    {}
    std::string contents () const
    {
        return (myContents.str());
    }
};

void InitLogging(StringView logPath, StringView errorLogPath, StringView cerrLogPath, StringView stderrLogPath);

template<typename ...Args>
inline void log(std::format_string<Args...> fmt, Args&&... args) {
    auto& state = LogState::Get();
    state.logFile << "[info]: " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
    std::cout << "[info]: " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
}

template<typename ...Args>
inline void error(std::format_string<Args...> fmt, Args&&... args) {
    auto& state = LogState::Get();
    state.errorFile << "[error]: " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
    std::cout << "[error]: " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
}

template<typename ...Args>
inline String formatToString(std::format_string<Args...> fmt, Args&&... args) {
    auto& state = LogState::Get();
    return std::vformat(fmt.get(), std::make_format_args(args...));
}

}
