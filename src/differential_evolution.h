#ifndef _DIFFERENTIALEVOLUTION_H_
#define _DIFFERENTIALEVOLUTION_H_

#include "population.h"
#include "../lib/debug.h"
#include "../lib/mt19937ar_class.h"
#include "util.h"

template <class Tinput>
struct DEInputParameter
{
    int pop_size; ///< population size
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

    double scale_f;
    double scale_lambda;
    
};

template <class Tinput, class Toutput>
class DifferentialEvolution
{

    private:
    PopulationBenchmark<Tinput, Toutput> *actual_pop;
    PopulationBenchmark<Tinput, Toutput> *new_pop;
    DEInputParameter<Tinput> param;
    MersenneTwister ms_random; ///< mersenne twister random generator
    void randomR(int *r, int n, int current_i);
  
    public:
    DifferentialEvolution(DEInputParameter<Tinput> param);
    ~DifferentialEvolution();
    void runS7_DE_rand_1_bin();
};

#endif