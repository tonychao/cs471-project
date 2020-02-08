#ifndef _FUNCTIONS1_H_
#define _FUNCTIONS1_H_

#include <math.h>  
#include <cstdlib>

/// @brief  Template class with 18 standard benchmark functions
///
/// The main function of the class is getFunctionById(int id) which receive a function id,
/// then it returns the function pointer of the selected function.
/// @author Chao Huang Lin (chao.huanglin@cwu.edu)
/// @date 2020-01-31

template <class Tinput, class Toutput>
class Functions1
{ 
	private:
	/// @brief f1: Schwefel's function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f1Schwefels(Tinput* data, int dimension);

	/// @brief f2: 1st De Jong's function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f2DeJongs1(Tinput* data, int dimension);

	/// @brief f3: Rosenbrock function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f3Rosenbrock(Tinput* data, int dimension);

	/// @brief f4: Rastrigin function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f4Rastrigin(Tinput* data, int dimension);

	/// @brief f5: Griewangk function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f5Griewankgk(Tinput* data, int dimension);

	/// @brief f6: Sine Envelope Sine Wave function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f6SineEnvelopeSineWave(Tinput* data, int dimension);

	/// @brief f7: Stretched V Sine Wave function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f7StretchedVSineWave(Tinput* data, int dimension);

	/// @brief f8: Ackley's One function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f8AckleysOne(Tinput* data, int dimension);

	/// @brief f9: Ackley's Two function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f9AckleysTwo(Tinput* data, int dimension);

	/// @brief f10: Egg Holder function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f10EggHolder(Tinput* data, int dimension);

	/// @brief f11: Rana function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function	
	Toutput f11Rana(Tinput* data, int dimension);

	/// @brief f12: Pathological function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f12Pathological(Tinput* data, int dimension);

	/// @brief f13: Michalewicz function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function	
	Toutput f13Michalewicz(Tinput* data, int dimension);

	/// @brief f14: Masters Cosine Wave function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f14MasterCosineWave(Tinput* data, int dimension);

	/// @brief f15: Quartic function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f15Quartic(Tinput* data, int dimension);

	/// @brief f16: Levy function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f16Levy(Tinput* data, int dimension);

	/// @brief f17: Step function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f17Step(Tinput* data, int dimension);

	/// @brief f18: Alpine function
	/// @param data Pointer of a data array (datatype defined by the user)
	/// @param dimension Size of the data array, the size of the array also represents the number of dimensions
	/// @return The result of the mathematical function
	Toutput f18Alphine(Tinput* data, int dimension);
	
	public:
	/// @brief Type defenition of a function pointer
	typedef Toutput (Functions1::*function_pointer) (Tinput*, int); // function pointer
	
	/// @brief Get the function pointer by id of the benchmark function
	/// @param id identification code of the benchmark function
	/// @return Function pointer of the selected benchmark function 
	function_pointer getFunctionById(int id);

};

#endif