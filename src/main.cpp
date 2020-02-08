
#include <iostream>
#include <string> //stoi
//#include "functions1.h"
#include "functions1.cpp"  //it contains template class

//#include "runner.h"
#include "runner.cpp" //it contains template class

//#include "genetic_algorithm.h"
#include "genetic_algorithm.cpp" // contains template class

#include "population.h"
#include "../lib/debug.h"
//#include "differential_evolution.h"
#include "differential_evolution.cpp"

/// @brief  Run project 1, different benchmark functions 
///
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
