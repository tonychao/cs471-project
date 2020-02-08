#ifndef _DEBUG_H_
#define _DEBUG_H_


//comment this quit debug mode
//#define DEBUGLEVEL0 
//#define DEBUGLEVEL1
//#define DEBUGVAR
//#define DEBUGFILE    
    //std::cout << x
    #ifdef DEBUGLEVEL0
        #define debug(x) x
    #else
        #define debug(x) 
    #endif 

    
    #ifdef DEBUGLEVEL1
        #define debug1(x) x
    #else
        #define debug1(x) 
    #endif 

    #ifdef DEBUGVAR
        #define debug_var(x) std::cout << #x << " = " << x << std::endl;
    #else
        #define debug_var(x)
    #endif 

    #ifdef DEBUGFILE
        #define debugfile(x) x
    #else
        #define debugfile(x) 
    #endif 






      

#endif