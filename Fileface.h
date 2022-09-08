#ifndef tagFileInterface
#define tagFileInterface

class FileInterface:public Equation
{
public:
	FileInterface();
	bool Set(int Swath);
	
	BOOL TurnInFromT(CDC *pDC);
	BOOL TurnInFromDBF(CDC *pDC);
	BOOL TurnOutToT(CDC *pDC);
	BOOL TurnOutToDBF(CDC *pDC);
};

#endif

