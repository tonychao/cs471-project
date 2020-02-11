#ifndef _UTIL_H_
#define _UTIL_H_

#include <ctime>       //time
#include <iostream>
class Clock
{
    private:
    
    std::clock_t start_c, stop_c;
            
    public:
    void tic()
    {
        start_c = std::clock();
    }

    double tac()
    {
        stop_c = std::clock();
         // CLOCKS_PER_SEC=1000000 in linux   CLOCKS_PER_SEC=1000 in windows
        return  ((double)stop_c - (double)start_c)/CLOCKS_PER_SEC*1000.0; // convert to milisecond
    }
};



template <class T> 
void printArray(T* array, int n, char separator)
{
    for (int i = 0; i < n; i++) 
    {   
        std::cout<< array[i] << separator;
    }
    std::cout <<std::endl;
}
#endif