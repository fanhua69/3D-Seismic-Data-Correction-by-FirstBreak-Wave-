
#ifndef tagfh
#define tagfh

	#include "dswnum.h" 
	int AskSwathNumber();
	BOOL IntegrateFstBrk();
	CString IntToName(int Swath);

	CString STR(int Value);
	CString STR(long Value);
	CString STR(float Value);
	CString STR(double Value);
	BOOL CLS(CDC *pDC);
	long RevByte(long);

	template <class T>
	void Swap(T *a,T *b)
	{
		T x=*a;
		*a=*b;
		*b=x;
	}

#endif

/////////////////////////////////////////////////////////////////////////////
