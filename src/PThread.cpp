#include "PThread.h"

namespace mlib {

static PThread * thisThread = 0;

#ifndef _WIN32
void * InternalM (void *args) {
  PThread *thread = static_cast <PThread *> (args);
  thread->Main();
  thread->Stop();
  return NULL;
}
#else
DWORD WINAPI InternalM(void *args) {
  PThread *thread = static_cast <PThread *> (args);
  thread->Main();
  thread->Stop();
  return 0;
}
void sleep(DWORD d) {
  Sleep(d);
}
#endif

PThread::PThread() :
  threadID(0), args(0), running(false) {
}

PThread::PThread(void* a) :
  threadID(0), args(a), running(false) {
}

bool PThread::IsRunning() {
  return running;
}

void PThread::Wait() {
  if (!running)
    return;
#ifdef _WIN32
  WaitForSingleObject(threadID, INFINITE);
#else
  void * status;
  pthread_join(threadID, &status);
#endif
}

void PThread::Start() {
  WaitAndSignal m(mutex);
  thisThread = this;
#ifdef _WIN32
  threadID = CreateThread(0, 0, InternalM, (LPVOID)this, 0, 0);
  if (threadID != NULL)
    running = true;
#else
  int r = pthread_create (&threadID, 0, InternalM, (void*)this);
  if (!r)
  running = true;
#endif
}

void PThread::Sleep(long n) {
#ifndef _WIN32
  usleep(n * 1000);
#else
  sleep((DWORD)n);
#endif
}

void PThread::NextThread() {
#ifdef _WIN32
  SwitchToThread();
#else
  sched_yield();
#endif
}

void PThread::Stop() {
  WaitAndSignal m(mutex);
  running = false;
#ifdef _WIN32
  TerminateThread(threadID, 0);
#else
  pthread_cancel(threadID);
#endif
}

void PThread::Suspend() {
  //WaitAndSignal m(mutex);
  if (!running)
    return;
#ifdef _WIN32
  SuspendThread(threadID);
#else
  pthread_kill(threadID, SIGSTOP);
#endif
}

void PThread::Resume() {
  WaitAndSignal m(mutex);
  if (!running)
    return;
#ifdef _WIN32
  ResumeThread(threadID);
#else
  pthread_kill(threadID, SIGCONT);
#endif
}

void PThread::Restart() {
  WaitAndSignal m(mutex);
  Stop();
  Start();
}

PThread::~PThread() {
#ifdef _WIN32
  CloseHandle(threadID);
#else
#endif
}

}
