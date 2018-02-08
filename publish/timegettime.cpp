#ifdef WIN32
#else
#include <time.h>   
#include <sys/time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
unsigned int timeGetTime()  
{  
        unsigned int uptime = 0;  
        struct timespec on;  
#ifdef __MACH__
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        on.tv_sec = mts.tv_sec;
        on.tv_nsec = mts.tv_nsec;
        
        uptime = on.tv_sec*1000 + on.tv_nsec/1000000;  
        
#else
        if(clock_gettime(CLOCK_MONOTONIC, &on) == 0)  
                 uptime = on.tv_sec*1000 + on.tv_nsec/1000000;  
#endif
        return uptime;  
} 
#endif
