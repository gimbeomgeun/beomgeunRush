#include "stdafx.h"
#include "ZeroLogger.h"
#include "ZeroSystem.h"


#define MAX_BUF_SIZE 4096

using namespace std;

bool ZeroLogger::isUsing = false;

string extSmNum(int num) {
    char buf[10];
    if (num < 10) {
#if defined(_WIN32) || defined(_WIN64)
        sprintf_s(buf, sizeof(buf), "0%d", num);
#else
        snprintf(buf, sizeof(buf), "0%d", num);
#endif
    }
    else {
#if defined(_WIN32) || defined(_WIN64)
        sprintf_s(buf, sizeof(buf), "%d", num);
#else
        snprintf(buf, sizeof(buf), "%d", num);
#endif
    }
    return buf;
}

void SetConsoleTextColor(ZeroLogger::COLOR foreground, ZeroLogger::COLOR background) {
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            (int)foreground + (int)background * 16);
#endif
}

void SetConsoleTextColor() {
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            (int)ZeroLogger::COLOR::LIGHTGRAY + (int)ZeroLogger::COLOR::BLACK * 16);
#endif
}

void SetConsoleTextColor(string type) {
    if (type == "time") {
        SetConsoleTextColor(ZeroLogger::COLOR::DARKGRAY, ZeroLogger::COLOR::BLACK);
    }
    else if (type == "info") {
        SetConsoleTextColor(ZeroLogger::COLOR::BLACK, ZeroLogger::COLOR::LIGHTGRAY);
    }
    else if (type == "info_i") {
        SetConsoleTextColor(ZeroLogger::COLOR::LIGHTGRAY, ZeroLogger::COLOR::BLACK);
    }
    else if (type == "error") {
        SetConsoleTextColor(ZeroLogger::COLOR::BLACK, ZeroLogger::COLOR::RED);
    }
    else if (type == "error_i") {
        SetConsoleTextColor(ZeroLogger::COLOR::RED, ZeroLogger::COLOR::BLACK);
    }
    else if (type == "debug") {
        SetConsoleTextColor(ZeroLogger::COLOR::BLACK, ZeroLogger::COLOR::CYAN);
    }
    else if (type == "debug_i") {
        SetConsoleTextColor(ZeroLogger::COLOR::CYAN, ZeroLogger::COLOR::BLACK);
    }
    else {
        SetConsoleTextColor();
    }
}

string ZeroLogger::getTimeNow() {
    time_t timeNow = time(nullptr);
    struct tm pLocal {};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&pLocal, &timeNow);
#else
    localtime_r(&timeNow, &pLocal);
#endif
    if (&pLocal == nullptr) {
        ZeroLogger::Error("ZeroLogger::getTimeNow returned error : time get failed");
        return "TIME_SHOW_ERROR";
    }

    return "[" + extSmNum((&pLocal)->tm_year + 1900) + "-" +
           extSmNum((&pLocal)->tm_mon + 1) + "-" +
           extSmNum((&pLocal)->tm_mday) + " " + extSmNum((&pLocal)->tm_hour) + ":" +
           extSmNum((&pLocal)->tm_min) + ":" + extSmNum((&pLocal)->tm_sec) + "] ";
}

void ZeroLogger::RawLogger(const char* mes, ...) {
    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
    cout << ZeroLogger::getTimeNow();
    SetConsoleTextColor();
    cout << buf << endl;
#else
    fprintf(stdout, "%s\x1b[0m%s\n", ZeroWopLogger::getTimeNow().c_str(), buf);
#endif
    ZeroLogger::isUsing = false;
}

void ZeroLogger::Info(const char* mes, ...) {
    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
    cout << ZeroLogger::getTimeNow();
    SetConsoleTextColor(ZeroLogger::COLOR::BLACK, ZeroLogger::COLOR::LIGHTGRAY);
    cout << " INFO ";
    SetConsoleTextColor();
    cout << " " << buf << endl;
#else
    fprintf(stdout, "%s\x1b[47;30m INFO \x1b[0m %s\n", ZeroWopLogger::getTimeNow().c_str(), buf);
#endif
    ZeroLogger::isUsing = false;
}

void ZeroLogger::ErrorWithDialog(const char* mes, ...) {
    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
    cout << ZeroLogger::getTimeNow();
    SetConsoleTextColor("error");
    cout << " ERRR ";
    SetConsoleTextColor("error_i");
    cout << buf << endl;
    SetConsoleTextColor();
#else
    fprintf(stderr, "%s\x1b[30;41m ERRR \x1b[31;40m %s\x1b[0m\n",
        ZeroWopLogger::getTimeNow().c_str(), buf);
#endif

#if defined(_WIN32) || defined(_WIN64)
    wchar_t application_name[128] = L"", content[4096] = L"";
    size_t application_name_size, content_size;
    mbstowcs_s(&application_name_size, application_name, 128, ZERO_WINDOW->GetApplicationName().c_str(), 128);
    mbstowcs_s(&content_size, content, 4096, mes, 4096);
    MessageBox(NULL, reinterpret_cast<LPCSTR>(content), reinterpret_cast<LPCSTR>(application_name), MB_OK);
#else
    ZeroWopLogger::Debug("ZeroWopLogger::Error - ErrorStackDialogue is avaliable only in WINDOWS");
#endif
    ZeroLogger::isUsing = false;
}

void ZeroLogger::Error(const char* mes, ...) {
    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
    cout << ZeroLogger::getTimeNow();
    SetConsoleTextColor("error");
    cout << " ERRR ";
    SetConsoleTextColor("error_i");
    cout << " " << buf << endl;
    SetConsoleTextColor();
#else
    fprintf(stderr, "%s\x1b[30;41m ERRR \x1b[31;40m %s\x1b[0m\n",
        ZeroWopLogger::getTimeNow().c_str(), buf);
#endif
    ZeroLogger::isUsing = false;
}

void ZeroLogger::Debug(const char* mes, ...) {
#if defined(DEBUG) | defined(_DEBUG)

    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
    cout << ZeroLogger::getTimeNow();
    SetConsoleTextColor("debug");
    cout << " DEBG ";
    SetConsoleTextColor("debug_i");
    cout << " " << buf << endl;
    SetConsoleTextColor();
#else
    fprintf(stdout, "%s\x1b[30;46m DEBG \x1b[36;40m %s\x1b[0m\n", ZeroWopLogger::getTimeNow().c_str(), buf);
#endif

    ZeroLogger::isUsing = false;
#endif
}

void ZeroLogger::ClearWindow() {
    while (ZeroLogger::isUsing)
        ;
    ZeroLogger::isUsing = true;
    SetConsoleTextColor("time");

#if defined(_WIN32) || defined(_WIN64)
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
            console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(
            console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
    SetConsoleTextColor();
#else
    //    if (!cur_term)
    //    {
    //        int result;
    //        setupterm( nullptr, STDOUT_FILENO, &result );
    //        if (result <= 0) return;
    //    }
    //
    //    putp(tigetstr("clear"));
    //TODO : Cross Platform
    cout << string(20, '\n');
#endif
    ZeroLogger::isUsing = false;
}