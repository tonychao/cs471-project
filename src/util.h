#ifndef _UTIL_H_
#define _UTIL_H_

#include <ctime>       //time
#include <iostream>


/// @brief register the CPU processing time
/// this class use the clock function to calculate the CPU processing time
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-11
class Clock
{
    private:
    
    std::clock_t start_c, stop_c; 
            
    public:

    /// @brief register the clock when the code start
    void tic()
    {
        start_c = std::clock();
    }

    /// @brief register the clock when the code end, also return the elapsed cpu time
    /// 
    /// @return return the cpu elapsed time between tic() and tac()
    double tac()
    {
        stop_c = std::clock();
         // CLOCKS_PER_SEC=1000000 in linux   CLOCKS_PER_SEC=1000 in windows
        return  ((double)stop_c - (double)start_c)/CLOCKS_PER_SEC*1000.0; // convert to milisecond
    }
};


/// @brief template function to print array of any type
/// 
/// @param array pointer of input array
/// @param n size of the input array
/// @param separator separator char between each element of the array
/// @date 2020-02-11
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