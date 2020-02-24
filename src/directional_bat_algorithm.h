
#ifndef _DIRECTIONALBATALGORITHM_H_
#define _DIRECTIONALBATALGORITHM_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"
#include <math.h>

template <class Tinput>
struct DBAInputParameter
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
    double rinf;
    double A0; ///< initial loundness min
    double Ainf; ///< initial loundness max
    double f_min;
    double f_max;

};


template <class Tinput, class Toutput>
class DirectionalBatAlgortihm
{
    private:
    PopulationBenchmark<Tinput, Toutput> *population; ///< population
    DBAInputParameter <Tinput> param; ///< PSA parameters
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput global_best_cost; ///<  global best cost
    Tinput* global_best_data; ///< global best data
    Tinput** velocity; ///< array of array which keep the velocity (second array contains dimensions)

    Tinput* w;
    Tinput w_0;
    Tinput w_inf;
    Tinput* pulse_rate;
    Tinput* loundness; 




    int selectRandomBat(int i)
    {
        int rand;
        while (true)
        {
            rand = (int)ms_random.genrand_real_range_ex_high(0.0, param.pop_size);
            if (rand !=i)
            {
                break;
            }
        }
        return rand;

    }

    Tinput calculateAvgLoundness()
    {
        Tinput avg = 0.0;
        for (int i=0; i<param.pop_size; i++)
        {
            avg +=loundness[i];
        }
        return avg/param.pop_size;
    }

    void updateLocation(Tinput* new_location, int i, Tinput f1, Tinput f2, int random_bat, int function_id)
    {


        if(population->calcCost1Item(function_id, random_bat) < population->calcCost1Item(function_id, i))
        {
            for (int j=0; j<param.dim; j++)
            {
                Tinput location =  population->getData(i,j);
                Tinput random_bat_loc = population->getData(random_bat,j);
                new_location[j] = location + (global_best_data[j] - location)*f1 + (random_bat_loc-location)*f2;
                keepInRange(new_location[j]);
            }
        }
        else
        {
            for (int j=0; j<param.dim; j++)
            {
                Tinput location =  population->getData(i,j);
                new_location[j] = location + (global_best_data[j] - location)*f1;
                keepInRange(new_location[j]);
            }
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

    void getNewLocationAround(const Tinput* old, Tinput* new_location, Tinput average_loundness, int i)
    {
        for (int j=0; j<param.dim; j++)
        {
            new_location[j] = old[j] + ms_random.genrand_real_range(-1.0,1.0) * average_loundness * w[i];
            //keep new location in range
            keepInRange(new_location[j]);
        }
    }
   
    void initW(Tinput x)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            w[i] = x;
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
    
    void initLoundness(Tinput x)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            loundness[i] = x;
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
    DirectionalBatAlgortihm(DBAInputParameter<Tinput> param)
    {

        this->param = param;

        //print parameters
        std::cout << "bounds.lower: " << param.bounds.l << std::endl;
        std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
        std::cout << "population size: " << param.pop_size<< std::endl;
        std::cout << "maximum number of iterations: " << param.t_max << std::endl;
        std::cout << "dimension: " << param.dim << std::endl;
        std::cout << "r0: " << param.r0 << std::endl;
        std::cout << "rinf: " << param.rinf << std::endl;
        std::cout << "A0: " << param.A0 << std::endl;
        std::cout << "Ainf: " << param.Ainf << std::endl;
        std::cout << "f_min: " << param.f_min << std::endl;
        std::cout << "f_max: " << param.f_max << std::endl;
        
        
        

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
        w = new Tinput[param.pop_size];

        for (int i =0; i< param.pop_size; i++)
        {
            velocity[i] = new Tinput[param.dim];
        }
        
        
    }

    ~DirectionalBatAlgortihm()
    {
        if (population)
            delete population;
        
        if (pulse_rate)
            delete[] pulse_rate;
            
        if (w)
            delete[] w;

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
        Tinput f1;
        Tinput f2;
        
        //initilize pulse rate and loundness 
        initPulseRate(param.r0);
        initLoundness(param.A0);

        w_0 = (param.bounds.u-param.bounds.l)/4.0;
        w_inf = w_0 / 100;
        initW(w_0);
        

        
        // get the global best bat
        population->evaluateCost(function_id);
        population->calculateMinCost();
        global_best_cost = population->getMinCost();
        globalBestSetData(population->getMinCostData());


        
        //debug_var(global_best_cost);

        Tinput new_location[param.dim];
        Tinput average_loundness;
        for (int t =0; t<param.t_max; t++)
        {
            average_loundness = calculateAvgLoundness();
            debug_var(average_loundness);
            for (int i = 0; i<param.pop_size; i++)
            {
                //select random bat
                int random_bat = selectRandomBat(i);

                // adjust frequency
                f1 = ms_random.genrand_real_range(param.f_min,param.f_max);
                f2 = ms_random.genrand_real_range(param.f_min,param.f_max);
                
                // get new_location, by reference
                // go toward the global best and random bat, exploitation
                updateLocation(new_location,i, f1, f2, random_bat, function_id);

                //also get new location
                // exploration around the random bat
                if(ms_random.genrand_real_range(0,1)>pulse_rate[i])
                {
                   
                    //generate a local solution around the random bat, new_location by reference
                    // go toward the random bat, exploration
                    getNewLocationAround(population->getData(random_bat), new_location, average_loundness, i);

                    // update w
                    w[i] = (w_0-w_inf)/(0.0-param.t_max)*(t-param.t_max) + w_inf;
                }

              

                // check if we accept the new_solution
                Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,new_location, param.dim);
               
                if(ms_random.genrand_real_range(0,1)<loundness[i] && new_cost < population->calcCost1Item(function_id,i)) // some paper suggest to change the global best cost to old bat cost
                {
                    //accept the new solution
                    population->setData(new_location, i);
                    // update pulse rate
                    pulse_rate[i] = (param.r0 - param.rinf)/(0.0-param.t_max)*(t - param.t_max) + param.rinf;
                    // update loundness
                    loundness[i] = (param.A0 - param.Ainf)/(0.0-param.t_max)*(t - param.t_max) + param.Ainf;
                    debug_var(pulse_rate[i]);

                    //debug_var(new_cost);
                    //debug_var(global_best_cost);
                    //debug_var(pulse_rate[i]);
                }

                // check if the current bat is better than the global best
                if(new_cost < global_best_cost)
                {
                    global_best_cost = new_cost;
                    globalBestSetData(new_location);
                }

            }

            
            debugfile(saveResult(global_best_cost,global_best_data, "dba_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        debug_var(global_best_cost);
        return global_best_cost;
    }

    


};

#endif