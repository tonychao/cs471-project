#ifndef _DEBUG_H_
#define _DEBUG_H_


//comment this quit debug mode
//#define DEBUGMODE 
#define DEBUGMODE1
    
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

#endif