#include "libExceptions.h"
#include "LibClasses.h"
using namespace std;
using namespace BitMap;

auto main() -> int
{

	BitmapExt aa(5, 5);
	aa(1, 1) = aa(2, 1) = aa(3, 1) = aa(2, 2) = aa(1, 3) = aa(2, 3) = aa(3, 3) = true;
	cout << aa << "\n";
	Inversion i; // inwersja
	Erosion e; // erozja
	Reset r; // zerowanie
	Averaging a; // uœrednianie
	Dilatation d; // dylatacja	

	SubmissionTransformations<Transformation> sub;
	//sub.addTransformation(&a);
	//sub.addTransformation(&d);
	//sub.addTransformation(&e);
	sub.addTransformation(&a); 
	sub.addTransformation(&d);
	sub.transform(aa);

	cout << aa << "\n";

	cin.ignore();
	return 0;
}