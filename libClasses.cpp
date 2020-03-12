#include "libClasses.h"
#include "libExceptions.h"
using namespace BitMap;

BitMap::BitmapExt::BitmapExt(int len, int wid)
{
	l = len;
	w = wid;
	
	try
	{
		b = new bool* [l]; // --------------------------------------------------wyj¹tek!!!!!!!!!!!!!!!!!!!!
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
	return b[x][y]; // zwracana komórka tablicy
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
			str << bitmap.b[i][j] << "  "; // wypisanie bitmapy do odpowiedniego strumienia wyjœcia
		str << endl << endl;
	}
	return str;
}

void BitMap::Inversion::transform(Bitmap& bitmap)
{
	bool** temp = bitmap.bit(); // pomocniczy wskaŸnik na bitmapê
	int i, j;

	for (i = 0; i < bitmap.length(); i++)
	{
		for (j = 0; j < bitmap.width(); j++)
		{
			// zamiana czarnych pikseli na bia³e, a czarnych pikseli na bia³e
			if (temp[i][j]) 
				temp[i][j] = false;
			else
				temp[i][j] = true;
		}
	}
}

void BitMap::Erosion::transform(Bitmap& bitmap)
{
	bool** change = new bool* [bitmap.length()]; // pomocnicza tablica skojarzona z bitmap¹, zawiera informacjê, czy w komórce ma nast¹piæ zmiana
	
	bool** temp = bitmap.bit(); // pomocniczy wskaŸnik na bitmapê
	int i, j;
	if (not bitmap.width() or not bitmap.length())
		return;
	for (i = 0; i < bitmap.length(); i++)
	{
		change[i] = new bool[bitmap.width()];
		for (j = 0; j < bitmap.width(); j++)
		{
			
			if (not i) // górne skrajne komórki
			{
				if (not j) // lewy górny wierzcho³ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else if (j == bitmap.width() - 1) // prawy górny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else // górne komórki
				{
					if (temp[i][j - 1] and temp[i][j + 1] and temp[i+1][j]) 
						change[i][j] = true;
					else
						change[i][j] = false;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne komórki
			{
				if (not j) // lewy dolny wierzcho³ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
				else  // dolne komórki
				{
					if (temp[i][j - 1] and temp[i][j + 1] and temp[i - 1][j])
						change[i][j] = true;
					else
						change[i][j] = false;
				}
			}
			else if (not j) // lewe skrajne komórki
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j+1])
					change[i][j] = true;
				else 
					change[i][j] = false;
			}
			else if (j == bitmap.width()-1) // prawe skrajne komórki
			{
				if (temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1])
					change[i][j] = true;
				else
					change[i][j] = false;
			}
			else // komórki wenêtrzne z cech¹ czteros¹siedztwa
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
			if (not change[i][j]) // jeœli komórka bitmapy ma przynajmniej jednego s¹siada o bia³ym kolorze
				temp[i][j] = false; // zamieñ na bia³y kolor
}

void BitMap::Reset::transform(Bitmap& bitmap)
{
	
	bool** temp = bitmap.bit(); // pomocniczy wskaŸnik na bitmapê
	int i, j;

	for (i = 0; i < bitmap.length(); i++)
		for (j = 0; j < bitmap.width(); j++)
			temp[i][j] = false; // zerowanie ca³ej bitmapy
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
			if (not i) // górne skrajne komórki
			{
				if (not j) // lewy górny wierzcho³ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy górny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else // górne komórki
				{
					if (temp[i][j - 1] or temp[i][j + 1] or temp[i + 1][j])
						dilatationT[i][j] = false;				
					else
						dilatationT[i][j] = true;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne komórki
			{
				if (not j) // lewy dolny wierzcho³ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						dilatationT[i][j] = false;
					else
						dilatationT[i][j] = true;
				}
				else  // dolne komórki
				{
					if (temp[i][j - 1] or temp[i][j + 1] or temp[i - 1][j])
						dilatationT[i][j] = false;					
					else
						dilatationT[i][j] = true;
				}
			}
			else if (not j) // lewe skrajne komórki
			{
				if (temp[i - 1][j] or temp[i + 1][j] or temp[i][j + 1])
					dilatationT[i][j] = false;
				else
					dilatationT[i][j] = true;
			}
			else if (j == bitmap.width() - 1) // prawe skrajne komórki
			{
				if (temp[i - 1][j] or temp[i + 1][j] or temp[i][j - 1])
					dilatationT[i][j] = false;
				else
					dilatationT[i][j] = true;
			}
			else // komórki wewnêtrzne z cech¹ czteros¹siedztwa
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
			if (not dilatationT[i][j]) // jeœli chocia¿ jedna s¹siaduj¹ca komórka jest w kolorze bia³ym
				temp[i][j] = true; // to zamieñ na czarny
			else
				temp[i][j] = false; // jesli nie to na bia³y
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
			if (not i) // górne skrajne komórki
			{
				if (not j) // lewy górny wierzcho³ek
				{
					if (temp[i][j + 1] and temp[i + 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy górny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i + 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else // górne komórki
				{
					if ((temp[i][j - 1] and temp[i][j + 1] and temp[i + 1][j]) or (temp[i][j - 1] and temp[i][j + 1]) or(temp[i][j + 1] and temp[i + 1][j])or (temp[i][j - 1] and temp[i+1][j]))
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
			}
			else if (i == bitmap.length() - 1) // dolne skrajne komórki
			{
				if (not j) // lewy dolny wierzcho³ek
				{
					if (temp[i - 1][j] and temp[i][j + 1])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else if (j == bitmap.width() - 1) // prawy dolny wierzcho³ek
				{
					if (temp[i][j - 1] and temp[i - 1][j])
						avg[i][j] = false;
					else
						avg[i][j] = true;
				}
				else  // dolne komórki
				{
					if ((temp[i][j - 1] and temp[i][j + 1] and temp[i - 1][j]) or (temp[i][j - 1] and temp[i][j + 1])or(temp[i][j + 1] and temp[i - 1][j])or(temp[i][j - 1] and temp[i - 1][j]))
						avg[i][j] = false;								
					else
						avg[i][j] = true;
				}
			}
			else if (not j) // lewe skrajne komórki
			{
				if ((temp[i - 1][j] and temp[i + 1][j] and temp[i][j+1]) or (temp[i - 1][j] and temp[i + 1][j])or(temp[i + 1][j] and temp[i][j + 1])or(temp[i - 1][j] and temp[i][j + 1]))
					avg[i][j] = false;
				else
					avg[i][j] = true;
			}
			else if (j == bitmap.width() - 1) // prawe skrajne komórki
			{
				if ((temp[i - 1][j] and temp[i + 1][j] and temp[i][j - 1]) or (temp[i - 1][j] and temp[i + 1][j]) or (temp[i + 1][j] and temp[i][j - 1]) or (temp[i - 1][j] and temp[i][j - 1]))
					avg[i][j] = false;
				else
					avg[i][j] = true;
			}
			else // komórki wewnêtrzne z cech¹ czteros¹siedztwa
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
			if (not avg[i][j]) // jeœli chocia¿ jedna s¹siaduj¹ca komórka jest w kolorze bia³ym
				temp[i][j] = true; // to zamieñ na czarny
			else
				temp[i][j] = false; // jesli nie to zamieñ na czarny
		}

}



