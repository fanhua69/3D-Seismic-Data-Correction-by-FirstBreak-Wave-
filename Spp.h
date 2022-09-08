#ifndef tagShotPointPar
#define tagShotPointPar

class ShotPointPar
{
public:
	long m_Ph;
	long m_FileNumber;	 
	long m_ZP;
	long m_HP;
	long m_BeginTrace;
	long m_EndTrace;
	long m_BeginGapTrace;
	long m_EndGapTrace;
	double m_XZB;
	double m_YZB;	    
	
	ShotPointPar(){
		Reset();		
	};
	
	ShotPointPar operator=(ShotPointPar &Other){
		m_Ph=Other.m_Ph;
		m_FileNumber=Other.m_FileNumber;
		m_ZP=Other.m_ZP;
		m_HP=Other.m_HP;
		m_BeginTrace=Other.m_BeginTrace;
		m_EndTrace=Other.m_EndTrace;
		m_BeginGapTrace=Other.m_BeginGapTrace;
		m_EndGapTrace=Other.m_EndGapTrace;
		m_XZB=Other.m_XZB;
		m_YZB=Other.m_YZB;
		return *this;
	};
	void Reset(){
		m_Ph=0;
		m_FileNumber=0;
		m_ZP=0;
		m_HP=0;
		m_BeginTrace=0;
		m_EndTrace=0;
		m_BeginGapTrace=0;
		m_EndGapTrace=0;
		m_XZB=0;
		m_YZB=0;
	};
};

#endif