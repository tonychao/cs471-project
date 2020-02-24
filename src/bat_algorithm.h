
#ifndef _BATALGORITHM_H_
#define _BATALGORITHM_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"
#include <math.h>

template <class Tinput>
struct BAInputParameter
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
 
    

    double r0; ///< initial pulse rate
    double A0_min; ///< initial loundness min
    double A0_max; ///< initial loundness max
    double alfa;
    double gamma;
    double f_min;
    double f_max;
    double ratio_best; ///< percent of the sorted solutions that can be considered best solutions 

};


template <class Tinput, class Toutput>
class BatAlgortihm
{
    private:
    PopulationBenchmark<Tinput, Toutput> *population; ///< population
    BAInputParameter <Tinput> param; ///< PSA parameters
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput global_best_cost; ///<  global best cost
    Tinput* global_best_data; ///< global best data
    Tinput** velocity; ///< array of array which keep the velocity (second array contains dimensions)

    Tinput* pulse_rate;
    Tinput* loundness; 


    Tinput calculateAvgLoundness()
    {
        Tinput avg = 0.0;
        for (int i=0; i<param.pop_size; i++)
        {
            avg +=loundness[i];
        }
        return avg/param.pop_size;
    }

    void updateVelocity(int i, Tinput f)
    {
        for (int j=0; j<param.dim; j++)
        {
            Tinput location =  population->getData(i,j);
            velocity[i][j] = velocity[i][j] + (global_best_data[j] - location)*f;
            //Tinput new_location = location + velocity[i][j];
            //population->setData(new_location,i,j);
        }

    }
    void getNewLocation(int i, Tinput* new_location)
    {
        for (int j=0; j<param.dim; j++)
        {
            new_location[j] = population->getData(i,j) + velocity[i][j];
            //keep new location in range
            keepInRange(new_location[j]);
        }
    }

    void getNewLocationAround(const Tinput* old, Tinput* new_location, Tinput average_loundness)
    {
        for (int j=0; j<param.dim; j++)
        {
            new_location[j] = old[j] + ms_random.genrand_real_range(-1.0,1.0) * average_loundness;
            //keep new location in range
            keepInRange(new_location[j]);
        }
    }
   
    void initVelocity(Tinput x)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            for(int j = 0; j<param.dim; j++)
            {
                velocity[i][j] = x;
            }
        }
    }
    void initPulseRate(Tinput x)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            pulse_rate[i] = x;
        }
    }
    
    void initLoundness(Tinput min, Tinput max)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            loundness[i] = ms_random.genrand_real_range(min, max);
        }
    }

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
    BatAlgortihm(BAInputParameter<Tinput> param)
    {

        this->param = param;

        //print parameters
        std::cout << "bounds.lower: " << param.bounds.l << std::endl;
        std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
        std::cout << "population size: " << param.pop_size<< std::endl;
        std::cout << "maximum number of iterations: " << param.t_max << std::endl;
        std::cout << "dimension: " << param.dim << std::endl;
        std::cout << "r0: " << param.r0 << std::endl;
        std::cout << "A0_min: " << param.A0_min << std::endl;
        std::cout << "A0_max: " << param.A0_max << std::endl;
        std::cout << "alfa: " << param.alfa << std::endl;
        std::cout << "gamma: " << param.gamma << std::endl;
        std::cout << "f_min: " << param.f_min << std::endl;
        std::cout << "f_max: " << param.f_max << std::endl;
        std::cout << "ratio_best: " << param.ratio_best << std::endl;
        
        

        //allocate memory for population
        population = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);

        //init seed random generator
        ms_random.init_genrand(time(0));


        //data: allocate memory for data
        global_best_data = new Tinput[param.dim];


        //allocate memory 
        pulse_rate = new Tinput[param.pop_size];
        loundness = new Tinput[param.pop_size];
        velocity =  new Tinput*[param.pop_size];
        for (int i =0; i< param.pop_size; i++)
        {
            velocity[i] = new Tinput[param.dim];
        }
        
        
    }

    ~BatAlgortihm()
    {
        if (population)
            delete population;
        
        if (pulse_rate)
            delete[] pulse_rate;
        
        if (loundness)
            delete[] loundness;
        
        if (global_best_data)
            delete[] global_best_data;

        // free array of array
        if (velocity)
        {
            for (int i =0; i< param.pop_size; i++)
            {
                if(velocity[i])
                    delete [] velocity[i];
            }
            delete [] velocity;
        }


    }

    Toutput run(int function_id)
    {
        
        // initialize bat population
        population->fillWithRandom(param.bounds.l, param.bounds.u);
        debug(population->printPopulation());
        // initilize bat velocity
        initVelocity(0.0); // with 0.0

        //define frequency
        Tinput frequency;
        
        //initilize pulse rate and loundness 
        initPulseRate(0.0);
        initLoundness(param.A0_min, param.A0_max);
        
        // get the global best bat
        population->evaluateCost(function_id);
        population->sortIndexByCostAsc();
        debug(population->printCost());

        int best_index = population->getAscIndex(0); // get the index of the best bat
        
        debug_var(best_index);
        globalBestSetData(population->getData(best_index));
        global_best_cost = population->getCost(best_index);

        
        //debug_var(global_best_cost);

        Tinput new_location[param.dim];
        Tinput average_loundness;
        for (int t =0; t<param.t_max; t++)
        {
            average_loundness = calculateAvgLoundness();
            debug_var(average_loundness);
            for (int i = 0; i<param.pop_size; i++)
            {
                // adjust frequency
                frequency = ms_random.genrand_real_range(param.f_min,param.f_max);
                // update velocity 
                updateVelocity(i, frequency);
                // get new_location, by reference
                // go toward the global best, exploitation
                getNewLocation(i,new_location);

                //also get new location
                // exploration around the random local best
                
                if(ms_random.genrand_real_range(0,1)>pulse_rate[i])
                {
                    //select a solution among the best solutions
                    int rand_int = (int)(ms_random.genrand_real_range_ex_high(0.0,param.ratio_best)*param.pop_size);
                    //debug_var(rand_int);
                    int selected_index = population->getAscIndex(rand_int);
                    //generate a local solution around the selected best solution, new_location by reference
                    // go toward the local best, exploration
                    getNewLocationAround(population->getData(selected_index), new_location, average_loundness);

                }

              

                // check if we accept the new_solution
                Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,new_location, param.dim);
               
                if(ms_random.genrand_real_range(0,1)<loundness[i] && new_cost<global_best_cost) // some paper suggest to change the global best cost to old bat cost
                {
                    //accept the new solution
                    population->setData(new_location, i);
                    pulse_rate[i] = param.r0*(1.0-exp(-param.gamma*t));
                    loundness[i] = param.alfa*loundness[i];
                    debug_var(pulse_rate[i]);

                    //debug_var(new_cost);
                    //debug_var(global_best_cost);
                    //debug_var(pulse_rate[i]);
                }

            }

            // rank the bat and find the current best 
            // get the global best bat
            population->evaluateCost(function_id);
            population->sortIndexByCostAsc();

            int best_index = population->getAscIndex(0); // get the index of the best bat
            //global_best_data = population->getData(best_index);
            globalBestSetData(population->getData(best_index));
            global_best_cost = population->getCost(best_index);

            
            debugfile(saveResult(global_best_cost,global_best_data, "ba_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        debug_var(global_best_cost);
        return global_best_cost;
    }

    


};

#endif