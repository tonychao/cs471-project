#include "genetic_algorithm.h"
#include <iostream>
#include "functions1.h"
#include <time.h> 


template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::GeneticAlgorithm(Tinput** population, GAInputParameter<Tinput> parameters)
{
    this->population = population;
    this->parameters = parameters;

    //allocate memory for array of cost
    cost =  new Toutput[parameters.ns];
    fitness = new Toutput[parameters.ns];
    
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

template <class Tinput, class Toutput>
GeneticAlgorithm<Tinput,Toutput>::~GeneticAlgorithm()
{
    // free array of cost
    if(cost)
        delete[] cost;
    
    // free array of fitness
    if(fitness)
        delete[] fitness;
    
}

template <class Tinput, class Toutput>
Tinput* GeneticAlgorithm<Tinput, Toutput>::findBestSolution(int function_id)
{
    int elite_sn = (int) (parameters.er * parameters.ns);

    evaluateCost(function_id); // calculate the cost of each individuo
    //print to see
    printCost();

    getFitness();
    printFitness();
    std::cout<<"total fitness: " << total_fitness << std::endl;
    //allocate memory for new population
    allocateMemoryNewPopulation();

    // create parent1 and parent2
    Tinput parent1[parameters.dim];
    Tinput parent2[parameters.dim];

    

    //select parents by roulette wheel selection
    select(parent1, parent2); // pass parents by reference
    crossover(parent1,parent2, parameters.cr);
    mutate(parent1);
    mutate(parent2);

    for (int t=0; t<parameters.t_max; t++) // GA iteration
    {
        for (int i=0; i<parameters.ns; i++)
        {



            
        }
    }

    freeMemoryNewPopulation();
    return population[0];
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::crossover(Tinput* parent1, Tinput* parent2, double cr)
{
    if (ms_random_generator.genrand_real_range(0,1) < cr) //crossover only when the criteria meet
    {
        int d = (int) ms_random_generator.genrand_real_range_ex_high(0,parameters.dim); //cast to int
        std::cout << "random int: " << d << std::endl;
        Tinput temp1[parameters.dim];
        // copy parent1 to temp1
        std::copy(parent1, parent1+parameters.dim, temp1);
        //crossover
        std::copy(parent2+d, parent2+parameters.dim, parent1+d); // copy 2nd part of parent2 to parent1
        std::copy(temp1+d, temp1+parameters.dim, parent2+d); // copyt 2nd part of parent1 (stored in temp1) to parent2
        
        std::cout << "---after crossover---" << std::endl;
        for (int i=0; i<parameters.dim; i++)
        {
            std::cout << parent1[i] << " ";
        }
        std::cout << std::endl;
        for (int i=0; i<parameters.dim; i++)
        {
            std::cout << parent2[i] << " ";
        }
        std::cout << std::endl;

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
                            

        }

    }

    std::cout << "---after mutation---" << std::endl;
    for (int i=0; i<parameters.dim; i++)
    {
        std::cout << individuo[i] << " ";
    }
    std::cout<<std::endl;
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
    std::cout << "random fitness: " << random_number << std::endl;
    int s=0;
    while (s < parameters.ns && random_number > 0)
    {
        random_number -= fitness[s];
        s++;
    }
    //s-1 = selected element;
    // copy the individuo to parent
    std::cout << "selected parent: " << (s-1) << std::endl;
    std::copy(population[s-1], population[s-1]+parameters.dim, parent);

    for (int i=0; i<parameters.dim; i++)
    {
        std::cout << parent[i] << " ";
    }
    std::cout << std::endl;
}


template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::allocateMemoryNewPopulation()
{
    new_population = new Tinput*[parameters.ns];

    for (int i = 0; i<parameters.ns; i++)
    {
        new_population[i] = new Tinput[parameters.dim];
    }
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::freeMemoryNewPopulation()
{
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

//minimize cost
template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::evaluateCost(int function_id)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
    for (int i = 0; i<parameters.ns; i++)
    {
        cost[i] = (functions.*fp)(population[i], parameters.dim); //evaluate the selected benchmark function
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
void GeneticAlgorithm<Tinput,Toutput>::printPopulation()
{
    std::cout<<"------ population ------" <<std::endl;
    for (int i = 0; i < parameters.ns; i++) 
    {  
        for (int j = 0; j < parameters.dim; j++) 
        {
            std::cout << population[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput,Toutput>::printCost()
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