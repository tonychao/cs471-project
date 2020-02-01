#include "functions1.h"
#include <math.h>  
#include <cstdlib>


template <class Tinput, class Toutput>
typename Functions1<Tinput,Toutput>::function_pointer Functions1<Tinput,Toutput>::getFunctionById(int id)
{
	// return the address of a function
	function_pointer fp;

	switch(id)
	{
		case 1:
			// add Functions1<Tinput,Toutput>:: for g++ linux compilter
			fp = &Functions1<Tinput,Toutput>::f1Schwefels;
			break;
		case 2:
			fp = &Functions1<Tinput,Toutput>::f2DeJongs1;
			break;
		case 3:
			fp = &Functions1<Tinput,Toutput>::f3Rosenbrock;
			break;
		case 4:
			fp = &Functions1<Tinput,Toutput>::f4Rastrigin;
			break;
		case 5:
			fp = &Functions1<Tinput,Toutput>::f5Griewankgk;
			break;
		case 6:
			fp = &Functions1<Tinput,Toutput>::f6SineEnvelopeSineWave;
			break;
		case 7:
			fp = &Functions1<Tinput,Toutput>::f7StretchedVSineWave;
			break;
		case 8:
			fp = &Functions1<Tinput,Toutput>::f8AckleysOne;
			break;
		case 9:
			fp =  &Functions1<Tinput,Toutput>::f9AckleysTwo;
			break;
		case 10:
			fp =  &Functions1<Tinput,Toutput>::f10EggHolder;
			break;
		case 11:
			fp =  &Functions1<Tinput,Toutput>::f11Rana;
			break;
		case 12:
			fp =  &Functions1<Tinput,Toutput>::f12Pathological;
			break;
		case 13:
			fp =  &Functions1<Tinput,Toutput>::f13Michalewicz;
			break;
		case 14:
			fp =  &Functions1<Tinput,Toutput>::f14MasterCosineWave;
			break;
		case 15:
			fp =  &Functions1<Tinput,Toutput>::f15Quartic;
			break;
		case 16:
			fp =  &Functions1<Tinput,Toutput>::f16Levy;
			break;
		case 17:
			fp =  &Functions1<Tinput,Toutput>::f17Step;
			break;
		case 18:
			fp =  &Functions1<Tinput,Toutput>::f18Alphine;
			break;

	}
	return fp;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f1Schwefels(Tinput* data, int dimension)
{
	Toutput result;
	result = 418.9829 * dimension;

	for (int i = 0; i < dimension; i++)
	{
		result -= (-data[i] * sin(sqrt(abs(data[i]))));
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f2DeJongs1(Tinput* data, int dimension)
{
	Toutput result = 0.0;

	for (int i = 0; i < dimension; i++)
	{
		result += data[i]*data[i];
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f3Rosenbrock(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < (dimension - 1); i++)
	{
		//result += 100.0 * pow((data[i] * data[i] - data[i + 1]), 2) + (1.0 - data[i]) * (1.0 - data[i]);
		//the pow() function is slower

		Toutput a = data[i] * data[i] - data[i + 1];
		Toutput b = 1.0 - data[i];
		result += 100 * a * a + b * b;
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f4Rastrigin(Tinput* data, int dimension)
{
	Toutput result = 0.0;

	for (int i = 0; i < dimension; i++)
	{
		result += (data[i] * data[i] - 10.0 * cos(2.0 * M_PI * data[i]));
	}

	result = 10.0 * dimension * result;

	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f5Griewankgk(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	Toutput sum = 0.0;
	Toutput product = 1.0;

	for (int i = 0; i < dimension; i++)
	{
		sum += data[i]*data[i] / 4000.0;
	}

	for (int i = 0; i < dimension; i++)
	{
		product *= cos(data[i] / sqrt(i + 1.0));
	}

	result = 1.0 + sum - product;
	return result;

}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f6SineEnvelopeSineWave(Tinput* data, int dimension)
{
	Toutput sum = 0.0;
	Toutput result=0.0;

	for (int i=0; i<(dimension-1); i++)
	{
		double common_op = data[i] * data[i] + data[i + 1] * data[i + 1];
		double numerator = sin(common_op - 0.5);
		numerator *= numerator;

		double denominator = 1.0 + 0.001 * (common_op);
		denominator *= denominator;

		result += 0.5 + numerator / denominator;

	}

	return -result;

}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f7StretchedVSineWave(Tinput* data, int dimension)
{
	Toutput result = 0.0;

	for (int i = 0; i < (dimension - 1); i++)
	{
		double common_op = data[i] * data[i] + data[i + 1] * data[i + 1];
		double sine_sq = sin(50.0 * pow(common_op, 1.0 / 10));
		sine_sq *= sine_sq;
		result += pow(common_op, 1.0 / 4) * sine_sq + 1.0;
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f8AckleysOne(Tinput* data, int dimension)
{
	Toutput result = 0.0;

	for (int i = 0; i < (dimension - 1); i++)
	{
		double p1 = 1.0 / exp(0.2) * sqrt(data[i] * data[i] + data[i + 1] * data[i + 1]);
		double p2 = 3.0 * (cos(2.0 * data[i]) + sin(2.0 * data[i + 1]));
		result += p1 + p2;
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f9AckleysTwo(Tinput* data, int dimension)
{	
	Toutput result=0.0;
	for (int i = 0; i < (dimension-1); i++)
	{
		double p1 = 20.0 + exp(1);
		double p2 = -20.0 / exp(0.2 * sqrt((data[i] * data[i] + data[i + 1] * data[i + 1]) / 2));
		double p3 = -exp(0.5 * (cos(2.0 * M_PI * data[i]) + cos(2.0 * M_PI * data[i + 1])));
		result += p1 + p2 + p3;
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f10EggHolder(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < (dimension - 1); i++)
	{
		double p1 = -data[i] * sin(sqrt(fabs(data[i] - data[i + 1] - 47.0)));
		double p2 = -(data[i + 1] + 47.0) * sin(sqrt(fabs(data[i + 1] + 47.0 + data[i] / 2.0)));
		result += p1 + p2;
	}

	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f11Rana(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < (dimension - 1); i++)
	{
		double common_op_minus = sqrt(fabs(data[i + 1] - data[i] + 1.0));
		double common_op_plus = sqrt(fabs(data[i + 1] + data[i] + 1.0));
		result += data[i] * sin(common_op_minus) * cos(common_op_plus) + (data[i + 1] + 1.0) * cos(common_op_minus) * sin(common_op_plus);

	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f12Pathological(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < (dimension - 1); i++)
	{
		double sin_op = sin(sqrt(100.0 * data[i] * data[i] + data[i + 1] * data[i + 1]));
		double numerator = sin_op * sin_op - 0.5;
		double poly_op = data[i] * data[i] - 2.0 * data[i] * data[i + 1] + data[i + 1] * data[i + 1];
		double denominator = 1.0 + 0.001 * poly_op * poly_op;

		result += 0.5 + numerator / denominator;
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f13Michalewicz(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i =0; i<dimension; i++)
	{
		result += sin(data[i]) * pow(sin((i + 1) * data[i] * data[i] / M_PI), 20);
	}
	return -result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f14MasterCosineWave(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < (dimension-1); i++)
	{
		double common_op = data[i] * data[i] + data[i + 1] * data[i + 1] + 0.5 * data[i] * data[i + 1];
		result += exp(-1.0/8.0*common_op)*cos(4.0*sqrt(common_op));
	}
	return -result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f15Quartic(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < dimension; i++)
	{
		result += (i + 1.0) * data[i] * data[i] * data[i] * data[i];
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f16Levy(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	Toutput wn = 1.0 + (data[dimension - 1] - 1.0) / 4.0;
	for (int i = 0; i < (dimension - 1); i++)
	{
		double wi = 1.0 + (data[i] - 1.0) / 4.0;
		double p1 = (wi - 1) * (wi - 1) * (1.0 + 10.0 * sin(M_PI * wi + 1.0) * sin(M_PI * wi + 1.0));
		double p2 = (wn - 1) * (wn - 1) * (1.0 + sin(2.0*M_PI*wn) * sin(2.0 * M_PI * wn));
		result += p1 + p2;
	}
	Toutput w1 = 1.0 + (data[0] - 1.0) / 4.0;
	result += sin(M_PI * w1) * sin(M_PI * w1);
	return result;
}


template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f17Step(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < dimension; i++)
	{
		result += (fabs(data[i]) + 0.5) * (fabs(data[i]) + 0.5);
	}
	return result;
}

template <class Tinput, class Toutput>
Toutput Functions1<Tinput,Toutput>::f18Alphine(Tinput* data, int dimension)
{
	Toutput result = 0.0;
	for (int i = 0; i < dimension; i++)
	{
		result += fabs(data[i] * sin(data[i]) + 0.1 * data[i]);
	}
	return result;
}


