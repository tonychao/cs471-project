#ifndef _UTIL_H_
#define _UTIL_H_

#include <time.h>       //time

class Clock
{
    private:
    
    clock_t start_c, stop_c;
            
    public:
    void tic()
    {
        start_c = clock();
    }

    double tac()
    {
        stop_c = clock();
         // CLOCKS_PER_SEC=1000000 in linux   CLOCKS_PER_SEC=1000 in windows
        return  ((double)stop_c - (double)start_c)/CLOCKS_PER_SEC*1000.0; // convert to milisecond
    }
};

#endif