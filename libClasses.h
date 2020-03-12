#ifndef LIBCLASSES_H
#define LIBCLASSES_H
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include "libExceptions.h"

using namespace std;
using namespace MyRuntimeErrorExceptions;

/* Przestrze� nazw Bitmapy */
namespace BitMap
{
	/* Klasa polimorficzna, abstrakcyjna -> posiada funkcje wirtualne
		Funkcja wirtualna w klasie podstawowej nie jest przes�oni�ta przez funkcj� o tej samej nazwie w klasie pochodnej.
		W przypadku zwyk�ych funkcji, klasa dziedzicz�ca funkcj� po klasie podstawowej, przes�ania j� funkcj� z zakresu tej klasy pochodnej.
		Destruktory nie s� dziedziczone przez klasy pochodne.
	*/

	/*	Klasa podstawowa bitmapy */
	class Bitmap
	{
	public:
		
		// Funkcje czysto wirtualne
		virtual int length() const = 0; 
		virtual int width() const = 0; 
		virtual bool** bit() = 0;

		virtual bool& operator() (unsigned x, unsigned y) = 0;
		virtual bool operator() (unsigned x, unsigned y) const = 0;

		virtual ~Bitmap() {}
	};

	/* Klasa pochodna dziedzicz�ca po klasie abstrakcyjnej Bitmap
		Dane bitmapy s� przechowywane w tablicy dynamicznej dwuwymiarowej bool. 
		True (1) oznacza czarny piksel, False (0) bia�y piksel.
		Bitmapa zostaje wypisana w podany strumie� za pomoc� prze�adowanego operatora <<.
		Prze�adowany operator () zwraca adres do danej kom�rki bitmapy.
	*/
	class BitmapExt : public Bitmap
	{
		
		int l; // d�ugo�� bitmapy
		int w; // szeroko�� bitmapy
		bool** b; // bitmapa
	
	public:
		
		BitmapExt(int len, int wid); // konstruktor tworz�cy pust� bitmap� o podanej d�ugo�ci i szeroko�ci

		int length() const; // metoda zwracaj�ca d�ugo�� bitmapy
		int width() const; // metoda zwracaj�ca szeroko�� bitmapy
		bool** bit(); // metoda zwracaj�ca bitmap�

		/* Prze�adowany operator<< wypisuj�cy do odpowiedniego strumienia wyj�cia bitmap�
		@param str strumie� wyj�cia
		@param bitmap bitmapa
		*/
		friend ostream& operator<<(ostream& str, BitmapExt const& bitmap);

		/* Prze�adowany operator() zwracaj�cy adres kom�rki bitmapy
		@param x po�o�enie poziome danej kom�rki
		@param y po�o�enie pionowe danej kom�rki
		*/
		bool& operator() (unsigned x, unsigned y);

		bool operator() (unsigned x, unsigned y) const;
	};


	// ---------------------------------- KLASY FILTR�W MORFOLOGICZNYCH ------------------------------------------------

	class  Transformation
	{
	public:
		virtual void transform(Bitmap&) = 0;
	};

	template<typename Tclass>
	class SubmissionTransformations : public Transformation
	{
		queue<Transformation*> addedT; // kolejka dodanych transformacji
	public:
		
		/* Metoda dodaj�ca transformacj� bitmapy
		@param p wska�nik wskazuj�cy na dan� transformacj�
		*/
		void addTransformation(Transformation* p);

		/* Metoda przekszta�caj�ca bitmap� kolejnymi transformacjami, z uwzgl�dnieniem kolejno�ci ich dodania
		@param bitmap bitmapa
		*/
		void transform(Bitmap& bitmap);
	};

	class Inversion : public Transformation
	{
	public:
		void transform(Bitmap& bitmap);
	};

	class Erosion : public Transformation
	{
	public:
		void transform(Bitmap& bitmap);
	};

	class Dilatation : public Transformation
	{
	public:
		void transform(Bitmap& bitmap);
	};

	class Reset : public Transformation
	{
	public:
		void transform(Bitmap& bitmap);
	};

	class Averaging : public Transformation
	{
	public:
		void transform(Bitmap& bitmap);
	};

	// ---------------------------------- KLASY WYJATK�W ------------------------------------------------------

	class MyExceptions
	{
	public:
	};


	template<typename Tclass>
	inline void SubmissionTransformations<Tclass>::addTransformation(Transformation* p)
	{
		addedT.push(p); // dodanie do kolejki danej transformacji
	}

	template<typename Tclass>
	inline void SubmissionTransformations<Tclass>::transform(Bitmap& bitmap)
	{
		while (not addedT.empty())
		{
			addedT.front()->transform(bitmap); // przekszta�cenie bitmapy kolejnymi transformacjami
			addedT.pop();
		}
	}
}

#endif

