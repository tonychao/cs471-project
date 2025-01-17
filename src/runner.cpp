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

        case 11:
        {
            fillPSAParameterFromFile(config_file); 
            psa_parameters.bounds.l = range_low; 
            psa_parameters.bounds.u = range_high;
            psa_parameters.dim = this->dimensions;

            psa_algorithm = new ParticleSwarm<Tinput, Toutput>(psa_parameters);
        
            debug(psa_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = psa_algorithm->run(function_id,false); // variable_weight = false
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete psa_algorithm;
        }
        break;

        case 12:
        {
            fillSCAParameterFromFile(config_file); 
            sca_parameters.bounds.l = range_low; 
            sca_parameters.bounds.u = range_high;
            sca_parameters.dim = this->dimensions;

            sca_algorithm = new SineCosineAlgorithm<Tinput, Toutput>(sca_parameters);
        
            debug(sca_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = sca_algorithm->run(function_id);
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete sca_algorithm;
        }
        break;

        case 13:
        {
            fillBAParameterFromFile(config_file); 
            ba_parameters.bounds.l = range_low; 
            ba_parameters.bounds.u = range_high;
            ba_parameters.dim = this->dimensions;

            ba_algorithm = new BatAlgortihm<Tinput, Toutput>(ba_parameters);
        
            debug(ba_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = ba_algorithm->run(function_id); 
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete ba_algorithm;
        }
        break;

        case 14:
        {
            fillDBAParameterFromFile(config_file); 
            dba_parameters.bounds.l = range_low; 
            dba_parameters.bounds.u = range_high;
            dba_parameters.dim = this->dimensions;

            dba_algorithm = new DirectionalBatAlgortihm<Tinput, Toutput>(dba_parameters);
        
            debug(dba_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = dba_algorithm->run(function_id); 
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete dba_algorithm;
        }
        break;

        case 15:
        {
            fillALOParameterFromFile(config_file); 
            alo_parameters.bounds.l = range_low; 
            alo_parameters.bounds.u = range_high;
            alo_parameters.dim = this->dimensions;

            alo_algorithm = new AntLionOptimizer<Tinput, Toutput>(alo_parameters);
        
            debug(alo_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = alo_algorithm->run(function_id); 
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete alo_algorithm;
        }
        break;

        case 16:
        {
            fillQABCParameterFromFile(config_file); 
            qabc_parameters.bounds.l = range_low; 
            qabc_parameters.bounds.u = range_high;
            qabc_parameters.dim = this->dimensions;

            qabc_algorithm = new QuickArtificialBeeColony<Tinput, Toutput>(qabc_parameters);
        
            debug(qabc_algorithm->printInputPopulation());
            
            clk.tic();
            for (int i = 0; i < n_runs; i++)
            {
                solutions[i] = qabc_algorithm->run(function_id); 
                //std::cout<<"best solution: " << best_solution<<std::endl;
            }
            
            computeStatistic(clk.tac()); // compute all the statistical analysis beyond cpu time in ms
            saveStatistic();

            delete qabc_algorithm;
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
            ga_parameters.ns = std::stoi(row[6]);
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
            de_parameters.pop_size = std::stoi(row[4]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}


template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillPSAParameterFromFile(std::string config_filename)
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
            psa_parameters.t_max = std::stoi(row[0]); 
            psa_parameters.c1 = std::stod(row[1]); 
            psa_parameters.c2 = std::stod(row[2]);
            psa_parameters.w = std::stod(row[3]);
            psa_parameters.w_min = std::stod(row[4]);
            psa_parameters.pop_size = std::stoi(row[5]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}



template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillBAParameterFromFile(std::string config_filename)
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
            ba_parameters.t_max = std::stoi(row[0]); 
            ba_parameters.r0 = std::stod(row[1]); 
            ba_parameters.A0_min = std::stod(row[2]);
            ba_parameters.A0_max = std::stod(row[3]);
            ba_parameters.alfa = std::stod(row[4]);
            ba_parameters.gamma = std::stod(row[5]);
            ba_parameters.f_min = std::stod(row[6]);
            ba_parameters.f_max = std::stod(row[7]);
            ba_parameters.ratio_best = std::stod(row[8]);
            ba_parameters.pop_size = std::stoi(row[9]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}



template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillDBAParameterFromFile(std::string config_filename)
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
            dba_parameters.t_max = std::stoi(row[0]); 
            dba_parameters.r0 = std::stod(row[1]); 
            dba_parameters.rinf = std::stod(row[2]);
            dba_parameters.A0 = std::stod(row[3]);
            dba_parameters.Ainf = std::stod(row[4]);
            dba_parameters.f_min = std::stod(row[5]);
            dba_parameters.f_max = std::stod(row[6]);
            dba_parameters.pop_size = std::stoi(row[7]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}


template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillALOParameterFromFile(std::string config_filename)
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
            alo_parameters.t_max = std::stoi(row[0]);
            alo_parameters.pop_size = std::stoi(row[1]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}


template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillQABCParameterFromFile(std::string config_filename)
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
            qabc_parameters.t_max = std::stoi(row[0]);
            qabc_parameters.l = std::stoi(row[1]);
            qabc_parameters.r =  std::stod(row[2]);
            qabc_parameters.pop_size = std::stoi(row[3]);
            break; 
        } 
        
    } 
   
    if(count!=2)
        std::cout <<config_filename<< " config file not found\n"; 

    fin.close();
    
}



template <class Tinput, class Toutput>
void Runner<Tinput,Toutput>::fillSCAParameterFromFile(std::string config_filename)
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
            sca_parameters.t_max = std::stoi(row[0]); 
            sca_parameters.a = std::stod(row[1]); 
            sca_parameters.r3 = std::stod(row[2]);
            sca_parameters.pop_size = std::stod(row[3]);
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

