//
// Created by william on 2021/5/4.
//

#include "logger.h"

void Logger::log(Logger::Level level, const char* message, ...)
{
    time_t currentTime = time(nullptr);
    char timeBuf[100];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
    std::vector<char> dynamicBuffer;
    int size = 1024;
    char stackBuffer[1024];
    char* str = stackBuffer;
    for (;;)
    {
        va_list args;
        va_start(args, message);
        int needed = vsnprintf(str, size - 1, message, args);
        // the number of characters actually needed to fill the buffer.
        if (needed >= 0 && needed < size)
        {
            // Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
            str[needed] = '\0';
            va_end(args);
            break;
        }
        size = needed > 0 ? (needed + 1) : (size * 2);
        dynamicBuffer.resize(size);
        str = &dynamicBuffer[0];
        va_end(args);
    }
    switch (level)
    {
    case Level::Info:
        printf("%s, [Logger::Level::Info] : %s", timeBuf, str);
        break;
    case Level::Warn:
        printf("%s, [Logger::Level::Warn] : %s", timeBuf, str);
        break;
    case Level::Error:
        printf("%s, [Logger::Level::Error] : %s", timeBuf, str);
        break;
    }
}
