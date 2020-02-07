#include "differential_evolution.h"


template <class Tinput, class Toutput>
DifferentialEvolution<Tinput, Toutput>::DifferentialEvolution(DEInputParameter<Tinput> param)
{
    this->param = param;
    //print parameters

    std::cout << "bounds.lower: " << param.bounds.l << std::endl;
    std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
    std::cout << "crossover rate: " << param.cr << std::endl;
    std::cout << "dimension: " << param.dim << std::endl;
    std::cout << "scale f: " << param.scale_f << std::endl;
    std::cout << "scale lambda: " << param.scale_lambda << std::endl;
    std::cout << "population size: " << param.pop_size<< std::endl;
    std::cout << "maximum number of iterations: " << param.t_max << std::endl;
    
    // allocate memory for population
    actual_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);
    new_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);
}

template <class Tinput, class Toutput>
DifferentialEvolution<Tinput, Toutput>::~DifferentialEvolution()
{

    if(actual_pop)
    {
        delete actual_pop;
    }

    if(new_pop)
    {
        delete new_pop;
    }
}
template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>::runS7_DE_rand_1_bin()
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug1(actual_pop->printPopulation());

}