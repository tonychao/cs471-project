
#include <iostream>
#include <string> //stoi
#include "functions1.h"
#include "functions1.cpp"  //it contains template class

#include "runner.h"
#include "runner.cpp" //it contains template class

#include "genetic_algorithm.h"
#include "genetic_algorithm.cpp" // contains template class


/// @brief  Run project 1, different benchmark functions 
///
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-01-31

int main(int argc, char** argv)
{
    int counter;
    int function_id;
    int n_samples;
    int dimensions;
    double range_min;
    double range_max;
    //printf("Program Name Is: %s\n",argv[0]); 
    if(argc!=6) 
    {
        printf("arguments: function_id number_of_samples dimensions range_min range_max\n");
        return 1;
    }
    else
    { 
        //printf("Number Of Arguments Passed: %d\n",argc); 
        //printf("----Following Are The Command Line Arguments Passed----\n"); 
        //for(counter=0;counter<argc;counter++) 
        //    printf("argv[%d]: %s\n",counter,argv[counter]);
        function_id = std::stoi(argv[1]);
        n_samples = std::stoi(argv[2]);
        dimensions = std::stoi(argv[3]);
        range_min = std::stod(argv[4]);
        range_max = std::stod(argv[5]);
    } 



    Runner <float,float> runner(dimensions,n_samples); // <Tinput, Toutput>, dimensions, n_sample
    runner.run(function_id, range_min, range_max); //function_id, range_min, range_max
    runner.runOptimization(0,"../../config/ga_config.csv",function_id,range_min, range_max);



    return 0;
}
