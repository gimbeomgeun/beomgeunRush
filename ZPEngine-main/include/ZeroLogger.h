//
// Created by EunwooSong on 2020-11-28.
//

#ifndef ZERO_ENGINE_ZEROLOGGER_H
#define ZERO_ENGINE_ZEROLOGGER_H

#include <string>
#include <ctime>

#if defined(_WIN32) | defined(_WIN64)
#include <Windows.h>
#endif

#define ZERO_LOGGER ZeroLogger

class ZeroLogger {
public:
    enum class COLOR {
        BLACK = 0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
        LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
        LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
    };
    static bool isUsing;
    static void RawLogger(const char *, ...);
    static void Info(const char *, ...);
    static void ErrorWithDialog(const char *, ...);
    static void Error(const char *, ...);
    static void Debug(const char *, ...);
    static void ClearWindow();
    static std::string getTimeNow();
};



#endif //ZERO_ENGINE_ZEROLOGGER_H
