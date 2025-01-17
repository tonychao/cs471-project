#include "genetic_algorithm.h"


/* pass population by reference to avoid the problem of:
 double free or corruption (fasttop)
 Aborted (core dumped)

this occurs because to increase the velocity of swap, we did copy the data, we just swap the address,
the problem was the population in runner class still have the old address which cause the double delete and memory leak problem.
*/

template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::GeneticAlgorithm(GAInputParameter<Tinput> parameters)
{
    // allocate memory for population
    this->population = new Tinput*[parameters.ns];

    for (int i = 0; i<parameters.ns; i++)
    {
        this->population[i] = new Tinput[parameters.dim];
    }
   
    
    this->parameters = parameters;

    //allocate memory for new population
    new_population = new Tinput*[parameters.ns];

    for (int i = 0; i<parameters.ns; i++)
    {
        new_population[i] = new Tinput[parameters.dim];
    }

    //allocate memory for array of cost
    cost =  new Toutput[parameters.ns];
    fitness = new Toutput[parameters.ns];
    new_population_cost = new Toutput[parameters.ns];
    
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
    

    // allocate memory and fill the population index for sorting

    population_asc_index = new int[parameters.ns];
    new_population_asc_index = new int[parameters.ns];
    for(int i=0; i<parameters.ns; i++)
    {
        population_asc_index[i] = i;
        new_population_asc_index[i] = i;
    }
}

template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::~GeneticAlgorithm()
{
    // free array of cost
    if(cost)
        delete[] cost;
    
    // free array of fitness
    if(fitness)
        delete[] fitness;
    
    // free array of new population cost
    if(new_population_cost)
        delete[] new_population_cost;

    // free array
    if(population_asc_index)
        delete[] population_asc_index;
    
    if(new_population_asc_index)
        delete[] new_population_asc_index;

    // free memory for population
    if(population)
    {
        for (int i = 0; i<parameters.ns; i++)
        {
            if(population[i])
            delete [] population[i]; //delete array
        }

        delete [] population; // delete array of array
    }

    // free memory for new population
    if(new_population)
    {
        for (int i = 0; i<parameters.ns; i++)
        {
            if(new_population[i])
            delete [] new_population[i]; //delete array
        }

        delete [] new_population; // delete array of array
    }
    
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::reduce(int elite_sn, Toutput& best_cost, Tinput* best_individuo)
{
        sortPopulationByIndexAsc();
        sortNewPopulationByIndexAsc();
        //print sorted index

        debug(std::cout<< "---population sorted index---" <<std::endl);
        debug(printArray<int>(population_asc_index, parameters.ns, '\n'));
        debug(std::cout<< "---new population sorted index---" <<std::endl);
        debug(printArray<int>(new_population_asc_index, parameters.ns, '\n'));
        
        // ---------find the best------------
    
        if(cost[population_asc_index[0]] < new_population_cost[new_population_asc_index[0]])
        {
            best_cost = cost[population_asc_index[0]];
            
            std::copy(population[population_asc_index[0]], population[population_asc_index[0]]+parameters.dim,best_individuo);
        }
        else
        {
            best_cost = new_population_cost[new_population_asc_index[0]];
            std::copy(new_population[new_population_asc_index[0]], new_population[new_population_asc_index[0]]+parameters.dim,best_individuo);
        }
        
        debug(std::cout<<"best: " << best_cost << std::endl);
        



        //replace first elite_sn worst solutions in new population by best solutions in population
        for (int i =0; i<elite_sn; i++)
        {
            int pop_index = population_asc_index[i]; //get index of the best
            int new_pop_index = new_population_asc_index[parameters.ns-1-i]; //get index of the worst
            std::copy(population[pop_index], population[pop_index]+parameters.dim, new_population[new_pop_index]);
        }

        // swap population data 
        Tinput** temp;
        temp = new_population;
        new_population = population;
        population = temp;


}

template <class Tinput, class Toutput>
Toutput GeneticAlgorithm<Tinput, Toutput>::findBestSolution(int function_id, Tinput range_low, Tinput range_high)
{
    int elite_sn = (int) (parameters.er * parameters.ns);
    randomInit(range_low, range_high); // population random Init
    evaluateCost(function_id,population,cost); // calculate the cost of each individuo
    //print to see
    debug(printCost(cost));


    getFitness();

    debug(printFitness());
    debug(std::cout<<"total fitness: " << total_fitness << std::endl);

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
            std::copy(parent1, parent1+parameters.dim, new_population[i]);
            
            if((i+1)<parameters.ns) //problem: i+=2 will go outside of the range... fix
            std::copy(parent2, parent2+parameters.dim, new_population[i+1]);
            
        }
 
        debug(printNewPopulation());
        evaluateCost(function_id,new_population,new_population_cost);
        debug(printCost(new_population_cost));
        
        

        // caution: the swap of address of population can cause memory leak and double free problem
        reduce(elite_sn, best_cost, best_individuo); //also can find the best  solution in constant time, the arrays are already sorted

        debugfile(saveResult(best_cost, best_individuo, "ga_f"+std::to_string(function_id)+"_iterations.csv"));
        debug(printInputPopulation());

        evaluateCost(function_id,population,cost);
        debug(printCost(cost));

        getFitness();

        
    }

    //freeMemoryNewPopulation();
    return best_cost;
}

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


template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::sortPopulationByIndexAsc()
{
    
    std::sort(population_asc_index, population_asc_index +parameters.ns, doCompare(*this));
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::sortNewPopulationByIndexAsc()
{
    std::sort(new_population_asc_index, new_population_asc_index +parameters.ns, doCompareNewPop(*this));
}


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

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::select(Tinput *parent1, Tinput* parent2)
{
    selectParent(parent1);
    selectParent(parent2);
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::selectParent(Tinput* parent)
{
    // changed to 0, because the total fitness can be less than 1, from the document (1,total_fitness)
    Toutput random_number = ms_random_generator.genrand_real_range(0,total_fitness); 
    debug(std::cout << "random fitness: " << random_number << std::endl);
    int s=0;
    while (s < parameters.ns && random_number > 0)
    {
        random_number -= fitness[s];
        s++;
    }
    //s-1 = selected element;
    // copy the individuo to parent
    std::copy(population[s-1], population[s-1]+parameters.dim, parent);


    debug(std::cout << "selected parent: " << (s-1) << std::endl);
    debug(printArray(parent, parameters.dim, ' '));

    
}




//minimize cost
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::evaluateCost(int function_id, Tinput** pop, Toutput* cost)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
    for (int i = 0; i<parameters.ns; i++)
    {
        cost[i] = (functions.*fp)(pop[i], parameters.dim); //evaluate the selected benchmark function
    }
    

}

//maximize fitness
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::getFitness()
{
    total_fitness = 0.0;
    for (int i=0; i<parameters.ns; i++)
    {
        if(cost[i]>=0.0)
        {
            fitness[i] = 1/(1+cost[i]); //normalized index
        }
        else
        {
            fitness[i] = 1+fabs(cost[i]); // positive
        }
        //calculate the total fitness
        total_fitness+= fitness[i];
    }

}


template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printInputPopulation()
{
    std::cout<<"------ population ------" <<std::endl;
    printPopulation(population);

}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printNewPopulation()
{
    std::cout<<"------ new population ------" <<std::endl;
    printPopulation(new_population);

}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printPopulation(Tinput** pop)
{
    for (int i = 0; i < parameters.ns; i++) 
    {  
        for (int j = 0; j < parameters.dim; j++) 
        {
            std::cout << pop[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}


template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printCost(Toutput* cost)
{
    std::cout<<"------ cost ------" << std::endl;
    for (int i = 0; i < parameters.ns; i++) 
    {   
        std::cout<< cost[i] <<std::endl;
    }
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printFitness()
{
    std::cout<<"------ fitness ------" << std::endl;
    for (int i = 0; i < parameters.ns; i++) 
    {   
        std::cout<< fitness[i] <<std::endl;
    }
}


template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::randomInit(Tinput range_low, Tinput range_high)
{
    for (int i = 0; i < parameters.ns; i++) 
    {  
        for (int j = 0; j < parameters.dim; j++) 
        {

            population[i][j] = ms_random_generator.genrand_real_range(range_low,range_high);
           
        }
        
    }
}