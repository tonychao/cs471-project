#include "population_benchmark.h"

template <class Tinput, class Toutput>
PopulationBenchmark<Tinput, Toutput>::PopulationBenchmark(int n_items, int dimension): Population<Tinput,Toutput>(n_items,dimension)
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

template <class Tinput, class Toutput>
PopulationBenchmark<Tinput, Toutput>::~PopulationBenchmark()
{
    // free array of cost
    if(cost)
        delete[] cost;
    // free array of index
    if(asc_index)
        delete[] asc_index;

}

//https://stackoverflow.com/questions/1902311/problem-sorting-using-member-function-as-comparator
template <class Tinput, class Toutput>
struct PopulationBenchmark<Tinput, Toutput>::doCompare
{ 
    const PopulationBenchmark& m_info;
    doCompare( const PopulationBenchmark& info ) : m_info(info) { } // only if you really need the object state
    bool operator()( const int & i1, const int & i2  )
    { 
            // comparison code using m_info
            return (m_info.cost[i1]<m_info.cost[i2]);
    }
};


template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::evaluateCost(int function_id)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
    for (int i = 0; i<this->n_items; i++)
    {
        cost[i] = (functions.*fp)(this->data[i], this->dimension); //evaluate the selected benchmark function
    }
    
}

template <class Tinput, class Toutput>
Toutput PopulationBenchmark<Tinput, Toutput>::calcCost1Item(int function_id, int i)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);

    return  (functions.*fp)(this->data[i], this->dimension); //evaluate the selected benchmark function
}

template <class Tinput, class Toutput>
Toutput PopulationBenchmark<Tinput, Toutput>::calcCostExt(int function_id, Tinput *item, int dim)
{
    Functions1<Tinput,Toutput> functions;
    typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);

    return  (functions.*fp)(item, dim); //evaluate the selected benchmark function
}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::sortIndexByCostAsc()
{
    std::sort(asc_index, asc_index + this->n_items, doCompare(*this));
}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::printCost()
{
    std::cout<<"--- cost ---"<<std::endl;
    printArray<Toutput>(cost, this->n_items, '\n');
}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::printIndex()
{
    std::cout<<"--- index ---"<<std::endl;
    printArray<int>(asc_index, this->n_items, '\n');
}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::setDataAndCost(Tinput *item, int i, Toutput c)
{
    for(int j=0; j<this->dimension; j++)
    {
        this->data[i][j] = item[j];
    }
    this->cost[i] = c;

}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::calculateMinCost()
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

template <class Tinput, class Toutput>
Toutput PopulationBenchmark<Tinput, Toutput>::getMinCost()
{
    return min_cost;
}


template <class Tinput, class Toutput>
Tinput* PopulationBenchmark<Tinput, Toutput>::getMinCostData()
{
    return this->data[min_cost_i];
}

template <class Tinput, class Toutput>
void PopulationBenchmark<Tinput, Toutput>::swap(PopulationBenchmark& A, PopulationBenchmark& B)
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