#ifndef _POPULATIONBENCHMARK_H_
#define _POPULATIONBENCHMARK_H_

#include "population.cpp" // template class
#include "functions1.h"
#include <algorithm> //sort

template <class Tinput, class Toutput>
class PopulationBenchmark: public Population <Tinput,Toutput>
{
    private:

    Toutput* cost;  ///< array that contains the cost of each item
    Toutput* fitness;
    Toutput total_fitness;
    Toutput min_cost;
    int min_cost_i;
    int* asc_index;
    struct doCompare; //<< to use std::sort with class function member

    public:

    PopulationBenchmark(int n_items, int dimension);
    ~PopulationBenchmark();
    void evaluateCost(int function_id);
    void evaluateFitness();
    Toutput getTotalFitness()
    {
        return total_fitness;
    }
    Toutput getFitness(int i)
    {
        return fitness[i];
    }
    int getAscIndex(int i)
    {
        return asc_index[i];
    }
    Toutput calcCost1Item(int function_id, int i);
    static Toutput calcCostExt(int function_id, Tinput *item, int dim);
    void sortIndexByCostAsc();
    void saveBest(Toutput best_cost, Tinput *best_individuo);
    void printCost();
    void printFitness()
    {
        std::cout<<"--- fitness ---"<<std::endl;
        printArray<Toutput>(fitness, this->n_items, '\n');
    }
    void printIndex();
    void setDataAndCost(Tinput *item, int i, Toutput c);
    void calculateMinCost();
    Toutput getMinCost();
    Tinput* getMinCostData();
    static void swap(PopulationBenchmark& A, PopulationBenchmark& B);
 
};


#endif