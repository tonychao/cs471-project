
#ifndef _QUICKARTIFICIALBEECOLONY_H_
#define _QUICKARTIFICIALBEECOLONY_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"
#include <math.h>

template <class Tinput>
struct QABCInputParameter
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
    int l; ///< limit
    double r; ///< neighbourhood radio


};


template <class Tinput, class Toutput>
class QuickArtificialBeeColony
{
    private:
    PopulationBenchmark<Tinput, Toutput> *food_pop; ///< population

    QABCInputParameter <Tinput> param; ///< PSA parameters
    MersenneTwister ms_random; ///< mersenne twister random generator
    Toutput global_best_cost; ///<  global best cost
    Tinput* global_best_data; ///< global best data
    int* trial; ///< store how many time the mutation has not find better solution 


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

    
    int rouletteWheelSelect(PopulationBenchmark<Tinput, Toutput>* population)
    {
        // changed to 0, because the total fitness can be less than 1, from the document (1,total_fitness)
        Toutput random_number = ms_random.genrand_real_range(0,population->getTotalFitness()); 
        debug(std::cout << "random fitness: " << random_number << std::endl);
        int s=0;
        while (s < param.pop_size && random_number > 0)
        {
            random_number -= population->getFitness(s);
            s++;
        }
        //s-1 = selected element;

        debug(std::cout << "selected : " << (s-1) << std::endl);
        return s-1;
    }

 


    void calculateEuclideanDistance(int xm, Tinput* eucli_dist)
    {  
        
        for (int i=0; i< param.pop_size; i++)
        {
            Tinput sum = 0.0;
            for (int j = 0; j<param.dim; j++)
            {
                Tinput d = food_pop->getData(xm,j) - food_pop->getData(i,j);
                sum += d*d;       
            }
            eucli_dist[i] = sqrt(sum);
        }

      
    }    

    Tinput meanEuclideanDistance(Tinput* eucli_dist)
    {
        Tinput sum_dist = 0.0;
        for (int i=0; i< param.pop_size; i++)
        {
            sum_dist += eucli_dist[i];
        }

        return sum_dist / (param.pop_size - 1.0);
 
    }

    int getBestFromNeighbourhood(int xm)
    {
        Tinput eucli_dist[param.pop_size];

        //calculate mean euclidean distance for xm
        calculateEuclideanDistance(xm, eucli_dist);
        Tinput mdm = meanEuclideanDistance(eucli_dist);
        
        // operation inside neighbourhood
        Tinput best_fitness = std::numeric_limits<Tinput>::min();
        int best_i  = -1;
        for (int i  = 0; i<param.pop_size; i++)
        {
            if (eucli_dist[i] <= param.r * mdm)
            {
                if (food_pop->getFitness(i) > best_fitness)
                {
                    best_fitness = food_pop->getFitness(i);
                    best_i = i;
                }

            }

        }

        return best_i;
    }

    void init(int scout, int function_id)
    {
        Tinput new_pos[param.dim];
        for (int j =0; j<param.dim; j++)
        {
            new_pos[j] = ms_random.genrand_real_range(param.bounds.l,param.bounds.u);
        }
        Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id, new_pos, param.dim);
        food_pop->setDataAndCost(new_pos,scout,new_cost);
        trial[scout] = 0;

    }
    void employedBeePhase(int function_id)
    {
        Tinput vm[param.dim];

        for (int i = 0; i<param.pop_size; i++)
        {
            
            for (int j =0; j<param.dim; j++)
            {
                vm[j] = food_pop->getData(i,j);
            }
            // todo: maybe it is better to add code to avoid selecting the same i
            int j = (int)ms_random.genrand_real_range_ex_high(0,param.dim); // random chosen dimension
            int k = (int)ms_random.genrand_real_range_ex_high(0,param.pop_size); // food source selected randomly
            //similar to mutation
            vm[j] = food_pop->getData(i,j) + ms_random.genrand_real_range(-1.0,1.0) * (food_pop->getData(i,j) - food_pop->getData(k,j));
            keepInRange(vm[j]);
            

            // evaluate new vm
            Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vm, param.dim);

            if (new_cost < food_pop->getCost(i))
            {
                food_pop->setDataAndCost(vm,i,new_cost);
                trial[i] = 0;
            }
            else
            {
                trial[i]++;
            }
            
        }
    }

    void onLookerBeePhase(int function_id)
    {
         Tinput vm[param.dim];
        food_pop->evaluateFitness();
        // qABC part... different than ABC
        for (int i = 0; i<param.pop_size; i++)
        {
            // for each onlooker bee  select food position by roulette wheel
            int xm = rouletteWheelSelect(food_pop);
            int best_xm_neighbour = getBestFromNeighbourhood(xm);

            
            for (int j =0; j<param.dim; j++)
            {
                vm[j] = food_pop->getData(best_xm_neighbour,j);
            }
            // todo: maybe it is better to add code to avoid selecting the same i
            int j = (int)ms_random.genrand_real_range_ex_high(0,param.dim); // random chosen dimension
            int k = (int)ms_random.genrand_real_range_ex_high(0,param.pop_size); // food source selected randomly
            //similar to mutation
            vm[j] = food_pop->getData(best_xm_neighbour,j) + ms_random.genrand_real_range(-1.0,1.0) * (food_pop->getData(best_xm_neighbour,j) - food_pop->getData(k,j));
            keepInRange(vm[j]);

            // evaluate new vm
            Toutput new_cost = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vm, param.dim);

            if (new_cost < food_pop->getCost(best_xm_neighbour)) // can use getCost
            {
                food_pop->setDataAndCost(vm,i,new_cost); // replace the best of the neighbourhood or replace i?
                trial[i] = 0;
            }
            else
            {
                trial[i]++;
            }
        }
    }

    void bestSolutionMemo()
    {
        food_pop->calculateMinCost();

        if (food_pop->getMinCost() < global_best_cost)
        {
            global_best_cost = food_pop->getMinCost();
            globalBestSetData(food_pop->getMinCostData());
        }
    }

    void scoutBeePhase(int function_id)
    {
        for (int i =0; i<param.pop_size; i++)
        {
            if(trial[i] > param.l)
            {
                init(i,function_id); // convert i into scout .... asign new random position.
            }
        }
    }
    public:
    QuickArtificialBeeColony(QABCInputParameter<Tinput> param)
    {

        this->param = param;
        //print parameters
        std::cout << "bounds.lower: " << param.bounds.l << std::endl;
        std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
        std::cout << "population size: " << param.pop_size<< std::endl;
        std::cout << "maximum number of iterations: " << param.t_max << std::endl;
        std::cout << "dimension: " << param.dim << std::endl;
        std::cout << "limit: " << param.l<< std::endl;
        std::cout << "neighbourhood radio: " << param.r<< std::endl;

        
    
        //allocate memory for population
        food_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);


        //init seed random generator
        ms_random.init_genrand(time(0));

        //data: allocate memory for data
        global_best_data = new Tinput[param.dim];
        trial = new int[param.pop_size];
        // set trial with 0
        for(int i =0; i< param.pop_size; i++)
        {
            trial[i]=0; // can be changed to memset
        }
        
    }

    ~QuickArtificialBeeColony()
    {
        if (food_pop)
            delete food_pop;
        
        if (trial)
            delete[] trial;

        if (global_best_data)
            delete[] global_best_data;

    }



    Toutput run(int function_id)
    {
        
        // initialize food population
        food_pop->fillWithRandom(param.bounds.l, param.bounds.u);
        debug(food_pop->printPopulation());
        

        // calculate the cost 
        food_pop->evaluateCost(function_id);
        food_pop->calculateMinCost();

        
        //int elite = food_pop->getMinCostIndex();
        global_best_cost = food_pop->getMinCost();
        globalBestSetData(food_pop->getMinCostData());
        debug(food_pop->printCost());
        

        
        for (int t =0; t<param.t_max; t++)
        {

            // employed bee phase
            employedBeePhase(function_id);
            // onlooker bees phase
            onLookerBeePhase(function_id);

            // memorize the best solution found so far
            bestSolutionMemo();
            
            // scout bee phase
            // note: the author's c version onlly send one scout bee per iteration
            scoutBeePhase(function_id);
 
            debugfile(saveResult(global_best_cost,global_best_data, "qabc_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        debug_var(global_best_cost);
        return global_best_cost;
    }

    


};

#endif