#ifndef _DEBUG_H_
#define _DEBUG_H_


//comment this quit debug mode
//#define DEBUGMODE 
//#define DEBUGMODE1
//#define DEBUGVAR
    
    //std::cout << x
    #ifdef DEBUGMODE
        #define debug(x) x
    #else
        #define debug(x) 
    #endif 

    
    #ifdef DEBUGMODE1
        #define debug1(x) x
    #else
        #define debug1(x) 
    #endif 

    #ifdef DEBUGVAR
        #define debug_var(x) std::cout << #x << " = " << x << std::endl;
    #else
        #define debug_var(x)
    #endif 


      

#endif