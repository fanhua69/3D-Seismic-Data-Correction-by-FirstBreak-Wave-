

#ifndef tagSvSys
#define tagSvSys
#include "struct.h"
class SvSys
{
public:   
	bool Set();
	void Reset();

	CString Area;
	CString Crew;
	long GroupInterval;
	long RecieveLineNumber;
	long ShotPointNumber;
	long ShotLineInterval;
	long GroupNumberOfSmallNumber;
	long GroupNumberOfBigNumber;
	long GapOfSmallNumber;
	long GapOfBigNumber;
	long FirstShotPointPosition;

	long WidthOfShotPoint;
	long WidthOfRecievePoint;

	int RLIBoxNumber;
	int SPIBoxNumber;
	long RecieveLinePosition[51];
	long ShotPointPosition[51];
	long RecieveLineInterval[50];
	long ShotPointInterval[50];

	long LeftPlusSquareLength;
	long RightPlusSquareLength;
	BOOL DataOK;

	// Functions:
	SvSys();
	BOOL  AskCommonParameter();
	BOOL  AskRLISPI();


private:


	BOOL ReadRLISPI();
	BOOL ReadCommonParameter();
	BOOL CalculateParameters();
};

#endif    

