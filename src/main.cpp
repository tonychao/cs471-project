
#include <iostream>
#include <string> //stoi
//#include "functions1.h"
#include "functions1.cpp"  //it contains template class

//#include "runner.h"
#include "runner.cpp" //it contains template class

//#include "genetic_algorithm.h"
#include "genetic_algorithm.cpp" // contains template class

#include "population.h"
#include "debug.h"
//#include "differential_evolution.h"
#include "differential_evolution.cpp" // contains template class

/// @brief  Project 3, Genetic Algorithm (GA) and Differential Evolution (DE) 
///
/// the main must receive 7 arguments to be able to run GA or DE algorithms with the 18 benchmarking functions
/// arguments: opt_alg_id opt_conf_file n_runs function_id dimensions range_min range_max
/// opt_alg_id = optimization algorithm id [0 = GA, 1-10 = DE]
/// opt_conf_file = optimization algorithm configuration file
/// n_runs = numbers of runs o iterations of a specific optimization algorithm and benchmarking function
/// function_id = id of benchmarking function
/// dimensions = dimensions of each vector o individual (size of each individual array)
/// range_min = range lower bound
/// range_max = range upper bound
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-01-31

int main(int argc, char** argv)
{
    int counter;
    int function_id;
    int n_runs;
    int dimensions;
    double range_min;
    double range_max;
    int opt_alg_id;
    std::string opt_conf_file;

    // c++ standard number
    std::cout << __cplusplus << std::endl;
    if (__cplusplus == 201703L)
        std::cout << "C++17" << std::endl;
    else if (__cplusplus == 201402L)
        std::cout << "C++14" << std::endl;
    else if (__cplusplus == 201103L)
        std::cout << "C++11" << std::endl;
    else if (__cplusplus == 199711L)
        std::cout << "C++98" << std::endl;
    else
        std::cout << "pre-standard C++" << std::endl;


    if(argc!=8) 
    {
        printf("arguments: opt_alg_id opt_conf_file n_runs function_id dimensions range_min range_max\n");
        return 1;
    }
    else
    { 

        opt_alg_id = std::stoi(argv[1]);
        opt_conf_file = argv[2];
        n_runs = std::stoi(argv[3]);
        function_id = std::stoi(argv[4]);
        dimensions = std::stoi(argv[5]);
        range_min = std::stod(argv[6]);
        range_max = std::stod(argv[7]);
    } 


    Runner <float,float> runner(n_runs); // <Tinput, Toutput>
    runner.runOptimization(opt_alg_id, opt_conf_file, function_id, dimensions, range_min, range_max);


    return 0;
}
