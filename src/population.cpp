#include "population.h"

template <class Tinput, class Toutput>
Population<Tinput, Toutput>::Population(int n_items, int dimension)
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
}

template <class Tinput, class Toutput>
Population<Tinput, Toutput>::~Population()
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

template <class Tinput, class Toutput>
void Population<Tinput, Toutput>::fillWithRandom(Tinput range_low, Tinput range_high)
{       
    for (int i = 0; i < n_items; i++) 
    {  
        for (int j = 0; j < dimension; j++) 
        {
                data[i][j] = random_ms.genrand_real_range(range_low,range_high);
        
        }
        
    }
}

template <class Tinput, class Toutput>
void Population<Tinput, Toutput>::printPopulation()
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
}

template <class Tinput, class Toutput>
Tinput Population<Tinput, Toutput>::getData(int i, int j)
{
    return data[i][j];
}

template <class Tinput, class Toutput>
Tinput* Population<Tinput, Toutput>::getData(int i)
{
    return data[i];
}

template <class Tinput, class Toutput>
void Population<Tinput, Toutput>::setData(Tinput *item, int i)
{
    for(int j=0; j<dimension; j++)
    {
        data[i][j] = item[j];
    }
}

template <class Tinput, class Toutput>
void Population<Tinput, Toutput>::swap(Population& A, Population& B)
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