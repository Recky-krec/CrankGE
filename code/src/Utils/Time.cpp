#include "Time.h"


namespace crank
{
    std::string getLocalTime()
    {
        std::time_t t = time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

} // namespace crank

