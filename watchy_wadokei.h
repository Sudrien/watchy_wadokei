#ifndef WATCHY_WADOKEI_H
#define WATCHY_WADOKEI_H

#include <Watchy.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <sunset.h>

// 丑寅卯辰巳午 未申酉戌亥子
#include "hours.h"

#define DEG2RAD 0.0174532925
#define PI 3.14159265358979323846

class WatchyWadokei : public Watchy{
    using Watchy::Watchy;
    public:
        WatchyWadokei(const watchySettings& s) : Watchy(s) {}
        void drawWatchFace();
    private:
  };
#endif
