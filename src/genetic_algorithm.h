#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_

#include "../lib/mt19937ar_class.h"
#include "../lib/debug.h"
#include <iostream>
#include "functions1.h"
#include <time.h> 
#include <algorithm>    //std::sort
#include <fstream>  //file

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

template <class Tinput, class Toutput>
class GeneticAlgorithm
{


    private:
    Tinput** population; ///< pointer to arrray of pointers
    GAInputParameter<Tinput> parameters;
    Toutput* cost;  ///< array that contains the cost of each individuo
    Toutput* new_population_cost; ///< cost of the new population
    Toutput* fitness; ///< array that contains the fitness of each individuo
    Toutput total_fitness; ///< total fitness of the population
    Tinput** new_population;



    //todo: create population class
    int* population_asc_index;
    int* new_population_asc_index;

   

    void evaluateCost(int function_id, Tinput** pop, Toutput* cost); ///< calculate the cost
    void getFitness(); ///< calculate fitness
    void select(Tinput* parent1, Tinput* parent2); ///< select 2 parents by roulette wheel selection
    void selectParent(Tinput* parent); ///< select 1 parent
    MersenneTwister ms_random_generator; ///< mersenne twister random generator
    void crossover(Tinput* parent1, Tinput* parent2, double cr); ///< crossover
    void reduce(int elite_sn, Toutput& best_cost, Tinput* best_individuo);
    void mutate(Tinput* individuo); 
    
    void printPopulation(Tinput** pop);
    

    // ... for sort
    //https://stackoverflow.com/questions/1902311/problem-sorting-using-member-function-as-comparator
    struct doCompare
    { 
        const GeneticAlgorithm& m_info;
        doCompare( const GeneticAlgorithm& info ) : m_info(info) { } // only if you really need the object state

        bool operator()( const int & i1, const int & i2  )
        { 
                // comparison code using m_info
                return (m_info.cost[i1]<m_info.cost[i2]);
        }
    };
    struct doCompareNewPop
    { 
        const GeneticAlgorithm& m_info;
        doCompareNewPop( const GeneticAlgorithm& info ) : m_info(info) { } // only if you really need the object state

        bool operator()( const int & i1, const int & i2  )
        { 
                // comparison code using m_info
                return (m_info.new_population_cost[i1]<m_info.new_population_cost[i2]);
        }
    };

    //todo create a population class insted of these 2 functions
    void sortPopulationByIndexAsc();
    void sortNewPopulationByIndexAsc();

    void saveResult(Toutput best_cost, Tinput *best_individuo, std::string result_file);

    void randomInit(Tinput range_low, Tinput range_high);
    
    public:

    /// @brief constructor of the class GeneticAlgorithm
    /// 
	/// @param population ///< get the population allocated in the runner
	/// @param parameters ///< get the configuration parameters for GA
    GeneticAlgorithm( GAInputParameter<Tinput> parameters);
    ~GeneticAlgorithm();

    /// @brief Find the best individuo of the population
    /// @return pointer to the array of the best individuo (each element of the array represents each dimension)
    Tinput* findBestSolution(int function_id, std::string result_file, Tinput range_low, Tinput range_high);

    //bool operator() (int i,int j);
    template <class T>
    void printArray(T* array, int n, char separtor);
    void printInputPopulation();
    void printNewPopulation();

    void printCost(Toutput* cost);
    void printFitness();
    


};

#endif