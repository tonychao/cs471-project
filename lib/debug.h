#ifndef _DEBUG_H_
#define _DEBUG_H_


//comment this quit debug mode
//#define DEBUGMODE 
    
    //std::cout << x
    #ifdef DEBUGMODE
        #define debug(x) x
    #else
        #define debug(x) 
    #endif 

#endif