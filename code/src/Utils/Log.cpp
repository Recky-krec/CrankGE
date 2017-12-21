#include "Log.h"

namespace crank
{

// Instantiate static variables
std::ofstream Log::s_file;
Log::Level Log::s_prevLvl = Log::NONE;
std::string Log::s_lvlString;
std::ostringstream Log::s_osstream;

void Log::Init(const char* filename)
{
    // file is open until the end of execution
    s_file.open(filename, std::ios_base::trunc);
    s_file << "*======= CrankGE LOG =======*\n";
}

Log::~Log()
{
    s_file.close();
}

std::ostringstream& Log::Stream()
{
    return s_osstream;
}

std::ostringstream& Log::Write(Level lvl, const char* ident)
{
    if(ident == nullptr) ident = "";

    ReportingLevel(lvl);
    s_osstream << "\n\t" << crank::getLocalTime() << ident;

    Stream();
}

void Log::Flush()
{
    s_file << s_osstream.str() << std::flush;
}

void Log::ReportingLevel(Level lvl)
{
    if(s_prevLvl == lvl)
        return;

    switch(lvl)
    {
        case ERROR:
            s_lvlString = "Level => logERROR";
            break;

        case WARNING:
            s_lvlString = "Level => logWARNING";
            break;

        case INFO:
            s_lvlString = "Level => logINFO";
            break;

        case DEBUG:
            s_lvlString = "Level => logDEBUG";
            break;

        case NONE:
            s_lvlString = "Level => logNONE";
            break;
    }

    s_osstream << "\n" <<s_lvlString;
    s_prevLvl = lvl;
}

} // namespace crank
