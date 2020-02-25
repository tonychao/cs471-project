
#ifndef _ANTLION_H_
#define _ANTLION_H_

#include "population_benchmark.h" //template class
#include "../lib/mt19937ar_class.h"
#include <math.h>

template <class Tinput>
struct ALOInputParameter
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
class AntLionOptimizer
{
    private:
    PopulationBenchmark<Tinput, Toutput> *ants_pop; ///< population
    PopulationBenchmark<Tinput, Toutput> *antlions_pop; ///< population
    PopulationBenchmark<Tinput, Toutput> *combined_pop; ///< population
    
    ALOInputParameter <Tinput> param; ///< PSA parameters
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

    
    int rouletteWheelSelect(PopulationBenchmark<Tinput, Toutput>* population)
    {
        // changed to 0, because the total fitness can be less than 1, from the document (1,total_fitness)
        Toutput random_number = ms_random_generator.genrand_real_range(0,population->getTotalFitness()); 
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

    Tinput getI(int t)
    {
        Tinput I = 1.0;
        if (t > param.t_max*0.95)
        {
            I=1+1000000*(t/param.t_max);
        }
        else if (t > param.t_max*0.9)
        {
            I=1+100000*(t/param.t_max);
        }
        else if (t > param.t_max*3.0/4.0)
        {
            I=1+10000*(t/param.t_max);
        }
        else if (t > param.t_max/2.0)
        {
            I=1+1000*(t/param.t_max);
        }
        else if (t > param.t_max/10.0)
        {
            I=1+100*(t/param.t_max);
        }

        return I;
    }
    void copyData(Tinput *dst, Tinput *ori)
    {
        for (int j = 0; j<param.dim; j++)
        {
            dst[j] = ori[j];
        }
    }
    void addDataConstant(Tinput* data, Tinput constant)
    {
        for (int j = 0; j<param.dim; j++)
        {
            data[j] = data[j] + constant;
            keepInRange(data[j]);
        }
    }

    void randomWalkCumSum(Tinput* X, int n)
    {
        X[0] = 0.0;
        for (int i=1; i<n; i++)
        {
            if(ms_random.genrand_real1()>0.5)
            {
                X[i] = X[i-1] + 1; 
            }
            else
            {
                X[i] = X[i-1] - 1;
            }
            
        }
    }

    Tinput getMax(Tinput* X, int n)
    {
        Tinput max = X[0];
        for (int i=1; i<n; i++)
        {
            if (X[i] > max)
            {
                max = X[i];
            }
        }
        return max;
    }

    Tinput getMin(Tinput* X, int n)
    {
        Tinput min = X[0];
        for (int i=1; i<n; i++)
        {
            if (X[i] < min)
            {
                min = X[i];
            }
        }
        return min;
    }

    void randomWalkAroundAntlion(int antlion,int t, Tinput* RWs)
    {
        Tinput lb, ub;
        Tinput I =  getI(t);
        lb = param.bounds.l / I; // equation (2.10) in the paper
        ub = param.bounds.u / I; // equation (2.11) in the paper

        Tinput lb_c[param.dim];
        Tinput ub_d[param.dim];
        copyData(lb_c, antlions_pop->getData(antlion));
        copyData(ub_d, antlions_pop->getData(antlion));

        
        // move around antlion
        if (ms_random.genrand_real1()<0.5)
        {
            addDataConstant(lb_c,lb); // equation (2.8) in the paper
        }
        else
        {
            addDataConstant(lb_c,-lb);
        }

        if (ms_random.genrand_real1()>= 05)
        {
            addDataConstant(ub_d,ub); // equation (2.9) in the paper
        }
        else
        {
            addDataConstant(ub_d,-ub);
        }

        // random walk and normallize
        // equation (2.1) and (2.7) in the paper
        Tinput X[param.t_max]; // storage for random walk
        //Tinput X_norm[param.t_max]; // normalized random walk
        //Tinput RWs[param.dim]; // normalized randomwalk for return
        for (int j =0; j < param.dim; j++)
        {
            randomWalkCumSum(X, param.t_max); // fill X with random walk
            Tinput a = getMin(X);
            Tinput b = getMax(X);
            Tinput c = lb_c[j];
            Tinput d = ub_d[j];
            RWs[j] = (X[t] - a)*(d-c)/(b-a) + c; // equation (2.7) in the paper
        }
        

         
    }
    
    public:
    AntLionOptimizer(DBAInputParameter<Tinput> param)
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
        ants_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);
        antlions_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);
        combined_pop = new PopulationBenchmark<Tinput, Toutput>(2*param.pop_size, param.dim);

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

    ~AntLionOptimizer()
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
        
        // initialize ants and antlions population
        ants_pop->fillWithRandom(param.bounds.l, param.bounds.u);
        antlions_pop->fillWithRandom(param.bounds.l, param.bounds.u);
        debug(ants_pop->printPopulation());
        

        // calculate the cost of ants and antlions
        ants_pop->evaluateCost(function_id);
        ants_pop->calculateMinCost();

        antlions_pop->evaluateCost(function_id);
        antlions_pop->calculateMinCost();
        antlions_pop->evaluateFitness();
        
        int elite = antlions_pop->getMinCostIndex();
        global_best_cost = antlions_pop->getMinCost();
        globalBestSetData(antlions_pop->getMinCostData());

        // store the selected antlions by each ant
        int selected_antlions[param.pop_size];


        
        for (int t =0; t<param.t_max; t++)
        {

            for (int i = 0; i<param.pop_size; i++)
            {
                
                // for each ant select an antlion by roulette wheel
                int antlion = rouletteWheelSelect(antlions_pop);
                selected_antlions[i] = antlion;  
                // RA = location around selected antlion by roulette wheel
                Tinput RA[param.dim]; // normalized randomwalk pass by reference
                randomWalkAroundAntlion(antlion,t, RA);

                // RE = location around elite antlion
                Tinput RE[param.dim];
                randomWalkAroundAntlion(elite,t,RE);
                
                // update the position of ant
                // equation (2.13) from the paper
                // the ant position is only based on random walk around elite and selected antlion

                Tinput new_ant[param.dim];
                for (int j = 0; j< param.dim; j++)
                {
                    new_ant[j] = (RA[j] + RE[j])/2.0;
                }

                ants_pop->setData(new_ant, i);
                

            }

            //evaluate the cost of ant population
            ants_pop->evaluateCost(function_id);
            ants_pop->calculateMinCost();

            // the antlion take the ant position if the ant has better cost
            for (int i = 0; i<param.pop_size; i++)
            {
                Tinput antlion_cost = antlions_pop->getCost(selected_antlions[i]);
                Tinput ant_cost = ants_pop->getCost(i);
                if(ant_cost < antlion_cost)
                {
                    //the antlion take the position of the ant
                    antlions_pop->setDataAndCost(ants_pop->getData(i),selected_antlions[i],ant_cost);
                }
            }

            // get mincost
            antlions_pop->calculateMinCost();
            Tinput new_min_cost = antlions_pop->getMinCost();
            // recalculate fitness
            antlions_pop->evaluateFitness();

            // update elite if an antlion becomes fitter than the elite
            if(new_min_cost < global_best_cost)
            {
                elite = antlions_pop->getMinCostIndex();
                global_best_cost = new_min_cost;
                globalBestSetData(antlions_pop->getMinCostData());
                
            }
            
            // select elite
            debugfile(saveResult(global_best_cost,global_best_data, "alo_f"+std::to_string(function_id)+"_iterations.csv"));
        
        }
        
        debug_var(global_best_cost);
        return global_best_cost;
    }

    


};

#endif