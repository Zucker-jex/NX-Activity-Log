
#include <fstream>
#include <ctime>
#include <cstdarg>
#include "utils/Debug.hpp"

namespace Utils {
    std::mutex mutex;
    void write_log(const char *pszFmt, ...) {
#ifdef ENABLE_DEBUG
        if (NULL == pszFmt || 0 == pszFmt[0]) return;

        std::time_t currentTime = std::time(nullptr);
        std::tm* time = std::localtime(&currentTime);
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", time);

        char logstr[MAX_LOG_LEN] = { 0 };
        va_list args;
        va_start(args, pszFmt);
        snprintf(logstr, MAX_LOG_LEN, pszFmt, args);
        va_end(args);

        std::lock_guard<std::mutex> lock(Utils::mutex);
        std::ofstream file("/switch/NX-Activity-Log/debug.log", std::ios::app);
        if (file.is_open()) {
            file << std::string(timestamp) + std::string(logstr) << std::endl;
        }
#endif
    }
};
