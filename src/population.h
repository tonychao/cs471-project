#ifndef _POPULATION_H_
#define _POPULATION_H_

#include "../lib/mt19937ar_class.h"
#include <time.h> 
#include <iostream>
#include "util.h"

/// @brief Template Population Class
///
/// The population class in used to store the population which is useful for GA and DE
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-11
///
template <class Tinput, class Toutput>
class Population
{
    private:
    MersenneTwister random_ms; ///< mersenne twister random generator
    
    protected:
    Tinput** data; ///< pointer to the array of array
    int dimension; ///< dimesion or size of the 2nd array
    int n_items; ///< total number of items in data or size of the 1st array

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