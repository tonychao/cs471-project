#include "genetic_algorithm.h"

template <class Tinput>
GeneticAlgorithm<Tinput>::GeneticAlgorithm(Tinput** population, GAInputParameter<Tinput> parameters)
{
    this->population = population;
    this->parameters = parameters;

}