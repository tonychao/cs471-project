
#ifndef _SINECOSINEALGORITHM_H_
#define _SINECOSINEALGORITHM_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"
#include <math.h>  

template <class Tinput>
struct SCAInputParameter
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
 

    double a; ///< for r1 calculation
    double r3; ///< r3
    
};


template <class Tinput, class Toutput>
class SineCosineAlgorithm
{
    private:
    PopulationBenchmark<Tinput, Toutput> *population; ///< population
    SCAInputParameter <Tinput> param; ///< PSA parameters
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput global_best_cost; ///<  global best cost
    Tinput* global_best_data; ///< global best data

    void saveResult(Toutput best_cost, Tinput* best_vector, std::string result_file)
    {
        // file pointer 
        std::fstream fout; 
    
        // opens an existing csv file or creates a new file. 
        std::string file_name = result_file;
        fout.open(file_name, std::ios::out | std::ios::app); 
    
        fout << best_cost << "," ;

        for(int i =0; i<param.dim; i++)
        {
            fout << best_vector[i] <<",";
        }
        fout<<"\n";

        fout.close();
    }
    
    void keepInRange(Tinput& element)
    {
        if (element > param.bounds.u)
        {
            element = param.bounds.u;
        }
        else if(element < param.bounds.l)
        {
            element = param.bounds.l;
        }
    }






    void globalBestSetData(Tinput* data)
    {
        for (int j = 0; j<param.dim; j++)
        {
            global_best_data[j] = data[j];
        }
    }
    public:
    SineCosineAlgorithm(SCAInputParameter<Tinput> param)
    {

        this->param = param;

        //print parameters
        std::cout << "bounds.lower: " << param.bounds.l << std::endl;
        std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
        std::cout << "population size: " << param.pop_size<< std::endl;
        std::cout << "maximum number of iterations: " << param.t_max << std::endl;
        std::cout << "dimension: " << param.dim << std::endl;
        std::cout << "a: " << param.a << std::endl;
        std::cout << "r3: " << param.r3 << std::endl;
        

        //allocate memory for population
        population = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);

        //init seed random generator
        ms_random.init_genrand(time(0));

        //data: allocate memory for data
        global_best_data = new Tinput[param.dim];

        
        
    }

    ~SineCosineAlgorithm()
    {
        if (population)
            delete population;
        
        
        if (global_best_data)
            delete[] global_best_data;

    }

    Toutput run(int function_id)
    {
        population->fillWithRandom(param.bounds.l, param.bounds.u);
        debug(population->printPopulation());
        population->evaluateCost(function_id);

 
        // save first global best
        population->calculateMinCost();
        global_best_cost = population->getMinCost();
        globalBestSetData(population->getMinCostData());

        //debug_var(global_best_cost);

        for (int t =0; t<param.t_max; t++)
        {
            Tinput r1 = param.a - t*param.a/param.t_max;
            for (int i = 0; i<param.pop_size; i++)
            {
                // update data of each dimension
                for(int j =0; j<param.dim; j++)
                {
                    Tinput xj = population->getData(i,j);
                    Tinput new_xj;
                    Tinput r2 = ms_random.genrand_real_range(0,2*M_PI);
                    Tinput r3 = ms_random.genrand_real_range(0,2);
                    Tinput r4 = ms_random.genrand_real_range(0,1);
                    if (r4 < 0.5) //sine
                    {
                        new_xj = xj + r1*sin(r2)*fabs(r3*global_best_data[j]-xj);
                    }
                    else //cosine
                    {
                        new_xj = xj + r1*cos(r2)*fabs(r3*global_best_data[j]-xj);
                    }
                    keepInRange(new_xj);
                    population->setData(new_xj, i, j);
                }

            }
            // update the new best
            population->evaluateCost(function_id);
            population->calculateMinCost();
            if(population->getMinCost() < global_best_cost)
            {
                global_best_cost = population->getMinCost();
                globalBestSetData(population->getMinCostData());
            }

            debugfile(saveResult(global_best_cost,global_best_data, "sca_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        debug_var(global_best_cost);
        return global_best_cost;
    }


};

#endif