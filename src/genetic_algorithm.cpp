#include "genetic_algorithm.h"


/// @brief constructor of the class GeneticAlgorithm
/// 
/// print the parameters and allocate memories
/// @param parameters configuration parameters for GA
///
template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::GeneticAlgorithm(GAInputParameter<Tinput> parameters)
{
    this->parameters = parameters;

    // allocate memory for population
    this->population = new PopulationBenchmark<Tinput, Toutput>(parameters.ns, parameters.dim);

    //allocate memory for new population
    this->new_population = new PopulationBenchmark<Tinput, Toutput>(parameters.ns, parameters.dim);


    
    std::cout << "bounds.lower: " << parameters.bounds.l << std::endl;
    std::cout << "bounds.uppper: " << parameters.bounds.u << std::endl;
    std::cout << "crossover rate: " << parameters.cr << std::endl;
    std::cout << "dimension: " << parameters.dim << std::endl;
    std::cout << "elitism rate: " << parameters.er << std::endl;
    std::cout << "mutation.rate: " << parameters.m.rate << std::endl;
    std::cout << "mutation.range: " << parameters.m.range << std::endl;
    std::cout << "mutation.precision: " << parameters.m.precision << std::endl;
    std::cout << "number of solutions: " << parameters.ns << std::endl;
    std::cout << "maximum number of iterations: " << parameters.t_max << std::endl;
    
    // random generator seed
    ms_random_generator.init_genrand(time(0));
    
}

/// @brief desstructor of the class GeneticAlgorithm
///
template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::~GeneticAlgorithm()
{
    // free memory for population
    if(population)
    {
        delete population; // delete
    }

    // free memory for new population
    if(new_population)
    {
        delete new_population; // delete
    }
    
}

/// @brief ellistism part, select the best inviduos of the old population and put into the new population, at the end the population and new_population are swapped
/// 
/// in this method, also the best_cost and best_individuo is located
/// @param elite_sn number of best inviduals which will be put into the new population
/// @param &best_cost output by reference, to get the individuo with best cost of the new population and the old population
/// @param *best_individuo pointer to best individuo which is an array
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::reduce(int elite_sn, Toutput& best_cost, Tinput* best_individuo)
{

        population->sortIndexByCostAsc();
        new_population->sortIndexByCostAsc();

        debug(population->printCost());
        debug(new_population->printCost());
        
        //print sorted index

        debug(std::cout<< "---population sorted index---" <<std::endl);
        debug(population->printIndex());
        debug(std::cout<< "---new population sorted index---" <<std::endl);
        debug(new_population->printIndex());
        
        // ---------find the best------------
    
        if(population->getMinCost() < new_population->getMinCost())
        {
            best_cost = population->getMinCost();
            
            std::copy(population->getMinCostData(), population->getMinCostData()+parameters.dim,best_individuo);
        }
        else
        {
            best_cost = new_population->getMinCost();
            std::copy(new_population->getMinCostData(), new_population->getMinCostData()+parameters.dim,best_individuo);
        }
        
        debug(std::cout<<"best: " << best_cost << std::endl);
        

        //replace first elite_sn worst solutions in new population by best solutions in population
        for (int i =0; i<elite_sn; i++)
        {
            int pop_index = population->getAscIndex(i); //get index of the best
            int new_pop_index = new_population->getAscIndex(parameters.ns-1-i); //get index of the worst
            new_population->setData(population->getData(pop_index),new_pop_index);
        }

        // swap population data 

        PopulationBenchmark<Tinput, Toutput>::swap(*population, *new_population);


}

/// @brief Find the best individuo of the population
///
/// @param function_id the cost will be calculated by on the benchmarking function id
/// @param range_low lower bound value of the data
/// @param range_high upper bound value of the data 
/// @return return the best cost (evaluate the function)
///
template <class Tinput, class Toutput>
Toutput GeneticAlgorithm<Tinput, Toutput>::findBestSolution(int function_id, Tinput range_low, Tinput range_high)
{
    int elite_sn = (int) (parameters.er * parameters.ns);
    population->fillWithRandom(range_low, range_high);
    population->evaluateCost(function_id);
    //print to see
    debug(population->printCost());


    population->evaluateFitness();

    debug(population->printFitness());
    debug(std::cout<<"total fitness: " << population->getTotalFitness() << std::endl);

    //allocate memory for new population
    // allocateMemoryNewPopulation();

    // create parent1 and parent2
    Tinput parent1[parameters.dim];
    Tinput parent2[parameters.dim];
    

    Toutput best_cost;
    Tinput best_individuo[parameters.dim];

    for (int t=0; t<parameters.t_max; t++) // GA iteration
    {
        for (int i=0; i<parameters.ns; i+=2) 
        {

            //select parents by roulette wheel selection
            select(parent1, parent2); // pass parents by reference
            
            crossover(parent1,parent2, parameters.cr);
            mutate(parent1);
            mutate(parent2);

            //add parents to the new population
            new_population->setData(parent1, i);
            
            if((i+1)<parameters.ns) //problem: i+=2 will go outside of the range... fix
            new_population->setData(parent2, i+1);
            
        }
 
        debug(new_population->printPopulation());
        new_population->evaluateCost(function_id);
        debug(new_population->printCost());
        
        

        // caution: the swap of address of population can cause memory leak and double free problem
        reduce(elite_sn, best_cost, best_individuo); //also can find the best  solution in constant time, the arrays are already sorted

        debugfile(saveResult(best_cost, best_individuo, "ga_f"+std::to_string(function_id)+"_iterations.csv"));
        //debug(printInputPopulation());

        
        population->evaluateCost(function_id);
        //debug(printCost(cost));
        
        population->evaluateFitness();

        
    }


    return best_cost;
}

/// @brief save the indivuo with best cost in a file 
/// 
/// this method is used to save the best cost of each iteration, the saved data are:
/// the cost and the n dimensions elements of the individuo.
/// @param best_cost ///< best cost
/// @param *best_vector ///< pointer to the best individuo
/// @param result_file ///< name of the file to be saved
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>:: saveResult(Toutput best_cost, Tinput* best_individuo, std::string result_file)
{
    // file pointer 
    std::fstream fout; 
  
    // opens an existing csv file or creates a new file. 
    std::string file_name = result_file;
    fout.open(file_name, std::ios::out | std::ios::app); 
  
    fout << best_cost << "," ;

    for(int i =0; i<parameters.dim; i++)
    {
        fout << best_individuo[i] <<",";
    }
    fout<<"\n";

    fout.close();
}



/// @brief crossover between 2 parents
/// 
/// @param *parent1 ///< pointer to the parent1
/// @param *parent2 ///< pointer to the parent2
/// @param cr ///< cross over rate
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::crossover(Tinput* parent1, Tinput* parent2, double cr)
{
    if (ms_random_generator.genrand_real_range(0,1) < cr) //crossover only when the criteria meet
    {
        int d = (int) ms_random_generator.genrand_real_range_ex_high(0,parameters.dim); //cast to int
        debug(std::cout << "random int: " << d << std::endl);
        Tinput temp1[parameters.dim];
        // copy parent1 to temp1
        std::copy(parent1, parent1+parameters.dim, temp1);
        //crossover
        std::copy(parent2+d, parent2+parameters.dim, parent1+d); // copy 2nd part of parent2 to parent1
        std::copy(temp1+d, temp1+parameters.dim, parent2+d); // copyt 2nd part of parent1 (stored in temp1) to parent2
        
        debug(std::cout << "---after crossover---" << std::endl);
        debug(printArray<Tinput>(parent1, parameters.dim, ' '));
        debug(printArray<Tinput>(parent2, parameters.dim, ' '));

    }
}

/// @brief keep the element of the vector inside the search range 
/// 
/// the method trucante the element to the upper and lower bound of the range, when the element excess these limits.
/// @param &element ///< element of the vector
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::keepInRange(Tinput& element)
{
    if (element > parameters.bounds.u)
    {
        element = parameters.bounds.u;
    }
    else if(element < parameters.bounds.l)
    {
        element = parameters.bounds.l;
    }
    
}

/// @brief mutation of an individuo
/// 
/// @param *individuo pointer of an individuo
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::mutate(Tinput* individuo)
{
    for (int i = 0; i<parameters.dim; i++)
    {   
        if(ms_random_generator.genrand_real_range(0,1)<parameters.m.rate)
        {
            individuo[i] += ms_random_generator.genrand_real_range(-1,1)*(parameters.bounds.u - parameters.bounds.l)*\
                            parameters.m.range*pow(2, -1*ms_random_generator.genrand_real_range(0,1)*parameters.m.precision);
            keepInRange(individuo[i]);                 

        }

    }

    debug(std::cout << "---after mutation---" << std::endl);
    debug(printArray<Tinput>(individuo,parameters.dim,' '));

    

}


/// @brief select 2 parents by roulette wheel selection
///
/// @param *parent1 pointer to the parent1
/// @param *parent2 pointer to the parent2
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::select(Tinput *parent1, Tinput* parent2)
{
    selectParent(parent1);
    selectParent(parent2);
}

/// @brief select 1 parent
///
/// @param *pointer to the parent
///
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::selectParent(Tinput* parent)
{
    // changed to 0, because the total fitness can be less than 1, from the document (1,total_fitness)
    Toutput random_number = ms_random_generator.genrand_real_range(0,population->getTotalFitness()); 
    debug(std::cout << "random fitness: " << random_number << std::endl);
    int s=0;
    while (s < parameters.ns && random_number > 0)
    {
        random_number -= population->getFitness(s);
        s++;
    }
    //s-1 = selected element;
    // copy the individuo to parent
    std::copy(population->getData(s-1), population->getData(s-1)+parameters.dim, parent);


    debug(std::cout << "selected parent: " << (s-1) << std::endl);
    debug(printArray(parent, parameters.dim, ' '));

    
}











