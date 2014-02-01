#include <time.h>

void ezSystemTime::Initialize()
{
}

void ezSystemTime::Shutdown()
{
}

ezTime ezSystemTime::Now()
{
  struct timespec sp;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sp);

  return ezTime::Seconds( (double)sp.tv_sec + (double)(sp.tv_nsec / 1000000000.0));
}
