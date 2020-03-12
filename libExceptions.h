#ifndef LIBEXCEPTIONS_H
#define LIBEXCEPTIONS_H
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

namespace MyRuntimeErrorExceptions
{
	class valException : public runtime_error
	{
	public:
		valException() :
			runtime_error("\nNiepoprawna wartosc") {};

		//valException(std::string msgerr) :
		//	runtime_error(msgerr.c_str()) {};
	};

	class fileOpenError : public runtime_error
	{
	public:
		fileOpenError() :
			runtime_error("\nNie mozna odnalezc pliku!") {};
	};

	class fileCreateError : public runtime_error
	{
	public:
		fileCreateError() :
			runtime_error("\nNie mozna utworzyc pliku!") {};
	};

	class valCreateError : public runtime_error
	{
	public:
		valCreateError() :
			runtime_error("\nBlad alokacji zmiennej w pamieci!") {};
	};

	class valGreaterThanZero : public runtime_error
	{
	public:
		valGreaterThanZero() :
			runtime_error("\nIlosc danych nie moze byc rowna 0!") {};
	};
	
	class tableEx :public bad_alloc
	{
	public:
		tableEx() :
			bad_alloc() {};
	};
}

#endif 