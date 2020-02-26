# CS471 Optimization - Project

## Population based Optimization Algorithms

## Compiler in Windows
g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
C++14

compile release version: 
g++ .\src\*.cpp .\lib\*.cpp -o .\bin\release\project.exe

compile debug version: 
g++ -g .\src\*.cpp .\lib\*.cpp -o .\bin\release\project.exe

## Compiler in Linux
g++ (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
C++14

compile release version
make 

compile debug version
make debug

## run python code
the python code will detect the operation system between linux and windows automatically, then it will read all the config files and run the experiment, it will generate solutions.csv and many plots.

option 1: open run_expriment.ipynb with jupyter notebook

option 2: ipython run_experiment.ipynb (cannot visualize the plot)