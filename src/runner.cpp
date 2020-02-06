
#include "runner.h"


template <class Tinput, class Toutput>
Runner<Tinput,Toutput>::Runner(int dimensions, int n_samples)
{
    this->dimensions = dimensions;
    this->n_samples = n_samples;


    // allocate memory for the vectors
    vectors = new Tinput*[n_samples];

    for (int i = 0; i<n_samples; i++)
    {
        vectors[i] = new Tinput[dimensions];
    }

    // allocate memory for array of solutions
    solutions = new Toutput[n_samples];

    // random generator seed
    mersenne_twister.init_genrand(time(0));
}

template <class Tinput,class Toutput>
Runner<Tinput,Toutput>::~Runner()
{

    // free memory for the vectors
    if(vectors)
    {
        for (int i = 0; i<n_samples; i++)
        {
            if(vectors[i])
            delete [] vectors[i]; //delete array
        }

        delete [] vectors; // delete array of array
    }

    delete [] solutions; // free memory for array of solutions


 

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::fillVectorsRandom(Tinput range_low, Tinput range_high)
{
    
    for (int i = 0; i < n_samples; i++) 
    {  
        for (int j = 0; j < dimensions; j++) 
        {

            vectors[i][j] = mersenne_twister.genrand_real_range(range_low,range_high);
           
        }
        
    }

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::printVectors()
{
    for (int i = 0; i < n_samples; i++) 
    {  
        for (int j = 0; j < dimensions; j++) 
        {
            std::cout<< vectors[i][j] << " ";
        }
        std::cout<<std::endl;
    }

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::printSolutions()
{
    for (int i = 0; i < n_samples; i++) 
    {  
      
        std::cout<< solutions[i] <<std::endl;
        
    }

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::run(int function_id,Tinput range_low, Tinput range_high)
{
    this->function_id = function_id;
    this->range_low = range_low;
    this->range_high = range_high;

    fillVectorsRandom(range_low, range_high);

    
    clock_t start_c, stop_c;
    start_c = clock();
    for (int i = 0; i < n_samples; i++)
    {
        
        //pointer to member function https://www.codeguru.com/cpp/cpp/article.php/c17401/C-Tutorial-PointertoMember-Function.htm
        Functions1<Tinput,Toutput> x;
        typename Functions1<Tinput,Toutput>::function_pointer fp = functions.getFunctionById(function_id);
        solutions[i] = (x.*fp)(vectors[i], dimensions); 

    }
    stop_c = clock();
    double clock_time;
    clock_time = ((double)stop_c - (double)start_c)/CLOCKS_PER_SEC; // CLOCKS_PER_SEC=1000000 in linux   CLOCKS_PER_SEC=1000 in windows
    clock_time *=1000.0; // convert to milisecond
    
    computeStatistic(clock_time); // compute all the statistical analysis beyond cpu time in ms
    saveStatistic();

}
template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::runOptimization(int algorithm_id, std::string config_file, std::string result_file, int function_id, Tinput range_low, Tinput range_high)
{
    this->function_id = function_id;
    this->range_low = range_low;
    this->range_high = range_high;

  
    // to do: identify the optimization algorithm by id
    // read some of the optimization algorithm parameters from file
    // fill the parameter struct from optimization algorithm class 
    GAInputParameter<Tinput> ga_parameters;
    fillGAParameterFromFile(config_file, ga_parameters); // pass by reference
    ga_parameters.bounds.l = range_low; 
    ga_parameters.bounds.u = range_high;
    ga_parameters.dim = this->dimensions;
    //ga_parameters.ns = this->n_samples;

    
    genetic_algorithm = new GeneticAlgorithm<Tinput, Toutput>(ga_parameters);
   
    debug(genetic_algorithm->printInputPopulation());
    
    clock_t start_c, stop_c;
    start_c = clock();
    for (int i = 0; i < n_samples; i++)
    {
        Toutput best_solution = genetic_algorithm->findBestSolution(function_id, range_low, range_high);
        solutions[i] = best_solution;
        //std::cout<<"best solution: " << best_solution<<std::endl;
    }
    
    stop_c = clock();
    double clock_time;
    clock_time = ((double)stop_c - (double)start_c)/CLOCKS_PER_SEC; // CLOCKS_PER_SEC=1000000 in linux   CLOCKS_PER_SEC=1000 in windows
    clock_time *=1000.0; // convert to milisecond
    
    computeStatistic(clock_time); // compute all the statistical analysis beyond cpu time in ms
    saveStatistic();

    delete genetic_algorithm; // this was deleted in the wrong place: destructor
    std::cout<<"solutions: " <<std::endl;
    printSolutions();
    

}

template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillGAParameterFromFile(std::string config_filename, GAInputParameter<Tinput> &parameters)
{
    // File pointer 
    std::fstream fin; 
  
    // Open an existing file 
    fin.open(config_filename, std::ios::in); 
  

    int count = 0; 


    // Read the Data from the file 
    // as String Vector 
    std::vector<std::string> row; 
    std::string line, word, temp; 

    while (fin) { 
        count++;
        row.clear(); 
         
        // read an entire row and 
        // store it in a string variable 'line' 
        std::getline(fin, line); 

        // used for breaking words 
        std::stringstream s(line); 
        
        // read every column data of a row and 
        // store it in a string variable, 'word' 
        
        while (std::getline(s, word, ',')) { 
  
            // add all the column data 
            // of a row to a vector 
            row.push_back(word); 
           
        } 
                
  
  
        // Compare the roll number 
        if (count==2) { 
  
            parameters.t_max = std::stoi(row[0]); 
            parameters.cr = std::stod(row[1]); 
            parameters.m.rate = std::stod(row[2]);
            parameters.m.range = std::stod(row[3]); 
            parameters.m.precision = std::stod(row[4]);
            parameters.er = std::stod(row[5]);
            parameters.ns = std::stod(row[6]);
            
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout << "config file not found\n"; 

    fin.close();
    
    
}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::computeStatistic(double time_ms)
{
    stat_analysis.time_ms = time_ms;
    
    // First we sort the array 
    std::sort(solutions, solutions + n_samples);
    
    // -----------  find the median
    // check for even case 
    if (n_samples % 2 != 0) 
    {
        stat_analysis.median = solutions[n_samples / 2]; 
    }
    else
    {
        stat_analysis.median = (solutions[(n_samples - 1) / 2] + solutions[n_samples / 2]) / 2.0;
    }
    
    
    // ----------- calculate range
    stat_analysis.range_max = solutions[n_samples-1];
    stat_analysis.range_min = solutions[0];
    stat_analysis.range = stat_analysis.range_max - stat_analysis.range_min;
    

    // ----------- calculate mean
    double mean = 0.0;
    for(int i=0 ; i<n_samples; i++)
    {
        mean+=solutions[i];
    }
    mean = mean/n_samples;
    stat_analysis.mean = mean;
    

    // ---------- calculate std
    double std_dev = 0.0;
    for(int i=0 ; i<n_samples; i++)
    {
        std_dev += (solutions[i] - mean)*(solutions[i] - mean);
    }
    std_dev /= n_samples;
    std_dev = sqrt(std_dev);
    stat_analysis.std_dev = std_dev;
    

    /*
    std::cout << "\ntime [ms] : " << stat_analysis.time_ms << "\n";
    std::cout << "median: " << stat_analysis.median << "\n";
    std::cout << "range : " << stat_analysis.range << "\n";
    std::cout << "mean : " << stat_analysis.mean << "\n";
    std::cout << "std dev : " << stat_analysis.std_dev << "\n";
    */

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::saveStatistic()
{
    // file pointer 
    std::fstream fout; 
  
    // opens an existing csv file or creates a new file. 
    std::string file_name = "f"+std::to_string(function_id)+"_"+std::to_string(n_samples)+"_"+std::to_string(dimensions)+".csv";
  
   
    if(!std::ifstream(file_name)) // if file not exist, create new file with header
    {
        fout.open(file_name, std::fstream::in | std::fstream::out | std::fstream::app);
        fout << "function_id" << ","
                << "n_samples" << ","
                << "dimensions" << ","
                << "range_low" << ","
                << "range_high" << ","
                << "mean" << ", "
                << "std_dev" << ", "
                << "range" << ", "
                << "median" << ", "
                << "time_ms" << ", "
                << "range_min" << ", " //new 
                << "range_max" << ", " //new
                << "\n"; 
        fout << function_id << ","
                << n_samples << ","
                << dimensions << ","
                << range_low << ","
                << range_high << ","
                << stat_analysis.mean << ", "
                << stat_analysis.std_dev << ", "
                << stat_analysis.range << ", "
                << stat_analysis.median << ", "
                << stat_analysis.time_ms << ", "
                << stat_analysis.range_min << ", " //new 
                << stat_analysis.range_max << ", " //new
                << "\n"; 
        fout.close();
     
    }
    else
    {
        fout.open(file_name, std::fstream::in | std::fstream::out | std::fstream::app);
        fout << function_id << ","
                << n_samples << ","
                << dimensions << ","
                << range_low << ","
                << range_high << ","
                << stat_analysis.mean << ", "
                << stat_analysis.std_dev << ", "
                << stat_analysis.range << ", "
                << stat_analysis.median << ", "
                << stat_analysis.time_ms << ", "
                << stat_analysis.range_min << ", " //new 
                << stat_analysis.range_max << ", " //new
                << "\n"; 
        fout.close();
    }

    
}

