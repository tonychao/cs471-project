
#ifndef _PARTICLESWARM_H_
#define _PARTICLESWARM_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"

template <class Tinput>
struct PSAInputParameter
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
 

    double c1; ///< config parameter c1
    double c2; ///< config parameter c2
    double w; ///< config parameter weight for vel
    double w_min; ///< config parameter for variable inertial weight
    
};


template <class Tinput, class Toutput>
class ParticleSwarm
{
    private:
    PopulationBenchmark<Tinput, Toutput> *population; ///< population
    PSAInputParameter <Tinput> param; ///< PSA parameters
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput global_best_cost; ///<  global best cost
    Tinput* global_best_data; ///< global best data
    Toutput* particle_best_cost; ///< array which keep each particle best cost
    Tinput** particle_best_data; ///< array of array to keep the data of best particle
    Tinput** particle_vel; ///< array of array which keep each particle velocity (second array contains dimensions)
   
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

    void particleUpdateVelAndPar(int i, int function_id, bool variable_weight, int iteration)
    {
        Tinput new_particle[param.dim];
        for(int j =0; j<param.dim; j++)
        {
            Tinput pj = population->getData(i,j);
            Tinput p_best = param.c1*ms_random.genrand_real_range(0,1)*(particle_best_data[i][j] - pj);
            Tinput g_best = param.c2*ms_random.genrand_real_range(0,1)*(global_best_data[j] - pj);
            if (variable_weight)
            {
                Tinput variable_w = param.w - (param.w-param.w_min)/param.t_max*iteration;
                particle_vel[i][j] = variable_w*particle_vel[i][j] + p_best + g_best;
            }
            else
            {
                particle_vel[i][j] = param.w*particle_vel[i][j] + p_best + g_best;
            }
                
            

            new_particle[j] = pj + particle_vel[i][j]; 
            keepInRange(new_particle[j]);

        }
        //debug_var(population->getCost(i));
        //calculate new particle cost
        Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,new_particle, param.dim);
        population->setDataAndCost(new_particle,i,new_cost);
        //debug_var(new_cost);

        // check if the particle velocity has improved
        if (new_cost < particle_best_cost[i])
        {
            particle_best_cost[i] = new_cost;
            particleBestSetData(i,new_particle);
        }

        //check if gbest has improve
        if (new_cost < global_best_cost)
        {
            global_best_cost = new_cost;
            globalBestSetData(new_particle);
            
        }
        
  
        
    } 

    void particleRandVel(int i)
    {
        for (int j=0; j<param.dim; j++)
        {
            particle_vel[i][j] = ms_random.genrand_real_range(0, 0.5*(param.bounds.u - param.bounds.l));
        }
        
    }

    void particleBestSetData(int i, Tinput* data)
    {
        for (int j = 0; j<param.dim; j++)
        {
            particle_best_data[i][j] = data[j];
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
    ParticleSwarm(PSAInputParameter<Tinput> param)
    {

        this->param = param;

        //print parameters
        std::cout << "bounds.lower: " << param.bounds.l << std::endl;
        std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
        std::cout << "population size: " << param.pop_size<< std::endl;
        std::cout << "maximum number of iterations: " << param.t_max << std::endl;
        std::cout << "dimension: " << param.dim << std::endl;
        std::cout << "c1: " << param.c1 << std::endl;
        std::cout << "c2: " << param.c2 << std::endl;
        std::cout << "w: " << param.w << std::endl;
        std::cout << "w_min: " << param.w_min << std::endl;
        

        //allocate memory for population
        population = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);

        //init seed random generator
        ms_random.init_genrand(time(0));

        //COST: allocate memory for global best, particle best and particle velocity
        particle_best_cost = new Toutput[param.pop_size];
        particle_vel =  new Tinput*[param.pop_size];
        for (int i =0; i< param.pop_size; i++)
        {
            particle_vel[i] = new Tinput[param.dim];
        }

        //data: allocate memory for data
        global_best_data = new Tinput[param.dim];

        particle_best_data =  new Tinput*[param.pop_size];
        for (int i =0; i< param.pop_size; i++)
        {
            particle_best_data[i] = new Tinput[param.dim];
        }

        
        
    }

    ~ParticleSwarm()
    {
        if (population)
            delete population;
        
        if (particle_best_cost)
            delete[] particle_best_cost;
        
        if (global_best_data)
            delete[] global_best_data;

        // free array of array
        if (particle_vel)
        {
            for (int i =0; i< param.pop_size; i++)
            {
                if(particle_vel[i])
                    delete [] particle_vel[i];
            }
            delete [] particle_vel;
        }

        // free array of array
        if (particle_best_data)
        {
            for (int i =0; i< param.pop_size; i++)
            {
                if(particle_best_data[i])
                    delete [] particle_best_data[i];
            }
            delete [] particle_best_data;
        }

    }

    Toutput run(int function_id, bool variable_weight)
    {
        population->fillWithRandom(param.bounds.l, param.bounds.u);
        debug(population->printPopulation());
        population->evaluateCost(function_id);
        for (int i =0; i<param.pop_size; i++)
        {
            particleRandVel(i);
            particle_best_cost[i] = population->getCost(i);
            particleBestSetData(i, population->getData(i));
        }
        debug(printArray<Toutput>(particle_vel[0],param.dim,' '));
        debug(printArray<Toutput>(particle_best,param.pop_size,' '));
        debug(printArray<Toutput>(particle_best_data[0],param.dim,' '));
        
        // save first global best
        population->calculateMinCost();
        global_best_cost = population->getMinCost();
        globalBestSetData(population->getMinCostData());

        //debug_var(global_best_cost);

        for (int t =0; t<param.t_max; t++)
        {
            for (int i = 0; i<param.pop_size; i++)
            {
                // calculate new velocity for each particle
                particleUpdateVelAndPar(i,function_id, variable_weight, t); 
            }
            
            debugfile(saveResult(global_best_cost,global_best_data, "psa_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        //debug_var(global_best_cost);
        return global_best_cost;
    }

    


};

#endif