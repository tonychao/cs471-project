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
}

template <class Tinput, class Toutput>
DifferentialEvolution<Tinput, Toutput>::~DifferentialEvolution()
{

    if(actual_pop)
    {
        delete actual_pop;
    }

    if(new_pop)
    {
        delete new_pop;
    }
}


template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>::randomR(int *r, int n, int current_i)
{
    int a;
    for (int i =0; i<n; i++)
    {
        while(true)
        {
            a = (int)ms_random.genrand_real_range_ex_high(0,param.dim);
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

template <class Tinput, class Toutput>
void DifferentialEvolution<Tinput, Toutput>::runS7_DE_rand_1_bin()
{
    
    actual_pop->fillWithRandom(param.bounds.l, param.bounds.u);
    debug1(actual_pop->printPopulation());
    int nr = 10;
    int r[nr];

    randomR(r,nr,5);
    std::cout<<"---random r---"<<std::endl;
    printArray<int>(r,nr,'\n');
}

