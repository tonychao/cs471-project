#ifndef _POPULATION_H_
#define _POPULATION_H_

#include "../lib/mt19937ar_class.h"
#include <time.h> 
#include <iostream>
#include "util.h"


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
    Population(int n_items, int dimension);
    ~Population();
    void fillWithRandom(Tinput range_low, Tinput range_high);
    void printPopulation();
    Tinput getData(int i, int j);
    Tinput* getData(int i);
    void setData(Tinput *item, int i);
    static void swap(Population& A, Population& B);

};

#endif