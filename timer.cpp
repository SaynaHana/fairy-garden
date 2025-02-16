#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void)
{
    start_time_ = 0;
    end_time_ = 0;
    enabled_ = false;
}


Timer::~Timer(void)
{
}


void Timer::Start(float end_time)
{
    start_time_ = glfwGetTime();
    end_time_ = end_time;
    enabled_ = true;
}


bool Timer::Finished(void) const
{
    return glfwGetTime() - start_time_ >= end_time_;
}

} // namespace game
