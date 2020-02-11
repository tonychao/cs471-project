#ifndef _DIFFERENTIALEVOLUTION_H_
#define _DIFFERENTIALEVOLUTION_H_

#include "functions1.h"
#include "population_benchmark.cpp" //template class
#include "debug.h"
#include "../lib/mt19937ar_class.h"
#include "util.h"
#include <time.h> 
#include <fstream>  //file

/// @brief Template Differential Evolution Input parameter Struct
///
/// Struct to where all the DE parameters are stored
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-08
///
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

    double scale_f; ///< scale factor F
    double scale_lambda; ///< scale factor lambda
    
};

/// @brief Template Differential Evolution class
///
/// Differential Evolution (DE) is based on the strategy that employs the difference of tow randomly
/// selected parameter vectors as the source of random variations for a third parameter vector.  
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-08
///
template <class Tinput, class Toutput>
class DifferentialEvolution
{

    private:
    PopulationBenchmark<Tinput, Toutput> *actual_pop; ///< actual population
    PopulationBenchmark<Tinput, Toutput> *new_pop; ///< new population
    DEInputParameter<Tinput> param; ///< DE parameters 
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput *best_cost; ///< array with history of best cost during the generations
    void randomR(int *r, int n, int current_i);
    void saveResult(Toutput best_cost, Tinput* best_vector, std::string result_file);
    void keepInRange(Tinput& element);

    public:
    DifferentialEvolution(DEInputParameter<Tinput> param);
    ~DifferentialEvolution();
    Toutput runS7_DE_rand_1_bin(int function_id);
    Toutput runS10_DE_rand_2_bin(int function_id);
    Toutput runS6_DE_best_1_bin(int function_id);
    Toutput runS9_DE_best_2_bin(int function_id);
    Toutput runS8_DE_randbest_1_bin(int function_id);
    Toutput runS1_DE_best_1_exp(int function_id);
    Toutput runS2_DE_rand_1_exp(int function_id);
    Toutput runS3_DE_randbest_1_exp(int function_id);
    Toutput runS4_DE_best_2_exp(int function_id);
    Toutput runS5_DE_rand_2_exp(int function_id);
    
    

};

#endif