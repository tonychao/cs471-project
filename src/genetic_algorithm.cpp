#include "genetic_algorithm.h"
#include <iostream>
#include "functions1.h"


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

    evaluate(function_id); // calculate the cost of each individuo
    //print to see
    printCost();

    //allocate memory for new population
    allocateMemoryNewPopulation();


    for (int t=0; t<parameters.t_max; t++) // GA iteration
    {
        for (int i=0; i<parameters.ns; i++)
        {
            //select parents by roulette wheel selection
        }
    }

    freeMemoryNewPopulation();
    return population[0];
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

template <class Tinput, class Toutput>
void GeneticAlgorithm<Tinput, Toutput>::evaluate(int function_id)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
    for (int i = 0; i<parameters.ns; i++)
    {
        cost[i] = (functions.*fp)(population[i], parameters.dim); //evaluate the selected benchmark function
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