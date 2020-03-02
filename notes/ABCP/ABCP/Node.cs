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
    public class Node:ICloneable
    {
        public string type;
        public string value;
        public Node left;
        public Node right;
       
        public object Clone()
        {
            Node n = new Node();
            n.type = this.type;
            n.value = this.value;
            if (this.left != null)
            {
                n.left = (Node)this.left.Clone();
            }
            if (this.right != null)
            {
                n.right = (Node)this.right.Clone();
            }
            return n;
        }
    }
}
