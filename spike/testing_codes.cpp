// form main.cpp
    PopulationBenchmark<float, float> p(5,5);
    p.fillWithRandom(0,10);
    p.printPopulation();
    p.evaluateCost(2);
    p.printCost();
    p.sortIndexByCostAsc();
    p.printIndex();