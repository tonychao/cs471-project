/* Artificial Bee Colony Programming (ABCP) implemantation in C# programming language

Artificial Bee Colony Programming (ABCP) is an evolutionary computation based automatic programming and machine learning method and introduced to literature in 2012 by Karaboga et al.

Referance Papers:

D. Karaboga, C. Ozturk, N. Karaboga, B. Gorkemli, Artificial bee colony programming for symbolic regression. Inf. Sci. 209, 1-15 (2012)
B. Gorkemli , D. Karaboga , A quick semantic artificial bee colony programming (qsABCP) for symbolic regression, Inf. Sci. 502, 346-362 (2019) 


Copyright © 2019 Erciyes University, Intelligent Systems Research Group, Department of Computer Engineering

Contact:
Dervis Karaboga (karaboga@erciyes.edu.tr )
Beyza Gorkemli (bgorkemli@erciyes.edu.tr)         */

using System;
using System.Collections;

namespace ABCP
{
    public class Methods
    {
        public static double RunTheModel(Node root, double input)
        {
            double result = 0;
            if (root.type == "function")
            {
                if (root.value == "+")
                {
                    result = RunTheModel(root.left, input) + RunTheModel(root.right, input);
                }
                else if (root.value == "-")
                {
                    result = RunTheModel(root.left, input) - RunTheModel(root.right, input);
                }
                else if (root.value == "*")
                {
                    result = RunTheModel(root.left, input) * RunTheModel(root.right, input);
                }
                else if (root.value == "/")
                {
                    double div = RunTheModel(root.right, input);
                    if (div == 0)
                    { result = 1; }
                    else
                    { result = RunTheModel(root.left, input) / div; }
                }
                
            }
            else if (root.type == "terminal")
            {
                double a;
                if (double.TryParse(root.value, out a) == true)
                {
                    result = a;
                }
                else if (root.value == "x")
                { result = input; }
            }
            if (Double.IsInfinity(result) || Double.IsNaN(result))// When such a case occurs, a suitable value can be assigned to the related subtree. For this test function 1000000 is assigned.
            { result = 1000000; }
            return result;
        }
        public static void ControlDifferenceOfTwoTrees(Node treeP, Node treeN, ref bool isDifferent)
        {
            if (treeP != null && treeN != null)
            {
                ControlDifferenceOfTwoTrees(treeP.left, treeN.left, ref isDifferent);
                if (treeN.value != treeP.value)
                { isDifferent = true; }
                ControlDifferenceOfTwoTrees(treeP.right, treeN.right, ref isDifferent);
            }
            else if ((treeP != null && treeN == null) || (treeP == null && treeN != null))
            { isDifferent = true; }
        }
      
        public static bool ControlDifference(FoodSource[] population, int j)
        {
            int counterD = 0;
            bool isDifferentFromWholePopulation = false;
            for (int h = 0; h < j; h++)
            {
                bool isDifferent = false;
                ControlDifferenceOfTwoTrees(population[h].solutionTree, population[j].solutionTree, ref isDifferent);
                if (isDifferent == true)
                { counterD++; }
            }
            if (counterD == j)
            { isDifferentFromWholePopulation = true; }
            return isDifferentFromWholePopulation;
        }
        public static bool ControlDifferenceForScout(FoodSource[] population, FoodSource sourceOfScout)
        {
            int counterD = 0;
            bool isDifferentFromWholePopulation = false;
            for (int h = 0; h < population.Length; h++)
            {
                bool isDifferent = false;
                ControlDifferenceOfTwoTrees(population[h].solutionTree, sourceOfScout.solutionTree, ref isDifferent);
                if (isDifferent == true)
                { counterD++; }
            }
            if (counterD == population.Length)
            { isDifferentFromWholePopulation = true; }
            return isDifferentFromWholePopulation;
        }

        public static void SeparateNodes(Node root, ref ArrayList functionNodes, ref ArrayList terminalNodes)
        {
            if (root != null)
            {
                SeparateNodes(root.left, ref functionNodes, ref terminalNodes);                
                if (root.type == "function")
                {
                    functionNodes.Add(root);
                }
                else if (root.type == "terminal")
                {
                    terminalNodes.Add(root);
                }                
                SeparateNodes(root.right, ref functionNodes, ref terminalNodes);
            }
        }
        public static void CountAndSeparateNodes(Node root, ref ArrayList functionNodes, ref ArrayList terminalNodes, ref int counter)
        {
            if (root != null)
            {
                CountAndSeparateNodes(root.left, ref functionNodes, ref terminalNodes, ref counter);              
                if (root.type == "function")
                { functionNodes.Add(counter); }
                else if (root.type == "terminal")
                { terminalNodes.Add(counter); }
                counter++;
                CountAndSeparateNodes(root.right, ref functionNodes, ref terminalNodes, ref counter);
            }
        }
        public static void PlaceNode(ref Node root, int i, Node nodefromK, ref int counter)
        {
            if (root != null)
            {
                PlaceNode(ref root.left, i, nodefromK, ref counter);
                if (counter == i)
                { root = (Node)nodefromK.Clone(); }
                counter++;
                PlaceNode(ref root.right, i, nodefromK, ref counter);
            }
        }
        public static void PerformNeighborhoodSearch(ref Node currentTree, Node kTree, Random rn, int maxDepth, double Pip, ref bool isSuccessful)
        {
            Node c = (Node)currentTree.Clone();
            ArrayList functionNodesK = new ArrayList();
            ArrayList terminalNodesK = new ArrayList();
            SeparateNodes(kTree, ref functionNodesK, ref terminalNodesK);
            Node nodeFromK;
            if ((rn.NextDouble() < Pip) && (functionNodesK.Count != 0))
            {
                nodeFromK = (Node)((Node)functionNodesK[rn.Next(0, functionNodesK.Count)]).Clone();
            }
            else
            {
                nodeFromK = (Node)((Node)terminalNodesK[rn.Next(0, terminalNodesK.Count)]).Clone();
            }
            ArrayList functionNodesC = new ArrayList();
            ArrayList terminalNodesC = new ArrayList();
            int counterC = 0;
            CountAndSeparateNodes(c, ref functionNodesC, ref terminalNodesC, ref counterC);
            int indexC = 0;
            if ((rn.NextDouble() < Pip) && (functionNodesC.Count != 0))
            {
                indexC = (int)(functionNodesC[rn.Next(0, functionNodesC.Count)]);
            }
            else
            {
                indexC = (int)(terminalNodesC[rn.Next(0, terminalNodesC.Count)]);
            }
            counterC = 0;
            PlaceNode(ref c, indexC, nodeFromK, ref counterC);            
            if (FindDepthOfTree(c) <= maxDepth + 1)
            {
                currentTree = (Node)c.Clone();
                isSuccessful = true;
            }
        }

        public static int FindDepthOfTree(Node n)
        {
            if (n == null)
            { return 0; }
            return Math.Max(FindDepthOfTree(n.left), FindDepthOfTree(n.right)) + 1;
        }
        public static FoodSource FindBestSource(FoodSource[] population)
        {
            FoodSource best = population[0];            
            for (int k = 1; k < population.Length; k++)
            {
                if (best.rawFitness > population[k].rawFitness)
                {
                    best = population[k];
                }
            }
            return best;
        }
        public static int FindMaxTrialCounterValuedSource(FoodSource[] population)
        {
            int indexOfTheMax = 0;
            double max = population[indexOfTheMax].trialCounter;            
            for (int k = 1; k < population.Length; k++)
            {
                if (max < population[k].trialCounter)
                {
                    max = population[k].trialCounter;
                    indexOfTheMax = k;
                }
            }
            return indexOfTheMax;
        }
    }
}
