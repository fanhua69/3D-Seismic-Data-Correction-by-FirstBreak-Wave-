
#ifndef fbk_h
#define fbk_h

class DataInFstBrkFile{
public:
	long FileNumber;		//4 bytes, from 0 to 65535. enough.			   
	double FirstBreak;      // for precision of calculation, use " double " type here.

	DataInFstBrkFile(){
		FileNumber=0;
		FirstBreak=0;
	};
};	


class Fbk{

private:
	FILE *m_fpFbk;
	BOOL m_DataOK;

public:
	static bool Save(CString sFile,DataInFstBrkFile *pRoom,long n);
	Fbk();
	Fbk(CString);
	~Fbk();
	BOOL Set(CString);
	void close();

	CString m_fFileName;
	long m_TraceNumber;

	long Get(DataInFstBrkFile *,long pos,long n=1);
	long Put(DataInFstBrkFile *,long pos,long n=1);
};

#endif