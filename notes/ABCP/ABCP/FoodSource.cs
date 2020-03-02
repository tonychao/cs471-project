/* Artificial Bee Colony Programming (ABCP) implemantation in C# programming language

Artificial Bee Colony Programming (ABCP) is an evolutionary computation based automatic programming and machine learning method and introduced to literature in 2012 by Karaboga et al.

Referance Papers:

D. Karaboga, C. Ozturk, N. Karaboga, B. Gorkemli, Artificial bee colony programming for symbolic regression. Inf. Sci. 209, 1-15 (2012)
B. Gorkemli , D. Karaboga , A quick semantic artificial bee colony programming (qsABCP) for symbolic regression, Inf. Sci. 502, 346-362 (2019) 


Copyright © 2019 Erciyes University, Intelligent Systems Research Group, Department of Computer Engineering

Contact:
Dervis Karaboga (karaboga@erciyes.edu.tr )
Beyza Gorkemli (bgorkemli@erciyes.edu.tr)       */

using System;


namespace ABCP
{
    public class FoodSource:ICloneable
    {
        public Node solutionTree;
        public double rawFitness;
        public int trialCounter = 0;
        public Node ProduceCandidate(FoodSource[] population, int currentIndex, Random rn, int maxDepth, double Pip)
        {
            int k = rn.Next(0, population.Length);
            while (k == currentIndex)
            {
                k = rn.Next(0, population.Length);
            }
            Node currentTree = (Node)this.solutionTree.Clone();
            bool isSuccessful = false;
            while (isSuccessful == false)
            {
                Methods.PerformNeighborhoodSearch(ref currentTree, population[k].solutionTree, rn, maxDepth, Pip, ref isSuccessful);
            }
            return currentTree;
        }
        public void CalculateRawFitness(Random rn, double lowerB, double upperB, double fitnessCaseNumber)
        {
            double targetValue = 0;
            double obtainedValue = 0;
            double totalError = 0;
            for (int i = 0; i < fitnessCaseNumber; i++)
            {
                double input = lowerB + rn.NextDouble() * (upperB - lowerB); 
                targetValue = Math.Pow(input, 5) + Math.Pow(input, 4) + Math.Pow(input, 3) + Math.Pow(input, 2) + input;
                obtainedValue = Methods.RunTheModel(this.solutionTree, input);
                totalError += Math.Abs(targetValue - obtainedValue);
            }
            this.rawFitness = totalError;
        }

        public object Clone()
        {
            FoodSource fs = new FoodSource();
            if (this.solutionTree != null)
            {
                fs.solutionTree = (Node)this.solutionTree.Clone();
            }
            fs.rawFitness = this.rawFitness;
            fs.trialCounter = this.trialCounter;
            return fs;           
        }
    }
}
