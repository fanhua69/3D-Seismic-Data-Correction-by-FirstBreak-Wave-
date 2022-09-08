
#include "stdafx.h"
#include "3dfstbrk.h"
#include "svsys.h"
#include "swathpar.h"
#include "equation.h"
#include "FileFace.h"

FileInterface::FileInterface()
{
}

bool FileInterface::Set(int nSwath)
{
	return Equation::Set (nSwath);
	
}

BOOL FileInterface::TurnInFromT(CDC *pDC)
{
	return TRUE;
}
BOOL FileInterface::TurnInFromDBF(CDC *pDC)
{
	return TRUE;
}
BOOL FileInterface::TurnOutToT(CDC *pDC)
{
	return TRUE;
}
BOOL FileInterface::TurnOutToDBF(CDC *pDC)
{
	return TRUE;
}
