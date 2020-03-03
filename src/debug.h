#ifndef _DEBUG_H_
#define _DEBUG_H_


/// @brief  using define to enable and disable debug print
///
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-03-11


//comment this quit debug mode
//#define DEBUGLEVEL0 
//#define DEBUGLEVEL1
//#define DEBUGVAR
//B#define DEBUGFILE    

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