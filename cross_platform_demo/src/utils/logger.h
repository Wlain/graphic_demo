//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_LOGGER_H
#define RASTERIZER_LOGGER_H
#include "base.h"

class Logger
{
public:
    enum class Level : uint32_t
    {
        Info = 0,
        Warn,
        Error
    };

public:
    static void log(Level level, const char* message, ...);
};


#endif //RASTERIZER_LOGGER_H
