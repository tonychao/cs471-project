#ifndef _POPULATIONBENCHMARK_H_
#define _POPULATIONBENCHMARK_H_

#include "population.cpp" // template class
#include "functions1.h"
#include <algorithm> //sort


/// @brief Template PopulationBenchmark Class
///
/// The PopulationBenchmark class inherit all features of Population base class, PopulationBenchmark has new features for cost and fitness evaluation
/// this class is useful for different population based optimization algorithms
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-11
///
template <class Tinput, class Toutput>
class PopulationBenchmark: public Population <Tinput,Toutput>
{
    private:

    Toutput* cost;  ///< array that contains the cost of each item
    Toutput* fitness; ///< array that contains the fitness of each item
    Toutput total_fitness; ///< sum of the array fitness
    Toutput min_cost; ///< minimum cost of the array cost
    int min_cost_i; ///< index of the mininum cost
    int* asc_index; ///< result of ascending sort expressed as index
    struct doCompare; //<< used for sorting function (std::sort) with class function member

    public:

    PopulationBenchmark(int n_items, int dimension);
    ~PopulationBenchmark();
    void evaluateCost(int function_id);
    void evaluateFitness();
    Toutput getTotalFitness();
    Toutput getFitness(int i);
    int getAscIndex(int i);
    Toutput calcCost1Item(int function_id, int i);
    static Toutput calcCostExt(int function_id, Tinput *item, int dim);
    void sortIndexByCostAsc();
    void saveBest(Toutput best_cost, Tinput *best_individuo);
    void printCost();
    void printFitness();
    void printIndex();
    void setDataAndCost(Tinput *item, int i, Toutput c);
    void calculateMinCost();
    Toutput getMinCost();
    Tinput* getMinCostData();
    static void swap(PopulationBenchmark& A, PopulationBenchmark& B);
    Toutput getCost(int i);
     
 
};


#endif