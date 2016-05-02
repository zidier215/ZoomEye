#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int get_current_time_str(char *time_str)
{
    return 0;
}

uint64_t get_time_ms_str(char *date, char *time)
{
  struct timeval tv;
  uint64_t t_ms = 0; 

  gettimeofday(&tv, NULL);
  t_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  struct tm *tm = localtime(&tv.tv_sec);
  
  snprintf(date, 16, "%04d-%02d-%02d",tm->tm_year+1900, tm->tm_mon + 1, tm->tm_mday);
  snprintf(time, 16, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
  return t_ms;
}
#if 0
int main()
{
  char year[128]="";
  char time[128]="";

  get_time_ms_str(year, time);
  printf("year:%s\ntime:%s\n", year, time);
  return 0;
}
#endif

