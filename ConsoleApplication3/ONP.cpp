#include "StdAfx.h"
#include "ONP.h"

#define _CRT_SECURE_NO_WARNINGS 1

CONP::CONP(void)
{
}

CONP::~CONP(void)
{
}

/**
* Zwraca priotytet operatora oraz jego ³acznoœæ.
* @param[in] op operator
* @param[out] assoc ³¹cznoœæ
* @return priorytet operatora (im wy¿szy, tym wy¿sza kolejnoœæ wykonywania)
*/
int CONP::getPrior(std::string op, int &assoc)
{
	assoc = LEFT_ASSOC;
	if(!op.compare("*") || !op.compare("/"))
		return 3;
	if(!op.compare("+") || !op.compare("-"))
		return 2;
	if(!op.compare("^"))
		return 4;

	return 0;
}
/**
* Pobiera kolejny element wyra¿enia.
* Elementem mo¿e byæ zarówno liczba, funkcja czy operator.
* @param[in] exp wyra¿enie
* @param[out] buffer bufor
* @param position Pozycja aktualnie przetwarzanego znaku w wyra¿eniu.
* @param[in] ONP 0 - jeœli wyra¿enie w postaci infiksowej, 1 - jeœli wyra¿enie w ONP
* @return typ rozpoznanego elementu (FUNCTION,OPERATOR,NUMBER)
*/
int CONP::getNextElement(string exp,string &buffer, int &position,bool ONP)
{
	buffer.clear();

	unsigned int i=position,j=0; // zmienne pomocnicze

	// pomijanie bia³ych znaków
	while(i < exp.length() && (exp[i] == ' ' || exp[i] == '\t'))
		i++;

	// sprawdzamy czy znak jest operatorem
	if(!isdigit(exp[i]) && exp[i] != '.') // jezeli jest sprawdzamy czy nie jest to operator jednoargumentowy + -
		// nie jest to pierwszy znak wyrazenia oraz czy nie jest to operator jednoargumentowy np -6
		if((i == 0 && (exp[i] == '-' || exp[i] == '+')) || (i > 0 && (exp[i] == '-' || exp[i] == '+') && ((exp[i-1] == ' ' || exp[i-1] == '(') && (i+1) < exp.length() && isdigit(exp[i+1]) )  ))
			i++; // jeœl ipowyzszy warunek jest spelniony to musimy potraktowaæ ci¹g jako liczbê z operatorem jednoarguemtnowym np -11 , wiêc i++
		else // a jezeli nie jest jednoargumentowy to jest dwuargumentowy
	{ 
		position = i+1; // zwieksz pozycje 
		buffer = exp[i]; // zapisz operator do bufora
		buffer+=' '; // dodaj odstep
		if(exp[i] == '(') // jezeli operator jest lewym nawiasem zwroc
			return LB; // zwroc LN
		else if(exp[i] == ')') // a jezeli prawym to
			return RB; // zwroc LN
		else // a jezeli nie jest nawiasem
		return OPERATOR; // zwracamy sygnal ze pobralismy OPERATOR zwykly
	}

	// blok dla przypadku napotkania liczby
	if(i < exp.length() && isdigit(exp[i]))
	{
	{
		// sprawdzamy czy przed znakiem nie stoi operator jednoargumentowy 
		if((i == 1 && (exp[0] == '-' || exp[0] == '+')) || ( i > 1 && (exp[i-1] == '-' || exp[i-1] == '+')  && (exp[i-2] == ' ' || exp[i-2] == '(')))
		{
			if((i == 1 && exp[0] == '-') || exp[i-1] == '-') // jezeli jesto to minus
				buffer += '-'; // to zapisujemy go w buforze bo mamy do czyneinia z ujemna liczba
			else ; // jezeli jest to plus to nie musimy nic robiæ
		}

		while(isdigit(exp[i])) // dopóki kolejne znaki s¹ cyframi
			buffer+= exp[i++]; // zapisujemy je do bufora
	}
	
	if(i < exp.length() && exp[i] == '.') // jezeli napotkamy kropke
		buffer+=exp[i++];// zapisujemy ja do bufora 
	while(i < exp.length() && isdigit(exp[i])) // jezeli sa cyfry po kropce to przepisujemy je
		buffer+=exp[i++]; // do bufora

	position = i; // uaktualniamy pozycje 
	buffer+=' '; // dodajemy odstep do bufora dla czytelnosci
	return NUMBER; // zwracamy ze pobralismy liczbe
	}
	position = i;
	return EOS; // jezeli zaden z powyzszych warunkow nie zosta³ spe³niony to odczytano
	           // ostatni znak, ktory jest bialym znakiem
}
/**
* Przekszta³ca wyra¿enie w notacji infiksowej do wyra¿enia w notacji ONP.
* @param[in] infix wyra¿enie w notacji infiksowej
* @param[out] onp obiekt klasy string , w którym ma byæ zapisane przekszta³cone do ONP wyra¿enie.
* @return zwraca 0, je¿eli przekszta³cenie sie powiedzie, -1 - jeœli siê nie powiedzie
*/
int CONP::infixToONP(string infix,string &onp)
{
	CStack stack;
	int result;
	int position = 0;
	string buffer,buffer2;
	int assoc,assoc2;

	// dopoki jestesmy w obrebie wyrazenia
	while(position < infix.length())
	{
		// w zaleznosci od tego co pobrala funkcja getNextElement
		result = getNextElement(infix,buffer,position,0);

		switch(result)
		{
			case NUMBER: // jezeli pobrala liczbe
				onp += buffer;//dajemy na wyjœcie
				break;
			
			case OPERATOR : // jezeli operator
			// dopóki na stackie znajduje sie operator ktorego kolejnosc(getPrior) wykonywania
			// jest wieksza niz aktualnei pobranego
				while(stack.look(buffer2) && buffer2.compare("( ") && buffer2.compare(") ") &&
					(getPrior(buffer.substr(0,buffer.length()-1),assoc) <= getPrior(buffer2.substr(0,buffer2.length()-1),assoc2) 
					&& assoc == LEFT_ASSOC )
					|| getPrior(buffer.substr(0,buffer.length()-1),assoc) < getPrior(buffer2.substr(0,buffer2.length()-1),assoc2) 
					&& assoc == RIGHT_ASSOC )
				{
					stack.pop(buffer2); // zdejmij operator ze stosu
					onp+= buffer2; // i dodaj na onpjscie
				}
				stack.push(buffer); // dodaj aktualny operator na stos
				break;
			
			case LB:
				stack.push(buffer); // wstaw go na stos
			break;
		
			case RB :// jezeli prawy nawias to
				buffer2[0] = 0;
				while(buffer2[0] != '(') //az do napotkania lewego nawiasu
				{
					if(!stack.pop(buffer2)) // zdejmuj operatory ze stosu
						return ERROR; // jezeli nie napotkano lewego nawiasu a stos pusty tzn ze wyrazenie zle skonstruowane zwroc BLAD
					if(buffer2[0] != '(')
						onp+= buffer2; // doloz pobrany ze stosu operator na wyjœcie
				}
				break;
			case ERROR: // jezeli funkcja getNextElement napotkala blad to 
				return ERROR;  // zwroc BLAD
		}
	}

	while(stack.pop(buffer2))
		onp += buffer2;

	return 0;
}

/**
* Oblicza wartoœæ wyra¿enia w ONP.
* @param[in] onp wyra¿enie w ONP
* @param[out] result wynik wyra¿enia
* @return zwraca -1, gdy obliczenie nie powiod³o siê, w przeciwnym razie 0.
*/
int CONP::evaluateONP(std::string onp,long double &result)
{

	if(onp.length() == 0) // jezeli nie ma nic w buforze wejœciowym
	{
		result = 0; // to wynik rowny jest zero
		return 0;
	}

	int position = 0;
	long double a,b,c;
	string buffer,buffer2;
	CStack stack;
	char buf[256];
	int res;
	// dopoki jestesmy w obrebie wyrazenia
	while(position < onp.length())
	{
		res = getNextElement(onp,buffer,position,1);

		switch(res)
		{
			case NUMBER: // jezeli napotkamy liczbe
				stack.push(buffer); // dajemy ja na stos
				break;
			case OPERATOR: // jezeli napotkamy operator
				if(buffer[0] == '+') // i jezeli jest to plus
			{

				stack.pop(buffer2); // sciagamy liczbe ze stosu
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stosu
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = a+b; // wykonujemy dodawanie
				sprintf_s(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stos
			}
			if(buffer[0] == '-') // analogiczie dla pozostalych operatorow ...
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = b-a; // wykonujemy dodawanie
				sprintf_s(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '*')
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = a*b; // wykonujemy dodawanie
				sprintf_s(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '/')
			{

				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				if(!a)
					return ERROR;
				c = b/a; // wykonujemy dodawanie
				sprintf_s(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			if(buffer[0] == '^')
			{
				stack.pop(buffer2); // sciagamy liczbe ze stacku
				a = atof(buffer2.c_str()); // zamieniamy na double
				memset(buf,0,256); // zerujemy buf
				stack.pop(buffer2); // sciagamy druga liczbe ze stacku
				b = atof(buffer2.c_str()); // zamieniamy na double
				c = pow(b,a); // wykonujemy dodawanie
				sprintf_s(buf,"%.15lf",c); // zamieniamy na ciag znakow
				buffer2 = buf;
				stack.push(buffer2);// i z powrotem wkladamy na stack
			}
			break;
		case ERROR: // jezeli funkcja PobierzOp napotkala ERROR 
			return ERROR; // zwracamy -1 ERROR
			break;
		}
	}
	
	stack.pop(buffer2); // sciagamy ostateczny wynik ze stosu
	result = atof(buffer2.c_str()); // i przypisujemy do zmiennej wynik
}


/**
* Zamienia ³añcuch zawieraj¹cy liczbê ca³kowit¹ na wartoœæ liczbow¹ typu long long 
* @param *instr wskaŸnik do tablicy z ³añcuchem liczbowym
* @return wartoœæ ca³kowita typu long long odpowiadaj¹ca ³añcuchowi wejœciowemu
*/
long long CONP::my_atoll(const char *instr) const
{
  long long retval;
  int i;

  retval = 0;
  for (; *instr; instr++) {
    retval = 10*retval + (*instr - '0');
  }
  return retval;
}





	












