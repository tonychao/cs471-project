#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_

#include "../lib/mt19937ar_class.h"
#include "debug.h"
#include <iostream>
#include "functions1.h"
#include <time.h> 
#include <algorithm>    //std::sort
#include <fstream>  //file
#include "util.h"
#include "population_benchmark.h"

/// @brief Template Genetic Algorithm Input parameter Struct
///
/// Struct to where all the GA parameters are stored
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-01
///
template <class Tinput>
struct GAInputParameter
{
    int ns; ///< number of solutions
    int dim; ///< problem dimension
    
    /// @brief problem bounds
    struct Bounds
    {
        Tinput u; ///< upper bound
        Tinput l; ///< lower bound
    };
    Bounds bounds; ///< instance of struct #Bounds
    
    int t_max; ///< maximum number of iterations
    double cr; ///< crossover rate
    
    /// @brief Mutation parameters
    struct Mutation
    {
        double rate;
        double range;
        double precision;
    };
    Mutation m; ///< instance of struct #Mutation

    double er; ///< elistism rate

};

/// @brief Template Genetic Algorithm (GA) class
///
/// Genetic Algorithms (GA) are the heuristic search and optimization techniques that mimic the process of natural evolution.  
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-01
///
template <class Tinput, class Toutput>
class GeneticAlgorithm
{
    private:

    PopulationBenchmark<Tinput, Toutput> *population; ///< pointer of population
    PopulationBenchmark<Tinput, Toutput> *new_population; ///< pointer of new population
    GAInputParameter<Tinput> parameters; ///< parameters for genetic algorithm
    MersenneTwister ms_random_generator; ///< mersenne twister random generator

    void saveResult(Toutput best_cost, Tinput *best_individuo, std::string result_file);
    void select(Tinput* parent1, Tinput* parent2); 
    void selectParent(Tinput* parent); 
    void crossover(Tinput* parent1, Tinput* parent2, double cr); 
    void reduce(int elite_sn, Toutput& best_cost, Tinput* best_individuo);
    void mutate(Tinput* individuo); 
    void keepInRange(Tinput& element);
    
    public:

    GeneticAlgorithm( GAInputParameter<Tinput> parameters);
    ~GeneticAlgorithm();
    Toutput findBestSolution(int function_id, Tinput range_low, Tinput range_high);

    
};

#endif