#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_


template <class Tinput>
struct GAInputParameter
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
    double cr; ///< crossover rate
    
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

/// @brief Template Genetic Algorithm (GA) class
///
/// Genetic Algorithms (GA) are the heuristic search and optimization techniques that mimic the process of natural evolution.  
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-02-01

template <class Tinput, class Toutput>
class GeneticAlgorithm
{


    private:
    Tinput** population; ///< pointer to arrray of pointers
    GAInputParameter<Tinput> parameters;
    Toutput* cost;  ///< array that contains the cost of each individuo
    Toutput* fitness; ///< array that contains the fitness of each individuo
    Toutput total_fitness; ///< total fitness of the population
    Tinput** new_population;
    void allocateMemoryNewPopulation();
    void freeMemoryNewPopulation();

    void evaluateCost(int function_id); ///< calculate the cost
    void getFitness(); ///< calculate fitness
    void select(Tinput &parent1, Tinput &parent2); ///< select 2 parents by roulette wheel selection
    void selectParent(Tinput &parent); ///< select 1 parent
    
    void reduce();

    void mutate();
    void crossover();
    
    
    public:

    /// @brief constructor of the class GeneticAlgorithm
    /// 
	/// @param population ///< get the population allocated in the runner
	/// @param parameters ///< get the configuration parameters for GA
    GeneticAlgorithm(Tinput** population, GAInputParameter<Tinput> parameters);
    ~GeneticAlgorithm();

    /// @brief Find the best individuo of the population
    /// @return pointer to the array of the best individuo (each element of the array represents each dimension)
    Tinput* findBestSolution(int function_id);

    void printPopulation();
    void printCost();
    void printFitness();
    


};

#endif