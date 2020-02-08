#ifndef _POPULATION_H_
#define _POPULATION_H_

#include "../lib/mt19937ar_class.h"
#include <time.h>       //time
#include <iostream>
#include "functions1.h"
#include <algorithm> //sort
#include "util.h"


template <class Tinput, class Toutput>
class Population
{
    private:
    MersenneTwister random_ms; ///< mersenne twister random generator
    
    protected:
    Tinput** data;
    int dimension;
    int n_items; ///< total number of items in data

    public:
    Population(int n_items, int dimension)
    {
        this->n_items = n_items;
        this->dimension = dimension;
        
        // allocate memory for the data
        data = new Tinput*[n_items];

        for (int i = 0; i<n_items; i++)
        {
            data[i] = new Tinput[dimension];
        }

        // random generator seed
        random_ms.init_genrand(time(0));
    };
    ~Population()
    {
        // free memory for data
        if(data)
        {
            for (int i = 0; i<n_items; i++)
            {
                if(data[i])
                delete [] data[i]; //delete array
            }

            delete [] data; // delete array of array
        }
    }

    void fillWithRandom(Tinput range_low, Tinput range_high)
    {       
        for (int i = 0; i < n_items; i++) 
        {  
            for (int j = 0; j < dimension; j++) 
            {
                 data[i][j] = random_ms.genrand_real_range(range_low,range_high);
            
            }
            
        }
    };

    void printPopulation()
    {
        std::cout<< "---population---" <<std::endl;
        for (int i = 0; i < n_items; i++) 
        {  
            for (int j = 0; j < dimension; j++) 
            {
                std::cout<< data[i][j] << " ";
            }
            std::cout<<std::endl;
        }
    };

    Tinput getData(int i, int j)
    {
        return data[i][j];
    };

    Tinput* getData(int i)
    {
        return data[i];
    };

    void setData(Tinput *item, int i)
    {
        for(int j=0; j<dimension; j++)
        {
            data[i][j] = item[j];
        }
    }
/*
    int getDimension()
    {
        return dimension;
    }
    int getNItems()
    {
        return n_items;
    }
    Tinput** getDataPointer()
    {
        return data;
    }

    void setDimension(int dimension)
    {
        this->dimension =  dimension;
    }
    void setNItems(int n_items)
    {
        this->n_items = n_items;
    }
*/
    static void swap(Population& A, Population& B)
    {
        if (A.n_items == B.n_items && A.dimension == B.dimension)
        {
            int temp_dim, temp_n;
            Tinput** temp_pointer;
            
            temp_n = A.n_items;
            temp_dim = A.dimension;
            temp_pointer = A.data;

            A.n_items = B.n_items;
            A.dimension = B.dimension;
            A.data = B.data;

            B.n_items = temp_n;
            B.dimension = temp_dim;
            B.data = temp_pointer;
        }
        else
        {
            std::cout<<"Cannot Swap, data with diference size and dimension"<<std::endl;
        } 

    }

    

};

template <class Tinput, class Toutput>
class PopulationBenchmark: public Population <Tinput,Toutput>
{
    private:

    Toutput* cost;  ///< array that contains the cost of each item
    Toutput min_cost;
    int min_cost_i;
    int* asc_index;
        // ... for sort
    
    //https://stackoverflow.com/questions/1902311/problem-sorting-using-member-function-as-comparator
    struct doCompare
    { 
        const PopulationBenchmark& m_info;
        doCompare( const PopulationBenchmark& info ) : m_info(info) { } // only if you really need the object state
        bool operator()( const int & i1, const int & i2  )
        { 
                // comparison code using m_info
                return (m_info.cost[i1]<m_info.cost[i2]);
        }
    };

    public:

    PopulationBenchmark(int n_items, int dimension) : Population<Tinput,Toutput>(n_items,dimension)
    {
        
        // allocate memory for array for cost
        cost = new Toutput[this->n_items];


        // allocate memory and fill the population index for sorting
        asc_index = new int[this->n_items];
        for(int i=0; i<this->n_items; i++)
        {
            asc_index[i] = i;
        }

    }

    ~PopulationBenchmark()
    {
         // free array of cost
        if(cost)
            delete[] cost;

        // free array of index
        if(asc_index)
            delete[] asc_index;
    
    };

 

    void evaluateCost(int function_id)
    {
        
        Functions1<Tinput,Toutput> functions;
        typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
        for (int i = 0; i<this->n_items; i++)
        {
            cost[i] = (functions.*fp)(this->data[i], this->dimension); //evaluate the selected benchmark function
        }
    
    };

    Toutput calcCost1Item(int function_id, int i)
    {
        
        Functions1<Tinput,Toutput> functions;
        typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);

        return  (functions.*fp)(this->data[i], this->dimension); //evaluate the selected benchmark function
        
    
    };

    static Toutput calcCostExt(int function_id, Tinput *item, int dim)
    {
        Functions1<Tinput,Toutput> functions;
        typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);

        return  (functions.*fp)(item, dim); //evaluate the selected benchmark function
    }
    


    void sortIndexByCostAsc()
    {
        std::sort(asc_index, asc_index + this->n_items, doCompare(*this));
    };
    
    void saveBest(Toutput best_cost, Tinput *best_individuo);
    void printCost()
    {
        std::cout<<"--- cost ---"<<std::endl;
        printArray<Toutput>(cost, this->n_items, '\n');
    };
    void printIndex()
    {
        std::cout<<"--- index ---"<<std::endl;
        printArray<int>(asc_index, this->n_items, '\n');
    };


    void setDataAndCost(Tinput *item, int i, Toutput c)
    {
        for(int j=0; j<this->dimension; j++)
        {
            this->data[i][j] = item[j];
        }
        this->cost[i] = c;

    }

    void calculateMinCost()
    {
        
        min_cost = this->cost[0];
        min_cost_i=0;

        for (int i=1; i<this->n_items; i++)
        {
            if(this->cost[i]<min_cost)
            {
                min_cost = this->cost[i];
                min_cost_i = i;
            }
        }
    }

    Toutput getMinCost()
    {
        return min_cost;
    }

    Tinput* getMinCostData()
    {
        return this->data[min_cost_i];
    }

    static void swap(PopulationBenchmark& A, PopulationBenchmark& B)
    {
        if (A.n_items == B.n_items && A.dimension == B.dimension)
        {
            int temp_dim, temp_n;
            Tinput** temp_pointer;

            Toutput* temp_cost;  ///< array that contains the cost of each item
            Toutput temp_min_cost;
            int temp_min_cost_i;
            int* temp_asc_index;
                    
            temp_n = A.n_items;
            temp_dim = A.dimension;
            temp_pointer = A.data;
            temp_cost = A.cost;
            temp_min_cost = A.min_cost;
            temp_min_cost_i = A.min_cost_i;
            temp_asc_index = A.asc_index;

            A.n_items = B.n_items;
            A.dimension = B.dimension;
            A.data = B.data;
            A.cost = B.cost;
            A.min_cost =  B.min_cost;
            A.min_cost_i = B.min_cost_i;
            A.asc_index = B.asc_index;

            B.n_items = temp_n;
            B.dimension = temp_dim;
            B.data = temp_pointer;
            B.cost = temp_cost;
            B.min_cost = temp_min_cost;
            B.min_cost_i = temp_min_cost_i;
            B.asc_index = temp_asc_index;



        }
        else
        {
            std::cout<<"Cannot Swap, data with diference size and dimension"<<std::endl;
        } 

    }

 
};
#endif