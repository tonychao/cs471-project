
/* Artificial Bee Colony Programming (ABCP) implemantation in C# programming language

Artificial Bee Colony Programming (ABCP) is an evolutionary computation based automatic programming and machine learning method and introduced to literature in 2012 by Karaboga et al.

Referance Papers:

D. Karaboga, C. Ozturk, N. Karaboga, B. Gorkemli, Artificial bee colony programming for symbolic regression. Inf. Sci. 209, 1-15 (2012)
B. Gorkemli , D. Karaboga , A quick semantic artificial bee colony programming (qsABCP) for symbolic regression, Inf. Sci. 502, 346-362 (2019) 


Copyright © 2019 Erciyes University, Intelligent Systems Research Group, Department of Computer Engineering

Contact:
Dervis Karaboga (karaboga@erciyes.edu.tr )
Beyza Gorkemli (bgorkemli@erciyes.edu.tr)      */

using System;
using System.Windows.Forms;

namespace ABCP
{
    public partial class Form1 : Form
    {
        public string[] functionSet = { "+", "-", "*", "/"};
        public string[] terminalSet = { "x", "1" };
        public string[] functionAndTerminalSet;
        Random rn = new Random();
        public string functionString;
        public Form1()
        {
            InitializeComponent();
        }
        public void WriteTheFunction(Node solution)
        {
            if (solution != null)
            {
                functionString = functionString + "(";
                WriteTheFunction(solution.left);
                functionString = functionString + solution.value;
                WriteTheFunction(solution.right);
                functionString = functionString + ")";
            }
        }
        public Node CreateTreeWithGrowMethod(Node current, int level, int maxDepth)
        {
            int f = rn.Next(0, functionAndTerminalSet.Length);
            if ((f < terminalSet.Length) || level == maxDepth)
            {
                if (level == maxDepth)
                { f = rn.Next(0, terminalSet.Length); }
                current.type = "terminal";
                current.value = functionAndTerminalSet[f];
            }
            else
            {
                current.type = "function";
                current.value = functionAndTerminalSet[f];
                level++;
                current.left = new Node();
                current.right = new Node();
                current.left = CreateTreeWithGrowMethod(current.left, level, maxDepth);
                current.right = CreateTreeWithGrowMethod(current.right, level, maxDepth);                
            }
            return current;
        }
        public Node CreateTreeWithFullMethod(Node current, int level, int maxDepth)
        {                        
            if (level == maxDepth)
            {
                int f = rn.Next(0, terminalSet.Length);
                current.type = "terminal";
                current.value = terminalSet[f];
            }
            else
            {
                int f = rn.Next(0, functionSet.Length);
                current.type = "function";
                current.value = functionSet[f];
                level++;
                current.left = new Node();
                current.right = new Node();
                current.left = CreateTreeWithFullMethod(current.left, level, maxDepth);
                current.right = CreateTreeWithFullMethod(current.right, level, maxDepth);
            }
            return current;
        }
        public FoodSource[] InitializePopulationWithRampedHaH(int numberOfFoodSources, int initMaxDepth)
        {
            FoodSource[] population = new FoodSource[numberOfFoodSources];
            int numberOfTreePerDepthLevel = population.Length / (initMaxDepth - 1);
            int minDepthLevel = 2;
            for (int i = minDepthLevel; i < initMaxDepth + 1; i++)
            {
                for (int j = (i - 2) * numberOfTreePerDepthLevel; j < (i - 1) * numberOfTreePerDepthLevel; j++)
                {
                    Node tree = new Node();
                    population[j] = new FoodSource();
                    if (j < (((i - 2) * numberOfTreePerDepthLevel) + numberOfTreePerDepthLevel / 2))
                    {                        
                        if (j != 0)
                        {
                            bool isDifferent = false;
                            while (isDifferent == false)
                            {
                                population[j].solutionTree = CreateTreeWithGrowMethod(tree, 0, i);
                                isDifferent = Methods.ControlDifference(population, j);
                            }
                        }
                        else
                        {                           
                            population[j].solutionTree = CreateTreeWithGrowMethod(tree, 0, i);
                        }
                    }
                    else
                    {
                        bool isDifferent = false;
                        while (isDifferent == false)
                        {
                            population[j].solutionTree = CreateTreeWithFullMethod(tree, 0, i);
                            isDifferent = Methods.ControlDifference(population, j);
                        }
                    }
                }
            }
            return population;
        }
        public FoodSource RunABCP(int numberOfFoodSources, int initMaxDepth, int maxCycleNumber, int maxDepth, int limit, double Pip, int numberOfFitnessCases, double lowerB, double upperB )
        { 
            //Initialize the population
            FoodSource[] population = InitializePopulationWithRampedHaH(numberOfFoodSources, initMaxDepth);

            //Evaluate the solutions.
            for (int i = 0; i < population.Length; i++)
            {
                population[i].CalculateRawFitness(rn, lowerB, upperB, numberOfFitnessCases);
            }

            //Find the best solution.
            FoodSource best = (FoodSource)Methods.FindBestSource(population).Clone();
            int c = 0;
            while (c < maxCycleNumber)
            {
                //Employed bee phase
                for (int i = 0; i < population.Length; i++)
                {
                    FoodSource candidate = new FoodSource();
                    candidate.solutionTree = population[i].ProduceCandidate(population, i, rn, maxDepth, Pip);
                    candidate.CalculateRawFitness(rn, lowerB, upperB, numberOfFitnessCases);    
                    if (candidate.rawFitness < population[i].rawFitness)
                    {
                        population[i] = candidate;                        
                    }
                    else
                    {
                        population[i].trialCounter++;
                    }                    
                }

                //Calculate probabilities for the onlookers
                FoodSource populationBest = Methods.FindBestSource(population);
                double qualityOfPopulationBest = 1 / (1 + populationBest.rawFitness);
                double[] probabilies = new double[population.Length];
                for (int pr = 0; pr < probabilies.Length; pr++)
                {
                    double quality = 1 / (1 + population[pr].rawFitness);
                    probabilies[pr] = (((quality * 0.9) / qualityOfPopulationBest) + 0.1);
                }

                //Onlooker bee phase
                int t = 0;
                int m = 0;
                while (t < population.Length)
                {
                    if (rn.NextDouble() < probabilies[m])
                    {
                        t++;
                        FoodSource candidate = new FoodSource();
                        candidate.solutionTree = population[m].ProduceCandidate(population, m, rn, maxDepth, Pip);
                        candidate.CalculateRawFitness(rn, lowerB, upperB, numberOfFitnessCases);
                        if (candidate.rawFitness < population[m].rawFitness)
                        {
                            population[m] = candidate;
                        }
                        else
                        {
                            population[m].trialCounter++;
                        }
                    }
                    m++;
                    if (m == (population.Length))
                    { m = 0; }
                }
               
                //Update the best solution achieved so far.
                FoodSource currentPopulationBest = Methods.FindBestSource(population);
                if ((currentPopulationBest.rawFitness < best.rawFitness))
                {
                    best = (FoodSource)currentPopulationBest.Clone();
                }

                //Scout bee phase
                int k = Methods.FindMaxTrialCounterValuedSource(population);
                if (population[k].trialCounter > limit)
                {
                    FoodSource sourceOfScout = new FoodSource();
                    sourceOfScout.solutionTree = new Node();
                    bool isDifferent = false;
                    while (isDifferent == false)
                    {                       
                        sourceOfScout.solutionTree = CreateTreeWithGrowMethod(sourceOfScout.solutionTree,0, maxDepth);
                        isDifferent = Methods.ControlDifferenceForScout(population, sourceOfScout);
                    }
                    sourceOfScout.CalculateRawFitness(rn, lowerB, upperB, numberOfFitnessCases);                    
                    population[k] = sourceOfScout;                    
                }
                c++;
            }
            return best;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int limit = Convert.ToInt32(textBox2.Text);
            int colonySize = Convert.ToInt32(textBox1.Text);
            int maxNumberOfCycles = Convert.ToInt32(textBox3.Text);
            int numberOfFoodSources = colonySize / 2;
            int numberOfRuns = Convert.ToInt32(textBox4.Text);
            
            int initMaxDepth = Convert.ToInt32(textBox5.Text);            
            int maxDepth = Convert.ToInt32(textBox6.Text);
            int numberOfFitnessCases = Convert.ToInt32(textBox7.Text);
            double lowerB = Convert.ToDouble(textBox8.Text);
            double upperB = Convert.ToDouble(textBox9.Text);

            double Pip = 0.9;

            //functionAndTerminalSet will contain the items of both terminal and function sets.
            functionAndTerminalSet = new string[terminalSet.Length + functionSet.Length];
            for (int i = 0; i < terminalSet.Length; i++)
            {
                functionAndTerminalSet[i] = terminalSet[i];
            }
            int itemIndex = terminalSet.Length;
            for (int i = 0; i < functionSet.Length; i++)
            {
                functionAndTerminalSet[itemIndex] = functionSet[i];
                itemIndex++;
            }
            
            FoodSource[] runBests = new FoodSource[numberOfRuns];            
            for (int r = 0; r < numberOfRuns; r++)
            {                                 
                //run the ABCP
                runBests[r] = RunABCP(numberOfFoodSources, initMaxDepth, maxNumberOfCycles, maxDepth, limit, Pip, numberOfFitnessCases, lowerB, upperB);
            }
            double meanOfRawFitnesses = 0;            
            for (int r = 0; r < numberOfRuns; r++)
            {
                meanOfRawFitnesses += runBests[r].rawFitness;                   
            }
            meanOfRawFitnesses = meanOfRawFitnesses / numberOfRuns;
            string results = "Mean of the raw fitness values: " + meanOfRawFitnesses.ToString()+ "\r\n ********************";
            for (int r = 0; r < numberOfRuns; r++)
            {
                WriteTheFunction(runBests[r].solutionTree);                                
                results += "\r\n " +"Run "+  r.ToString() + "\r\n Raw fitness value:" + String.Format("{0:0.000000}", runBests[r].rawFitness) + "\r\n" + functionString;               
                functionString = null;
            }
            richTextBox1.Text += results;          
        }

        private void button2_Click(object sender, EventArgs e)
        {
            richTextBox1.Clear();
        }

        private void ınfoContactToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Artificial Bee Colony Programming (ABCP) is an evolutionary computation based automatic programming and machine learning method and it was introduced to literature in 2012 by Karaboga et al.\r\n\r\nReferance Papers:\r\n-D.Karaboga, C.Ozturk, N.Karaboga, B.Gorkemli, Artificial bee colony programming for symbolic regression. Inf.Sci. 209, 1 - 15(2012)\r\n-B.Gorkemli, D.Karaboga, A quick semantic artificial bee colony programming(qsABCP) for symbolic regression, Inf. Sci. 502, 346 - 362(2019)\r\n\r\n\r\n\r\n\r\nCopyright © 2019 Erciyes University, Intelligent Systems Research Group, Department of Computer Engineering\r\n\r\n\r\nContact:\r\nDervis Karaboga (karaboga@erciyes.edu.tr)\r\nBeyza Gorkemli (bgorkemli@erciyes.edu.tr)", "Info/Contact");
        }
    }
}
