
#include <iostream>
#include <string> //stoi
#include "functions1.h"
#include "functions1.cpp"  //it contains template class

#include "runner.h"
#include "runner.cpp" //it contains template class

#include "genetic_algorithm.h"
#include "genetic_algorithm.cpp" // contains template class

#include "population.h"
#include "../lib/debug.h"


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
    std::string opt_conf_file, opt_result_file;
    //printf("Program Name Is: %s\n",argv[0]); 
    if(argc!=9) 
    {
        printf("arguments: opt_alg_id opt_conf_file opt_result_file n_runs function_id dimensions range_min range_max\n");
        return 1;
    }
    else
    { 
        //printf("Number Of Arguments Passed: %d\n",argc); 
        //printf("----Following Are The Command Line Arguments Passed----\n"); 
        //for(counter=0;counter<argc;counter++) 
        //    printf("argv[%d]: %s\n",counter,argv[counter]);
        opt_alg_id = std::stoi(argv[1]);
        opt_conf_file = argv[2];
        opt_result_file = argv[3];
        n_runs = std::stoi(argv[4]);
        function_id = std::stoi(argv[5]);
        dimensions = std::stoi(argv[6]);
        range_min = std::stod(argv[7]);
        range_max = std::stod(argv[8]);
    } 



    //Runner <float,float> runner(dimensions,n_runs); // <Tinput, Toutput>, dimensions, n_sample
    //runner.runOptimization(opt_alg_id, opt_conf_file, opt_result_file, function_id,range_min, range_max);


    // in preparation
    PopulationBenchmark<float, float> actual_pop(5,dimensions);
    actual_pop.fillWithRandom(-10,10);
    debug1(actual_pop.printPopulation());
    actual_pop.evaluateCost(function_id);
    debug1(actual_pop.printCost());
    actual_pop.sortIndexByCostAsc();
    debug1(actual_pop.printIndex());

    return 0;
}
