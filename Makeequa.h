    
// Names of Primitive Files that are condtructed 
// by MakeEqua.cpp are included in "Equation.h".

#ifndef tagMakeEquation
    #define tagMakeEquation
	#include "equation.h"
#include "Mypoint.h"	// Added by ClassView
	
	class MakeEquation:public Equation   
	{
	public:
		MakeEquation();
        ~MakeEquation();
		BOOL Set(int Swath);
		BOOL MakeEquationFile();
		bool MakeEquationOnPhyPos();
		
	private:
	};
#endif


