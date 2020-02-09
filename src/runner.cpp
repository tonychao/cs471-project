#include "runner.h"


template <class Tinput, class Toutput>
Runner<Tinput,Toutput>::Runner( int n_runs)
{
    
    this->n_runs = n_runs;

    // allocate memory for array of solutions
    solutions = new Toutput[n_runs];

}

template <class Tinput,class Toutput>
Runner<Tinput,Toutput>::~Runner()
{

    delete [] solutions; // free memory for array of solutions

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::printSolutions()
{
    for (int i = 0; i < n_runs; i++) 
    {  
      
        std::cout<< solutions[i] <<std::endl;
        
    }

}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::runOptimization(int algorithm_id, std::string config_file,  int function_id, int dimensions, Tinput range_low, Tinput range_high)
{
    this->function_id = function_id;
    this->range_low = range_low;
    this->range_high = range_high;
    this->dimensions = dimensions;
    this->algorithm_id = algorithm_id;

    Clock clk; 

    switch(algorithm_id)
    {
        case 0: // genetic algorithm
        {//avoid cross initialization error
            
            fillGAParameterFromFile(config_file); 
            ga_parameters.bounds.l = range_low; 
            ga_parameters.bounds.u = range_high;
            ga_parameters.dim = this->dimensions;

            genetic_algorithm = new GeneticAlgorithm<Tinput, Toutput>(ga_parameters);
        
            debug(genetic_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = genetic_algorithm->findBestSolution(function_id, range_low, range_high);
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete genetic_algorithm;
        }   
        break;
        
        case 1 ... 10:
        {
            // commun initialization of DE
            fillDEParameterFromFile(config_file); 
            de_parameters.bounds.l = range_low; 
            de_parameters.bounds.u = range_high;
            de_parameters.dim = this->dimensions;

            switch(algorithm_id)
            {
                case 1:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS1_DE_best_1_exp(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                }
                break;

                case 2:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS2_DE_rand_1_exp(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                    
                }
                break;

                case 3:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS3_DE_randbest_1_exp(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                }
                break;

                case 4:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);
                    
                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS4_DE_best_2_exp(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                    
                }
                break;

                case 5:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS5_DE_rand_2_exp(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                }
                break;

                case 6:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS6_DE_best_1_bin(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                    
                }
                break;

                case 7:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS7_DE_rand_1_bin(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                }
                break;

                case 8:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS8_DE_randbest_1_bin(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                    
                }
                break;

                case 9:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);

                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS9_DE_best_2_bin(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();
                    
                    delete diff_evo;
                }
                break;

                case 10:
                {
                    diff_evo = new DifferentialEvolution<Tinput, Toutput>(de_parameters);
                    
                    clk.tic();
                    for (int i = 0; i < n_runs; i++)
                    {
                        solutions[i] = diff_evo->runS10_DE_rand_2_bin(function_id);
                    }
                    
                    computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
                    saveStatistic();

                    delete diff_evo;
                }
                break;
            }
            
        }
        break;
    }


    debug(std::cout<<"solutions: " <<std::endl);
    debug(printSolutions());
    

}

template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillGAParameterFromFile(std::string config_filename)
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
            ga_parameters.t_max = std::stoi(row[0]); 
            ga_parameters.cr = std::stod(row[1]); 
            ga_parameters.m.rate = std::stod(row[2]);
            ga_parameters.m.range = std::stod(row[3]); 
            ga_parameters.m.precision = std::stod(row[4]);
            ga_parameters.er = std::stod(row[5]);
            ga_parameters.ns = std::stod(row[6]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout << config_filename << "config file not found\n"; 

    fin.close();
    
}


template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillDEParameterFromFile(std::string config_filename)
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
            de_parameters.t_max = std::stoi(row[0]); 
            de_parameters.cr = std::stod(row[1]); 
            de_parameters.scale_f = std::stod(row[2]);
            de_parameters.scale_lambda= std::stod(row[3]); 
            de_parameters.pop_size = std::stod(row[4]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::computeStatistic(double time_ms)
{
    stat_analysis.time_ms = time_ms;
    
    // First we sort the array 
    std::sort(solutions, solutions + n_runs);
    
    // -----------  find the median
    // check for even case 
    if (n_runs % 2 != 0) 
    {
        stat_analysis.median = solutions[n_runs / 2]; 
    }
    else
    {
        stat_analysis.median = (solutions[(n_runs - 1) / 2] + solutions[n_runs / 2]) / 2.0;
    }
    
    
    // ----------- calculate range
    stat_analysis.range_max = solutions[n_runs-1];
    stat_analysis.range_min = solutions[0];
    stat_analysis.range = stat_analysis.range_max - stat_analysis.range_min;
    

    // ----------- calculate mean
    double mean = 0.0;
    for(int i=0 ; i<n_runs; i++)
    {
        mean+=solutions[i];
    }
    mean = mean/n_runs;
    stat_analysis.mean = mean;
    

    // ---------- calculate std
    double std_dev = 0.0;
    for(int i=0 ; i<n_runs; i++)
    {
        std_dev += (solutions[i] - mean)*(solutions[i] - mean);
    }
    std_dev /= n_runs;
    std_dev = sqrt(std_dev);
    stat_analysis.std_dev = std_dev;


}

template <class Tinput,class Toutput>
void Runner<Tinput,Toutput>::saveStatistic()
{
    // file pointer 
    std::fstream fout; 
  
    // opens an existing csv file or creates a new file. 
    std::string file_name = "a"+std::to_string(algorithm_id)+"_f"+std::to_string(function_id)+".csv";
  
   
    if(!std::ifstream(file_name)) // if file not exist, create new file with header
    {
        fout.open(file_name, std::fstream::in | std::fstream::out | std::fstream::app);
        fout    << "algorithm_id" << ","
                << "function_id" << ","
                << "n_runs" << ","
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
        fout    << algorithm_id << ","
                << function_id << ","
                << n_runs << ","
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
        fout    << algorithm_id << ","
                << function_id << ","
                << n_runs << ","
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

