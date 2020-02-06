#ifndef _RUNNER_H_
#define _RUNNER_H_


#include "functions1.h"
#include "genetic_algorithm.h"
#include <string>
#include "../lib/mt19937ar_class.h"
#include "../lib/debug.h"

/// @brief  Template class which generates random sample of different dimensions and run a specific benchmark function
///
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-01-31

template <class Tinput, class Toutput>
class Runner
{
    private:
    /// @brief struct of statistical analysis
    struct statistics 
    {
        double mean;
        double std_dev;
        double range;
        double median;
        double time_ms;
        double range_min;
        double range_max;
    };
    statistics stat_analysis; ///< instance of struct #statistics [result of computeStatistic()]
    Tinput** vectors; ///< vectors is an array of pointers to the array of data type Tinput that it is named sample
    int dimensions; ///< size of the sample or size of the array pointed by each element of vectors [parameter from constructor Runner()]
    int n_samples; ///< size of the array vectors [parameter from constructor Runner()]
    Tinput range_low; ///< lowest value that the random generator should produce for each element of the sample [parameter from run()]
    Tinput range_high; ///< highgest value that the random generator should produce  for each element of the sample [parameter from run()]
    Toutput* solutions; ///< pointer to the array of the result of the benchmark function
    Functions1 <Tinput, Toutput> functions; ///< object of class Functions1 which contains the benchmark functions
    MersenneTwister mersenne_twister; ///< mersenne twister random generator

    int function_id; ///< id of the function that will be run [parameter from run()]
    
    /// @brief fill vectors of array with mersenne twister random generator
	/// @param range_low ///< lowest value that the random generator should produce for each element of the sample
	/// @param range_high ///< highgest value that the random generator should produce for each element of the sample
    void fillVectorsRandom(Tinput range_low, Tinput range_high);

    /// @brief compute the statistical result of #solutions and save it in #stat_analysis
	/// @param time_ms ///< running time in ms
    void computeStatistic(double time_ms);

    /// @brief save statistical result in csv file
    void saveStatistic();

    // to do: create instance of each optimization algorithm that will be run
    // create read configuration parameter for each optimization algorithm
    GeneticAlgorithm<Tinput, Toutput>* genetic_algorithm;
    void fillGAParameterFromFile(std::string config_filename, GAInputParameter<Tinput> &parameters); //param = address of the configuration struct

    public:
    
    /// @brief constructor of the class Runner
    /// allocate memory for vectors (array of array) 
	/// @param dimensions ///< size of the sample or size of the array pointed by each element of vectors
	/// @param n_samples ///< size of the array vectors 
    Runner(int dimensions, int n_samples); //constructorinput

    /// @brief destructor of the class Runner
    /// free memory for vectors (array of array) 
    ~Runner(); //destructor

    /// @brief run the function specified by #function_id
    /// first it generates random numbers with fillVectorsRandom(), then it calls computeStatistic() and saveStatistic()
	/// @param function_id ///< id of the function that will be run
	/// @param range_low ///< lowest value that the random generator should produce for each element of the sample
    /// @param range_high; ///< highgest value that the random generator should produce for each element of the sample
    void run(int function_id,Tinput range_low, Tinput range_high);

    void printVectors();

    void runOptimization(int algorithm_id, std::string config_file, int function_id, Tinput range_low, Tinput range_high);

};

#endif