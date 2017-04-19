/********************************************************************************************************
 * File:          proj2.c                                                                               *
 * Date:          2015/11/22                                                                            *
 * Author:        Bazik Martin, xbazik00@stud.fit.vutbr.cz                                              *
 * Project:       Calculation of natural logarithm using Taylor's and continued fraction algorithm;     *
 *                project no. 2 for subject IZP                                                         *
 * Description:   The program calculates natural logarithm using Taylor's and continued fraction        *
 *                algorithm. The program can also find sufficient number of iterations of the algorithms*
 *                for numbers from the given interval for the given deviation.                          *
 ********************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

double taylor_log (double x, unsigned int n);
double cfrac_log (double x,unsigned int n);
unsigned int number_iter (double number,double eps,int flag);
double ab (double number);
int check_log(char**str);
int check_iter(char**str,int x);
unsigned int show_iter (double min,double max,double eps,int flag);

int main(int argc,char *argv[])
{
	double number;
	unsigned int iter;
	double min,max,eps;
	char* str;
	if (argc!=1)
	{
		if (strcmp(argv[1],"--log")==0&&argc==4&&check_log(argv)==1)
		{
			number = strtod(argv[2],&str);
			iter = strtoul(argv[3],&str,10);
			printf("       log(%g) = %.12g\n",number,log(number));
			printf("    cf_log(%g) = %.12g\n",number,cfrac_log(number,iter));
			printf("taylor_log(%g) = %.12g\n",number,taylor_log(number,iter));
		}
		else if (strcmp(argv[1],"--iter")==0&&argc==5&&check_iter(argv,argc)==1)
		{
			min = strtod(argv[2],&str);
			max = strtod(argv[3],&str);
			eps = strtod(argv[4],&str);
			if (min>max)
                        {
                   	     int pom = min;
                	     min = max;
                             max = pom;
                             printf("Switching minimal and maximal value of interval.\n");
                        }
			printf("       log(%g) = %.12g\n",min,log(min));
			printf("       log(%g) = %.12g\n",max,log(max));
			printf("continued fraction iterations = %u\n",iter = show_iter(min,max,eps,0));
			printf("    cf_log(%g) = %.12g\n",min,cfrac_log(min,iter));
			printf("    cf_log(%g) = %.12g\n",max,cfrac_log(max,iter));
			printf("taylor polynomial iterations = %u\n",iter = show_iter(min,max,eps,1));
			printf("taylor_log(%g) = %.12g\n",min,taylor_log(min,iter));
			printf("taylor_log(%g) = %.12g\n",max,taylor_log(max,iter));
		}
		else
		{
			printf("    >Program calculates logarithm of the given number for the given number of iterations using Taylor's algorithm and Continued fraction.\n");
			printf("        >Format: --log number nuber_of_iterations\n");
			printf("    >It also calculates number of iterations needed to find lagorithm for the give accuracy.\n");
			printf("        >Format: --iter min_number max_value accuracy\n");
		}
	}
	else
	{
		printf("    >Name: Calculator of logarithm\n    >Author:Martin Bazik\n    >For more info run with arguments\n");
	}
	return 0;
}

/*
This function calculates value of natural logarithm using Taylor's algorithm.
It uses one algorithm for numbers from interval (0;1) and different version for
numbers greater then 0.
Values for number 0 and numbers smaller than 0 are set for -infinity and nan(Not a Number) respectively.
Returns value of natural logarithm for the given number.
*/
double taylor_log (double x, unsigned int n)
{
	double result=0;
	if (x>0.0&&x<1.0)
	{
		double sub=1-x;                //adjusting input of a function to the form given by the algorithm
		double origin=sub;             //storing original value of input
		double log=0;
		for(unsigned int i=1;i<=n;i++) //algorithm
		{
			log-=sub/i;
			sub=sub*origin;
		}
		result = log;
	}
	if (x>=1.0)
	{
		double sub=(x-1.0)/x;          //adjusting input of a function to the form given by the algorithm
		double origin=sub;             //storing original value of input
		double log=0;
		for(unsigned int i=1;i<=n;i++) //algorithm
		{
			log+=sub/i;
			sub=sub*origin;
		}
		result = log;
	}
	if (x==0.0)                        //special cases that algorithm cannot calculate
	{
		return -INFINITY;
	}
	else if (x<0.0)
	{
		result = NAN;
	}
	else if (isinf(x))
	{
		result = INFINITY;
	}
	else if (isnan(x))
	{
		result = NAN;
	}
	return result;
}

/*
This function calculates value of natural logarithm using Continued fraction algorithm.
Values for number 0 and numbers smaller than 0 are set for -infinity and nan(Not a Number) respectively.
Returns value of natural logarithm for the given number.
*/
double cfrac_log(double x,unsigned int n)
{
	double sub=(x-1)/(x+1);            //adjusting input of a function to the form given by the algorithm
	double log;
	double fc = 0.0,fa=1.0,fb;         //fc is value of a given fraction; fa is numerator of a function; fb is denominator
	if (n==0)
	{
		log = 0;
	}
	else
	{
		for(unsigned int i=n-1;i>=1;i--)   //algorithm
		{
			fa=sub*sub*i*i;
			fb=2*i+1;
			fc= fa/(fb-fc);
		}
		log = (2*sub)/(1-fc);              //final part of calculations
	}
	if (x==0.0)                        //special cases that algorithm cannot calculate
	{
		log = -INFINITY;
	}
	else if (x<0.0)
	{
		log = NAN;
	}
	else if (isinf(x))
	{
		log = INFINITY;
	}
    else if (isnan(x))
	{
		log = NAN;
	}
	return log;
}

/*
This function looks for sufficient number of iterations of the algorithms for numbers from the given interval.
flag = 0 is for continued fraction
flag = 1 is for Taylor's algorithm
The output is number of iterations for the given number and for the given algorithm for the given deviation (eps).
*/
unsigned int number_iter(double number, double eps, int flag)
{
    double diff=eps+1;                     //diff is greater than eps to run at least 1 iteration of a cycle
    unsigned int iterations=0;
    double lg=0.0;                         //value of logarithm
    for (unsigned int i = 1;diff>eps;i++)  //algorithm
    {
        if(flag==1)
        {
            lg = taylor_log(number,i);
        }
        if(flag==0)
        {
            lg = cfrac_log(number,i);
        }
        diff = lg - log(number);
        diff = ab(diff);
        iterations=i;
    }
    return iterations;
}
/*
This function calculates absolute value of the given number.
Output is absolute number.
*/
double ab (double number)
{
	if (number<0.0)
	{
		number = number *(-1.0);
	}
	return number;
}

/*
This function checks validity of input for calculation of logarithm.
Output 0 means invalid input.
Output 1 means valid input.
*/
int check_log(char**str)
{
	char *ptr="";
	double number=strtod(str[2],&ptr);
	(void) number;
	if (strcmp(ptr,"")!=0)
	{
		return 0;
	}
	double num = strtod(str[3],&ptr);
	if (strcmp(ptr,"")!=0)
	{
		return 0;
	}
	if (strcmp(ptr,"")==0&&(num<0||num>UINT_MAX))
	{
		return 0;
	}
	return 1;
}

/*
This function checks validity of input for calculation of number of iterations.
Output 0 means invalid input.
Output 1 means valid input.
*/
int check_iter(char**str,int x)
{
	char *ptr="";
	double number;
	for (int i=2;i<=x-1;i++)
	{
		number=strtod(str[i],&ptr);
	        if (i==x-1&&(number<=0.0||isinf(number)||isnan(number)||number<1e-12))
       		{
        		return 0;
        	}
		if (strcmp(ptr,"")!=0)
		{
			return 0;
		}
	}
	return 1;
}

/*
This function shows number of iterations.
It calls function for calculating number of iterations for minimal and maximal number of the given
interval and returns the greater.
*/
unsigned int show_iter(double min,double max,double eps,int flag)
{
	unsigned int minimal_number=number_iter(min,eps,flag);
	unsigned int maximal_number=number_iter(max,eps,flag);
	if (maximal_number>minimal_number)
	{
		return maximal_number;
	}
	else {return minimal_number;}
}
