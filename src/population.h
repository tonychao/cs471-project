#ifndef _POPULATION_H_
#define _POPULATION_H_

#include "../lib/mt19937ar_class.h"
#include <time.h>       //time
#include <iostream>
#include "functions1.h"


template <class Tinput, class Toutput>
class Population
{
    private:
    MersenneTwister random_ms; ///< mersenne twister random generator
    
    protected:
    Tinput** data;
    int dimension;
    int n_items; ///< total number of items in data

    public:
    Population(int n_items, int dimension)
    {
        this->n_items = n_items;
        this->dimension = dimension;
        
        // allocate memory for the data
        data = new Tinput*[n_items];

        for (int i = 0; i<n_items; i++)
        {
            data[i] = new Tinput[dimension];
        }

        // random generator seed
        random_ms.init_genrand(time(0));
    };
    ~Population()
    {
        // free memory for data
        if(data)
        {
            for (int i = 0; i<n_items; i++)
            {
                if(data[i])
                delete [] data[i]; //delete array
            }

            delete [] data; // delete array of array
        }
    }

    void fillWithRandom(Tinput range_low, Tinput range_high)
    {       
        for (int i = 0; i < n_items; i++) 
        {  
            for (int j = 0; j < dimension; j++) 
            {
                 data[i][j] = random_ms.genrand_real_range(range_low,range_high);
            
            }
            
        }
    };

    void printPopulation()
    {
        std::cout<< "---population---" <<std::endl;
        for (int i = 0; i < n_items; i++) 
        {  
            for (int j = 0; j < dimension; j++) 
            {
                std::cout<< data[i][j] << " ";
            }
            std::cout<<std::endl;
        }
    };


    

};

template <class Tinput, class Toutput>
class PopulationBenchmark: public Population <Tinput,Toutput>
{
    private:
    int dimension;
    int n_items; ///< total number of items in data
    Toutput* cost;  ///< array that contains the cost of each item
    Toutput* fitness; ///< array that contains the fitness of each item
    Toutput total_fitness; ///< total fitness of the population
    int* asc_index;

    public:
    PopulationBenchmark(int n_items, int dimension) : Population<Tinput,Toutput>(n_items,dimension)
    {
        
        // allocate memory for array for cost and fitenss
        cost = new Toutput[n_items];
        fitness = new Toutput[n_items];

        // allocate memory and fill the population index for sorting
        asc_index = new int[n_items];
        for(int i=0; i<n_items; i++)
        {
            asc_index[i] = i;
        }

    }

    ~PopulationBenchmark()
    {
         // free array of cost
        if(cost)
            delete[] cost;
        // free array of fitness
        if(fitness)
            delete[] fitness;
        // free array of index
        if(asc_index)
            delete[] asc_index;
    
    };
    void evaluateCost(int function_id)
    {
        Functions1<Tinput,Toutput> functions;
        typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
        for (int i = 0; i<n_items; i++)
        {
            cost[i] = (functions.*fp)(this->data[i], dimension); //evaluate the selected benchmark function
        }
    
    };



    void sortIndexByCostAsc();
    
    void saveBest(Toutput best_cost, Tinput *best_individuo);
    void printCost();
    void printFitness();
};
#endif