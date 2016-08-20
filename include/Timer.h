#ifndef TIMER_H_
#define TIMER_H_

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#include <windows.h>
#include <winbase.h>
#endif // _WIN32

#include <sys/time.h>

#ifndef _WIN32
unsigned long timeval2int (const timeval *t);
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using std::stringstream;
using std::string;

namespace mlib {

class ElapsedTime {
public:
   ElapsedTime(unsigned long startTime, unsigned long endTime);
   unsigned GetMinutes() const;
   unsigned GetHours() const;
   unsigned GetSeconds() const;
   unsigned GetElapsedMiliSeconds() const;
   unsigned GetMilisseconds() const;
   string FormatTime() const;
private:
   unsigned long startTime;
   unsigned long endTime;
};

class Timer
{
public:
   Timer(double frequency);
   void Wait ();
   void Reset () { count = 0; }
   ElapsedTime GetElapsedTime() const;
   void SetFrequency(double);
   double GetFrequency() const;
   virtual ~Timer();
private:
   double frequency;
   unsigned long count;
#ifndef _WIN32
   timeval time0;
   timeval current;
   int inicio;
#else
#endif //_WIN32
   unsigned long startTime;
   unsigned long currentTime;
   unsigned long waitTime;
   unsigned long oTime;
};
}

#endif /*TIMER_H_*/
