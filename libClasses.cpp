#include "libClasses.h"
#include "libExceptions.h"
using namespace BitMap;

BitMap::BitmapExt::BitmapExt(int len, int wid)
{
	l = len;
	w = wid;
	
	try
	{
		b = new bool* [l]; // --------------------------------------------------wyj�tek!!!!!!!!!!!!!!!!!!!!
		if (!b)
			throw MyRuntimeErrorExceptions::tableEx();
	}
	catch (const std::bad_alloc e)
	{
		cerr << e.what();
	}
	
	
	for (int i = 0; i < l; i++)
	{
		try {
			b[i] = new bool[w]; 
			if (!b[i])
				throw MyRuntimeErrorExceptions::tableEx();
		}
		catch (const std::bad_alloc e) {
			cerr << e.what();
		}
	}
	int i, j;
	for (i = 0; i < l; i++)
		for (j = 0; j < w; j++)
			b[i][j] = false; // przypisanie wszystkim pikselom bitmapy false
}

int BitMap::BitmapExt::length() const
{
	return l;
}

int BitMap::BitmapExt::width() const
{
	return w;
}

bool** BitMap::BitmapExt::bit()
{
	return b;
}

bool& BitMap::BitmapExt::operator()(unsigned x, unsigned y)
{
	return b[x][y]; // zwracana kom�rka tablicy
}

bool BitMap::BitmapExt::operator()(unsigned x, unsigned y) const
{
	return b;
}

ostream& BitMap::operator<<(ostream& str, BitmapExt const& bitmap)
{
	int i, j;
	
	for (i = 0; i < bitmap.l; i++)
	{
		for (j = 0; j < bitmap.w; j++)
			str << bitmap.b[i][j] << "  "; // wypisanie bitmapy do odpowiedniego strumienia wyj�cia
		str << endl << endl;
	}
	return str;
}

void BitMap::Inversion::transform(Bitmap& bitmap)
{
	bool** temp = bitmap.bit(); // pomocniczy wska�nik na bitmap�
	int i, j;

	for (i = 0; i < bitmap.length(); i++)
	{
		for (j = 0; j < bitmap.width(); j++)
		{
			// zamiana czarnych pikseli na bia�e, a czarnych pikseli na bia�e
			if (temp[i][j]) 
				temp[i][j] = false;
			else
				temp[i][j] = true;
		}
	}
}

void BitMap::Erosion::transform(Bitmap& bitmap)
{
	bool** change = new bool* [bitmap.length()]; // pomocnicza tablica skojarzona z bitmap�, zawiera informacj�, czy w kom�rce ma nast�pi� zmiana
	
	bool** temp = bitmap.bit(); // pomocniczy wska�nik na bitmap�
	int i, j;
	if (not bitmap.width() or not bitmap.length())
		return;
	for (i = 0; i < bitmap.length(); i++)
	{
		change[i] = new bool[bitmap.width()];
		for (j = 0; j < bitmap.width(); j++)
		{
			
			if (not i) // g�rne skrajne kom�rki
			{
				if (not j) // lewy g�rny wierzcho�ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else if (j == bitmap.width() - 1) // prawy g�rny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else // g�rne kom�rki
				{
					if (temp[i][j - 1] and temp[i][j + 1] and temp[i+1][j]) 
						change[i][j] = true;
					else
						change[i][j] = false;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne kom�rki
			{
				if (not j) // lewy dolny wierzcho�ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else  // dolne kom�rki
				{
					if (temp[i][j - 1] and temp[i][j + 1] and temp[i - 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
			}
			else if (not j) // lewe skrajne kom�rki
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j+1])
					change[i][j] = true;
				else 
					change[i][j] = false;
			}
			else if (j == bitmap.width()-1) // prawe skrajne kom�rki
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1])
					change[i][j] = true;
				else
					change[i][j] = false;
			}
			else // kom�rki wen�trzne z cech� czteros�siedztwa
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1] and temp[i][j + 1])
					change[i][j] = true;
				else
					change[i][j] = false;
			}
		}
	}

	for (i = 0; i < bitmap.length(); i++)
		for (j = 0; j < bitmap.width(); j++)
			if (not change[i][j]) // je�li kom�rka bitmapy ma przynajmniej jednego s�siada o bia�ym kolorze
				temp[i][j] = false; // zamie� na bia�y kolor
}

void BitMap::Reset::transform(Bitmap& bitmap)
{
	
	bool** temp = bitmap.bit(); // pomocniczy wska�nik na bitmap�
	int i, j;

	for (i = 0; i < bitmap.length(); i++)
		for (j = 0; j < bitmap.width(); j++)
			temp[i][j] = false; // zerowanie ca�ej bitmapy
}
void BitMap::Dilatation::transform(Bitmap& bitmap)
{
	bool** temp = bitmap.bit();
	int i, j;		
	bool** dilatationT = new bool* [bitmap.length()];		


	if (not bitmap.length() and bitmap.width())
		return;


	for (i = 0; i < bitmap.length(); i++)
	{
		dilatationT[i] = new bool[bitmap.width()];
		for (j = 0; j < bitmap.width(); j++)
		{
			if (not i) // g�rne skrajne kom�rki
			{
				if (not j) // lewy g�rny wierzcho�ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy g�rny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else // g�rne kom�rki
				{
					if (temp[i][j - 1] or temp[i][j + 1] or temp[i + 1][j])
						dilatationT[i][j] = false;				
					else
						dilatationT[i][j] = true;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne kom�rki
			{
				if (not j) // lewy dolny wierzcho�ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else  // dolne kom�rki
				{
					if (temp[i][j - 1] or temp[i][j + 1] or temp[i - 1][j])
						dilatationT[i][j] = false;					
					else
						dilatationT[i][j] = true;
				}
			}
			else if (not j) // lewe skrajne kom�rki
			{
				if (temp[i - 1][j] or temp[i + 1][j] or temp[i][j + 1])
					dilatationT[i][j] = false;
				else
					dilatationT[i][j] = true;
			}
			else if (j == bitmap.width() - 1) // prawe skrajne kom�rki
			{
				if (temp[i - 1][j] or temp[i + 1][j] or temp[i][j - 1])
					dilatationT[i][j] = false;
				else
					dilatationT[i][j] = true;
			}
			else // kom�rki wewn�trzne z cech� czteros�siedztwa
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1] and temp[i][j + 1])
					dilatationT[i][j] = true;
				else
					dilatationT[i][j] = false;
			}
		}
	}

	for (i = 0; i < bitmap.length(); i++)
		for (j = 0; j < bitmap.width(); j++) {
			if (not dilatationT[i][j]) // je�li chocia� jedna s�siaduj�ca kom�rka jest w kolorze bia�ym
				temp[i][j] = true; // to zamie� na czarny
			else
				temp[i][j] = false; // jesli nie to na bia�y
		}
}
		


void BitMap::Averaging::transform(Bitmap& bitmap)
{
	bool** temp = bitmap.bit();
	int i, j;
	bool** avg = new bool* [bitmap.length()];


	if (not bitmap.length() and bitmap.width())
		return;

	for (i = 0; i < bitmap.length(); i++)
	{
		avg[i] = new bool[bitmap.width()];
		for (j = 0; j < bitmap.width(); j++)
		{
			if (not i) // g�rne skrajne kom�rki
			{
				if (not j) // lewy g�rny wierzcho�ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy g�rny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else // g�rne kom�rki
				{
					if ((temp[i][j - 1] and temp[i][j + 1] and temp[i + 1][j]) or (temp[i][j - 1] and temp[i][j + 1]) or(temp[i][j + 1] and temp[i + 1][j])or (temp[i][j - 1] and temp[i+1][j]))
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne kom�rki
			{
				if (not j) // lewy dolny wierzcho�ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho�ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else  // dolne kom�rki
				{
					if ((temp[i][j - 1] and temp[i][j + 1] and temp[i - 1][j]) or (temp[i][j - 1] and temp[i][j + 1])or(temp[i][j + 1] and temp[i - 1][j])or(temp[i][j - 1] and temp[i - 1][j]))
						avg[i][j] = false;								
					else
						avg[i][j] = true;
				}
			}
			else if (not j) // lewe skrajne kom�rki
			{
				if ((temp[i - 1][j] and temp[i + 1][j] and temp[i][j+1]) or (temp[i - 1][j] and temp[i + 1][j])or(temp[i + 1][j] and temp[i][j + 1])or(temp[i - 1][j] and temp[i][j + 1]))
					avg[i][j] = false;
				else
					avg[i][j] = true;
			}
			else if (j == bitmap.width() - 1) // prawe skrajne kom�rki
			{
				if ((temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1]) or (temp[i - 1][j] and temp[i + 1][j]) or (temp[i + 1][j] and temp[i][j - 1]) or (temp[i - 1][j] and temp[i][j - 1]))
					avg[i][j] = false;
				else
					avg[i][j] = true;
			}
			else // kom�rki wewn�trzne z cech� czteros�siedztwa
			{
				if ((temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1] and temp[i][j + 1]) or (temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1])or(temp[i + 1][j] and temp[i][j - 1] and temp[i][j + 1])or(temp[i - 1][j] and temp[i + 1][j] and temp[i][j + 1])or(temp[i - 1][j]and temp[i][j - 1] and temp[i][j + 1])or(temp[i - 1][j]and temp[i + 1][j])or(temp[i - 1][j] and temp[i][j - 1])or(temp[i - 1][j] and temp[i][j + 1])or(temp[i + 1][j] and temp[i][j - 1])or(temp[i + 1][j] and temp[i][j + 1])or(temp[i][j - 1] and temp[i][j+1]))
					avg[i][j] = false;
				else
					avg[i][j] = true;
			}
		}
	}

	for (i = 0; i < bitmap.length(); i++)
		for (j = 0; j < bitmap.width(); j++) {
			if (not avg[i][j]) // je�li chocia� jedna s�siaduj�ca kom�rka jest w kolorze bia�ym
				temp[i][j] = true; // to zamie� na czarny
			else
				temp[i][j] = false; // jesli nie to zamie� na czarny
		}

}



