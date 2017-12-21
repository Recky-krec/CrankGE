#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils/Time.h"

// Log Labels
#define LOGEL crank::Log::Write(crank::Log::ERROR, " ")
#define LOGWL crank::Log::Write(crank::Log::WARNING, " ")
#define LOGIL crank::Log::Write(crank::Log::INFO, " ")
#define LOGDL crank::Log::Write(crank::Log::DEBUG, " ")

// Log Messages
#define LOGE crank::Log::Write(crank::Log::ERROR, "\t")
#define LOGW crank::Log::Write(crank::Log::WARNING, "\t")
#define LOGI crank::Log::Write(crank::Log::INFO, "\t")
#define LOGD crank::Log::Write(crank::Log::DEBUG, "\t")

namespace crank
{

class Log
{
public:
    enum Level
    {
        NONE     = 0,
        ERROR    = 1,
        WARNING  = 2,
        INFO     = 3,
        DEBUG    = 4,
    };

public:
    Log() = delete;
    ~Log();
    static void Init(const char* filename = "log.txt");
    static void ReportingLevel(Level lvl);
    static std::ostringstream& Stream();
    static std::ostringstream& Write(Level lvl, const char* ident);
    static void Flush();

private:
    static Level s_prevLvl;
    static std::string s_lvlString;
    static std::ofstream s_file;
    static std::ostringstream s_osstream;
};

} // namespace crank
