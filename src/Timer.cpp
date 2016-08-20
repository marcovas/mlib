#include "Timer.h"

#include <iostream>
#include <iomanip>

using std::setfill;
using std::setw;

#ifndef _WIN32
unsigned long timeval2int(const timeval *t) {
  return (long)((t->tv_sec)*1000000+t->tv_usec);
}
#endif // _WIN32

namespace mlib {

Timer::Timer(double d)
#ifndef _WIN32
: frequency ((double)d / 1000000), count (0) {
#else
:
  frequency((double)d / 1000), count(0) {
#endif //_WIN32
}

void Timer::Wait() {
  if (count == 0) {
#ifndef _WIN32
    gettimeofday (&time0, 0);
    startTime = timeval2int (&time0);
#else
    startTime = GetTickCount();
#endif   //_WIN32
  } else {
    waitTime = (unsigned long) (startTime + (1 / frequency) * count);
#ifndef _WIN32
    gettimeofday (&current, 0);
    currentTime = timeval2int (&current);
    if (waitTime> currentTime)
    usleep (waitTime - currentTime);
#else
    currentTime = GetTickCount();
    if (waitTime > currentTime)
      Sleep(waitTime - currentTime);
#endif   //_WIN32
  }
  count++;
}

void Timer::SetFrequency(double d) {
#ifndef _WIN32
  frequency = (double)d / 1000000;
#else
  frequency = (double)d / 1000;
#endif
  Reset();
}

double Timer::GetFrequency() const {
  return frequency;
}

Timer::~Timer() {
}

ElapsedTime Timer::GetElapsedTime() const {
  ElapsedTime t(startTime, currentTime);
  return t;
}

ElapsedTime::ElapsedTime(unsigned long st, unsigned long et) :
  startTime(st), endTime(et) {
}

unsigned ElapsedTime::GetHours() const {
  unsigned x = (GetElapsedMiliSeconds() / 3600000) % 60;
  return x;
}

unsigned ElapsedTime::GetMinutes() const {
  unsigned x = (GetElapsedMiliSeconds() / 60000) % 60;
  return x;
}

unsigned ElapsedTime::GetSeconds() const {
  unsigned x = (GetElapsedMiliSeconds() / 1000) % 60;
  return x;
}

unsigned ElapsedTime::GetElapsedMiliSeconds() const {
  unsigned x = 0;
#ifndef _WIN32
  x = (endTime - startTime) / 1000;
#else
  return endTime - startTime;
#endif
  return x;
}

unsigned ElapsedTime::GetMilisseconds() const {
  return GetElapsedMiliSeconds();
}

string ElapsedTime::FormatTime() const {
  stringstream ss(stringstream::out);
  ss << setfill('0') << setw(2) << GetHours() << ':' << setfill('0') << setw(2)
      << GetMinutes() << ':' << setfill('0') << setw(2) << GetSeconds() << '.'
      << setfill('0') << setw(2) << GetMilisseconds();
  return ss.str();
}

}