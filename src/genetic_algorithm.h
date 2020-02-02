#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_

/// @brief Template Genetic Algorithm (GA) class
///
/// Genetic Algorithms (GA) are the heuristic search and optimization techniques that mimic the process of natural evolution.  
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-01


template <class Tinput>
class GeneticAlgorithm
{
    private:
    Tinput** population; ///< pointer to arrray of pointers
    void reduce();
    void select();
    void selectParent();
    void getFitness();
    void mutate();
    void crossover();

    public:
    struct InputParameter
    {
        int ns; ///< number of solutions
        int dim; ///< problem dimension
        
        /// @brief problem bounds
        struct Bounds
        {
            Tinput u; ///< upper bound
            Tinput l; ///< lower bound
        };
        Bounds bounds; ///< instance of struct #Bounds
        
        int t_max; ///< maximum number of iterations
        
        /// @brief Mutation parameters
        struct Mutation
        {
            double rate;
            double range;
            double precision;
        };
        Mutation m; ///< instance of struct #Mutation

        double er; ///< elistism rate
    
        
    };
    
    /// @brief constructor of the class GeneticAlgorithm
    /// 
	/// @param population ///< get the population allocated in the runner
	/// @param parameters ///< get the configuration parameters for GA
    GeneticAlgorithm(Tinput** population, InputParameter parameters);

    /// @brief Find the best individuo of the population
    /// @return pointer to the array of the best individuo (each element of the array represents each dimension)
    Tinput* findBestSolution();

};

#endif