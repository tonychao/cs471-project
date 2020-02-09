#include "differential_evolution.h"


template <class Tinput, class Toutput>
DifferentialEvolution<Tinput, Toutput>::DifferentialEvolution(DEInputParameter<Tinput> param)
{
    this->param = param;
    //print parameters

    std::cout << "bounds.lower: " << param.bounds.l << std::endl;
    std::cout << "bounds.uppper: " << param.bounds.u << std::endl;
    std::cout << "crossover rate: " << param.cr << std::endl;
    std::cout << "dimension: " << param.dim << std::endl;
    std::cout << "scale f: " << param.scale_f << std::endl;
    std::cout << "scale lambda: " << param.scale_lambda << std::endl;
    std::cout << "population size: " << param.pop_size<< std::endl;
    std::cout << "maximum number of iterations: " << param.t_max << std::endl;
    
    // allocate memory for population
    actual_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);
    new_pop = new PopulationBenchmark<Tinput, Toutput>(param.pop_size, param.dim);

    //init seed random generator
    ms_random.init_genrand(time(0));

    // allocate memory for cost
    best_cost = new Toutput[param.t_max];
}

template <class Tinput, class Toutput>
DifferentialEvolution<Tinput, Toutput>::~DifferentialEvolution()
{

    if(actual_pop)
        delete actual_pop;
    

    if(new_pop)
        delete new_pop;

    if(best_cost)
        delete best_cost;
}

/// @brief generate random vector position r used to create trail vectors
/// 
/// the random vector positions r should satisfy the condition that they cannot repeat
/// and they cannot be equal to the current vector i
/// @param *r ///< pointer of array r
/// @param n ///< size of array r
/// @param current_i ///< current position of the vector i
template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>::randomR(int *r, int n, int current_i)
{
    int a;
    for (int i =0; i<n; i++)
    {
        while(true)
        {
            a = (int)ms_random.genrand_real_range_ex_high(0,param.pop_size);
            if(i>0) // the vector r has some elements
            {
                bool unique = true;
                for (int j =0; j<i; j++)
                {
                    if(a==r[j]||a==current_i)
                    {
                        unique = false;
                        break;
                    }

                }
                if(unique)
                {
                    r[i] = a;
                    break;
                }
            }
            else
            {
                if(a!=current_i)
                {
                    r[i] = a;
                    break;
                }
            }
            
        }
        
        
    }
}

/// @brief keep the element of the vector inside the search range 
/// 
/// the method trucante the element to the upper and lower bound of the range, when the element excess these limits.
/// @param &element ///< element of the vector

template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>::keepInRange(Tinput& element)
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

template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS7_DE_rand_1_bin(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    int nr = 3;
    int r[nr];

    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            //trial vector u;
            Tinput vector_u[param.dim];

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                if(ms_random.genrand_real_range(0,1) < param.cr || j==j_rand ) //
                {
                    vector_u[j] = actual_pop->getData(r[2],j) + param.scale_f*(actual_pop->getData(r[0],j) - actual_pop->getData(r[1],j));
                    keepInRange(vector_u[j]);
                }
                else
                {
                    vector_u[j] = actual_pop->getData(i,j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de7_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
 
    return best_cost[param.t_max-1];
}

template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS10_DE_rand_2_bin(int function_id)
{
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    int nr = 5;
    int r[nr];

    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {

        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            //trial vector u;
            Tinput vector_u[param.dim];

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                if(ms_random.genrand_real_range(0,1) < param.cr || j==j_rand ) //
                {
                    Tinput diff = actual_pop->getData(r[0],j) + actual_pop->getData(r[1],j) - actual_pop->getData(r[2],j) - actual_pop->getData(r[3],j);
                    vector_u[j] = actual_pop->getData(r[4],j) + param.scale_f*diff;
                    keepInRange(vector_u[j]);
                }
                else
                {
                    vector_u[j] = actual_pop->getData(i,j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de10_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));

    return best_cost[param.t_max-1];
}

template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS6_DE_best_1_bin(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 2;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            //trial vector u;
            Tinput vector_u[param.dim];

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                if(ms_random.genrand_real_range(0,1) < param.cr || j==j_rand ) //
                {
                    
                    vector_u[j] = actual_pop->getMinCostData()[j] + param.scale_f*(actual_pop->getData(r[0],j) - actual_pop->getData(r[1],j));
                    keepInRange(vector_u[j]);
                }
                else
                {
                    vector_u[j] = actual_pop->getData(i,j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de6_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
    
    return best_cost[param.t_max-1];
}


template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS9_DE_best_2_bin(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 4;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            //trial vector u;
            Tinput vector_u[param.dim];

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                if(ms_random.genrand_real_range(0,1) < param.cr || j==j_rand ) //
                {
                    Tinput diff = actual_pop->getData(r[0],j) + actual_pop->getData(r[1],j) - actual_pop->getData(r[2],j) - actual_pop->getData(r[3],j);
                    vector_u[j] = actual_pop->getMinCostData()[j] + param.scale_f*diff;
                    keepInRange(vector_u[j]);
                }
                else
                {
                    vector_u[j] = actual_pop->getData(i,j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de9_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));

    return best_cost[param.t_max-1];
}

template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS8_DE_randbest_1_bin(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 2;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            //trial vector u;
            Tinput vector_u[param.dim];

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                if(ms_random.genrand_real_range(0,1) < param.cr || j==j_rand ) //
                {
                    Tinput diff1 = actual_pop->getMinCostData()[j]  - actual_pop->getData(i,j) ;
                    Tinput diff2 = actual_pop->getData(r[0],j) - actual_pop->getData(r[1],j);
                    vector_u[j] = actual_pop->getData(i,j)   + param.scale_lambda*diff1 + param.scale_f*diff2;
                    keepInRange(vector_u[j]);
                }
                else
                {
                    vector_u[j] = actual_pop->getData(i,j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de8_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));

    return best_cost[param.t_max-1];
 
}

/// @brief save the indivuo with best cost in a file 
/// 
/// this method is used to save the best cost of each iteration, the saved data are:
/// the cost and the n dimensions elements of the individuo.
/// @param best_cost ///< best cost
/// @param *best_vector ///< pointer to the best individuo
/// @param result_file ///< name of the file to be saved
template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>:: saveResult(Toutput best_cost, Tinput* best_vector, std::string result_file)
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



template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS1_DE_best_1_exp(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 2;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            int n_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim);// for crosover
            //trial vector u;
            Tinput vector_u[param.dim];
            bool stop_cross = false;

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                int new_j = (j+n_rand)%param.dim;

                if(ms_random.genrand_real_range(0,1) > param.cr)
                {
                    stop_cross = true;
                }
                
                if(!stop_cross || new_j==j_rand ) //
                {
                    
                    vector_u[new_j] = actual_pop->getMinCostData()[new_j] + param.scale_f*(actual_pop->getData(r[0],new_j) - actual_pop->getData(r[1],new_j));
                    keepInRange(vector_u[new_j]);
                }
                else
                {
                    vector_u[new_j] = actual_pop->getData(i,new_j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de1_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));

    return best_cost[param.t_max-1];
}




template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS2_DE_rand_1_exp(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 3;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            int n_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim);// for crosover
            //trial vector u;
            Tinput vector_u[param.dim];
            bool stop_cross = false;

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                int new_j = (j+n_rand)%param.dim;

                if(ms_random.genrand_real_range(0,1) > param.cr)
                {
                    stop_cross = true;
                }
                
                if(!stop_cross || new_j==j_rand ) //
                {
                    
                    vector_u[new_j] = actual_pop->getData(r[2],new_j) + param.scale_f*(actual_pop->getData(r[0],new_j) - actual_pop->getData(r[1],new_j));
                    keepInRange(vector_u[new_j]);
                }
                else
                {
                    vector_u[new_j] = actual_pop->getData(i,new_j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de2_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
 
    return best_cost[param.t_max-1];
}



template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS3_DE_randbest_1_exp(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 2;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            int n_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim);// for crosover
            //trial vector u;
            Tinput vector_u[param.dim];
            bool stop_cross = false;

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                int new_j = (j+n_rand)%param.dim;

                if(ms_random.genrand_real_range(0,1) > param.cr)
                {
                    stop_cross = true;
                }
                
                if(!stop_cross || new_j==j_rand ) //
                {
                    
                    Tinput diff1 = actual_pop->getMinCostData()[new_j]  - actual_pop->getData(i,new_j) ;
                    Tinput diff2 = actual_pop->getData(r[0],new_j) - actual_pop->getData(r[1],new_j);
                    vector_u[new_j] = actual_pop->getData(i,new_j)   + param.scale_lambda*diff1 + param.scale_f*diff2;
                    keepInRange(vector_u[new_j]);

                }
                else
                {
                    vector_u[new_j] = actual_pop->getData(i,new_j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de3_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
 
    return best_cost[param.t_max-1];
}



template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS4_DE_best_2_exp(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 4;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            int n_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim);// for crosover
            //trial vector u;
            Tinput vector_u[param.dim];
            bool stop_cross = false;

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                int new_j = (j+n_rand)%param.dim;

                if(ms_random.genrand_real_range(0,1) > param.cr)
                {
                    stop_cross = true;
                }
                
                if(!stop_cross || new_j==j_rand ) //
                {
                    
                    Tinput diff = actual_pop->getData(r[0],new_j) + actual_pop->getData(r[1],new_j) - actual_pop->getData(r[2],new_j) - actual_pop->getData(r[3],new_j);
                    vector_u[new_j] = actual_pop->getMinCostData()[new_j] + param.scale_f*diff;
                    keepInRange(vector_u[new_j]);
                }
                else
                {
                    vector_u[new_j] = actual_pop->getData(i,new_j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de4_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
    
    return best_cost[param.t_max-1];

}



template <class Tinput, class Toutput>
Toutput DifferentialEvolution<Tinput, Toutput>::runS5_DE_rand_2_exp(int function_id)
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug(actual_pop->printPopulation());
    //get the best value by cost
    actual_pop->evaluateCost(function_id);
    actual_pop->calculateMinCost();
    
    int nr = 5;
    int r[nr];


    for (int gen =0; gen<param.t_max; gen++) // for each generation
    {
        for (int i=0; i<param.pop_size; i++) // for each vector
        {
            // generate many random r
            randomR(r,nr,i);
            debug(std::cout<<"---random r---"<<std::endl);
            debug(printArray<int>(r,nr,' '));
            
            //generate random j
            int j_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim); //mutation

            int n_rand=(int)ms_random.genrand_real_range_ex_high(0,param.dim);// for crosover
            //trial vector u;
            Tinput vector_u[param.dim];
            bool stop_cross = false;

            for (int j=0; j<param.dim; j++) // for each element or parameter
            {
                int new_j = (j+n_rand)%param.dim;

                if(ms_random.genrand_real_range(0,1) > param.cr)
                {
                    stop_cross = true;
                }
                
                if(!stop_cross || new_j==j_rand ) //
                {

                    Tinput diff = actual_pop->getData(r[0],new_j) + actual_pop->getData(r[1],new_j) - actual_pop->getData(r[2],new_j) - actual_pop->getData(r[3],new_j);
                    vector_u[new_j] = actual_pop->getData(r[4],new_j) + param.scale_f*diff;
                    keepInRange(vector_u[new_j]);
                
                }
                else
                {
                    vector_u[new_j] = actual_pop->getData(i,new_j);
                }
                
            }

            debug(std::cout<<"---vector u---"<<std::endl);
            debug(printArray<Tinput>(vector_u, param.dim, ' '));

            // select
            // evaluate cost
            Toutput  cost_u = PopulationBenchmark<Tinput,Toutput>::calcCostExt(function_id,vector_u,param.dim); //evaluate the selected benchmark function
            Toutput  cost_x = actual_pop->calcCost1Item(function_id,i);
            //debug_var(cost_u);
            //debug_var(cost_x);

            //store  data to new population  and store the best
            if(cost_u <= cost_x)
            {
                new_pop->setDataAndCost(vector_u,i,cost_u);
            }
            else
            {
                new_pop->setDataAndCost(actual_pop->getData(i), i, cost_x);
            }
            debug(new_pop->printPopulation());
            
        }
        new_pop->calculateMinCost();
        best_cost[gen] = new_pop->getMinCost();
        debugfile(saveResult(best_cost[gen],new_pop->getMinCostData(), "de5_f"+std::to_string(function_id)+"_iterations.csv"));
        //convert new_pop to actual_pop
        PopulationBenchmark<Tinput, Toutput>::swap(*actual_pop, *new_pop);
        debug(std::cout<<"---actual---"<<std::endl);
        debug(actual_pop->printPopulation());
        debug(std::cout<<"---new---"<<std::endl);
        debug(new_pop->printPopulation());
    }

    debug1(std::cout<<"---best cost---"<<std::endl);
    debug1(printArray<Toutput>(best_cost, param.t_max, '\n'));
 
    return best_cost[param.t_max-1];
}


    