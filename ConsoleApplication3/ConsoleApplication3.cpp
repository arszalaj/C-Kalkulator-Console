// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "ONP.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CONP onp;
	string infix, onp_ex, sresult;
	char buf[256];
	long double result;

	cout<<"Podaj wyrazenie"<<endl<<endl;
	cin>>infix;

	if(onp.infixToONP(infix,onp_ex) == -1)
	{
		cout<<"Nieprawidlowe lub nieobslugiwane wyrazenie"<<endl;
	}
	else
	{
		if(onp.evaluateONP(onp_ex,result) == -1)
		{
			cout<<"Nie mozna obliczyc wyrazenia"<<endl;
		}
		else
		{
			sprintf_s(buf,"%.15Lf",result);
			sresult= buf;
			cout<<endl<<"ONP : "<<onp_ex<<endl;
			cout<<"Wynik = "<<sresult<<endl<<endl;
		}
	}
	system("PAUSE");
	return 0;
}

