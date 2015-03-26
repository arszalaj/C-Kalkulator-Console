#pragma once
#include <string>
#include <ctype.h>
#include "Stack.h"
#include <cmath>
#include <iostream>
#include <sstream>

#define _USE_MATH_DEFINES

#define LEFT_ASSOC 0
#define RIGHT_ASSOC 1

#define ERROR -1
#define NUMBER 0
#define FUNCTION 1
#define OPERATOR 2
#define LB 3
#define RB 4
#define EOS 5

using namespace std;

static const string trigonometric[] = {"sin","cos","tan","ctg","NULL"};
static const double pi = 3.14159265;

/** 
* Zawiera pola i metody do przekszta³cania wyra¿enia do postaci ONP oraz do
* oszacowania wyniku wyra¿enia. Korzysta z klasy Stack.
*/
class CONP
{
public:
	CONP(void);
	~CONP(void);
	int infixToONP(std::string infix,std::string &onp);
	int evaluateONP(std::string onp,long double &result);


private:
	int getPrior(string op,int &assoc);
	int getNextElement(string exp,string &buffer, int &position,bool ONP);
	long long my_atoll(const char *instr) const;



};
