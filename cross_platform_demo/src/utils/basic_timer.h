//
// Created by william on 2020/9/21.
//

#ifndef CPP_DEMO_BASIC_TIMER_H
#define CPP_DEMO_BASIC_TIMER_H

#pragma once
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
    #include <windows.h>
#elif !defined(__ANDROID__) && defined(__linux__) || defined(__APPLE__)
    #include <sys/time.h>
#endif
#include <ctime>
// 用于基本计时的帮助器类。
class BasicTimer
{
public:
    // 初始化内部计时器值。
    BasicTimer()
    {
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
        if (!QueryPerformanceFrequency(&m_frequency))
        {
            exit(0);
        }
#endif
        Reset();
    }

    // 将计时器重置为初始值。
    void Reset()
    {
        Update();
        m_startTime = m_currentTime;
        m_total = 0.0f;
        m_delta = 1000.0f / 60.0f;
    }

    // 更新计时器的内部值。
    void Update()
    {
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
        if (!QueryPerformanceCounter(&m_currentTime))
        {
            exit(0);
        }

        m_total = static_cast<float>(static_cast<double>(m_currentTime.QuadPart -
                                                         m_startTime.QuadPart) /
                                     static_cast<double>(m_frequency.QuadPart)) *
            1000.0f;

        m_delta = static_cast<float>(static_cast<double>(m_currentTime.QuadPart -
                                                         m_lastTime.QuadPart) /
                                     static_cast<double>(m_frequency.QuadPart)) *
            1000.0f;
#else
        gettimeofday(&m_currentTime, nullptr);
        double timeuse = 1000000.0 * (double)(m_currentTime.tv_sec - m_startTime.tv_sec) +
            m_currentTime.tv_usec - m_startTime.tv_usec;
        m_total = static_cast<float>(timeuse * 0.001);
        timeuse = 1000000.0 * (double)(m_currentTime.tv_sec - m_lastTime.tv_sec) +
            m_currentTime.tv_usec - m_lastTime.tv_usec;
        m_delta = static_cast<float>(timeuse * 0.001);
#endif

        m_lastTime = m_currentTime;
    }

    // 在最后一次调用 Reset()与最后一次调用 Update()之间的持续时间(毫秒)。
    [[nodiscard]] float GetTotal() const { return m_total; }

    // 在对 Update()的前两次调用之间的持续时间(毫秒)。
    [[nodiscard]] float GetDelta() const { return m_delta; }

    [[maybe_unused]] float UpdateAndGetDelta()
    {
        this->Update();
        return GetDelta();
    }

    [[maybe_unused]] float UpdateAndGetTotal()
    {
        this->Update();
        return GetTotal();
    }

private:
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_currentTime;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_lastTime;
#else
    struct timeval m_startTime
    {};
    struct timeval m_currentTime
    {};
    struct timeval m_lastTime
    {};
#endif
    float m_total{};
    float m_delta{};
};

#endif // CPP_DEMO_BASIC_TIMER_H
