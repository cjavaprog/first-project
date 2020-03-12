#ifndef LIBCLASSES_H
#define LIBCLASSES_H
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include "libExceptions.h"

using namespace std;
using namespace MyRuntimeErrorExceptions;

/* Przestrzeñ nazw Bitmapy */
namespace BitMap
{
	/* Klasa polimorficzna, abstrakcyjna -> posiada funkcje wirtualne
		Funkcja wirtualna w klasie podstawowej nie jest przes³oniêta przez funkcjê o tej samej nazwie w klasie pochodnej.
		W przypadku zwyk³ych funkcji, klasa dziedzicz¹ca funkcjê po klasie podstawowej, przes³ania j¹ funkcj¹ z zakresu tej klasy pochodnej.
		Destruktory nie s¹ dziedziczone przez klasy pochodne.
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

	/* Klasa pochodna dziedzicz¹ca po klasie abstrakcyjnej Bitmap
		Dane bitmapy s¹ przechowywane w tablicy dynamicznej dwuwymiarowej bool. 
		True (1) oznacza czarny piksel, False (0) bia³y piksel.
		Bitmapa zostaje wypisana w podany strumieñ za pomoc¹ prze³adowanego operatora <<.
		Prze³adowany operator () zwraca adres do danej komórki bitmapy.
	*/
	class BitmapExt : public Bitmap
	{
		
		int l; // d³ugoœæ bitmapy
		int w; // szerokoœæ bitmapy
		bool** b; // bitmapa
	
	public:
		
		BitmapExt(int len, int wid); // konstruktor tworz¹cy pust¹ bitmapê o podanej d³ugoœci i szerokoœci

		int length() const; // metoda zwracaj¹ca d³ugoœæ bitmapy
		int width() const; // metoda zwracaj¹ca szerokoœæ bitmapy
		bool** bit(); // metoda zwracaj¹ca bitmapê

		/* Prze³adowany operator<< wypisuj¹cy do odpowiedniego strumienia wyjœcia bitmapê
		@param str strumieñ wyjœcia
		@param bitmap bitmapa
		*/
		friend ostream& operator<<(ostream& str, BitmapExt const& bitmap);

		/* Prze³adowany operator() zwracaj¹cy adres komórki bitmapy
		@param x po³o¿enie poziome danej komórki
		@param y po³o¿enie pionowe danej komórki
		*/
		bool& operator() (unsigned x, unsigned y);

		bool operator() (unsigned x, unsigned y) const;
	};


	// ---------------------------------- KLASY FILTRÓW MORFOLOGICZNYCH ------------------------------------------------

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
		
		/* Metoda dodaj¹ca transformacjê bitmapy
		@param p wskaŸnik wskazuj¹cy na dan¹ transformacjê
		*/
		void addTransformation(Transformation* p);

		/* Metoda przekszta³caj¹ca bitmapê kolejnymi transformacjami, z uwzglêdnieniem kolejnoœci ich dodania
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

	// ---------------------------------- KLASY WYJATKÓW ------------------------------------------------------

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
			addedT.front()->transform(bitmap); // przekszta³cenie bitmapy kolejnymi transformacjami
			addedT.pop();
		}
	}
}

#endif

