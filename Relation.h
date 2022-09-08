// Relation.h: interface for the CRelation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELATION_H__4C73D3E1_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_RELATION_H__4C73D3E1_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRelation  
{
public:
	long Calculate();
	void SetModelGroup(float *pModalGroup,long nPoint);
	void SetGroup(float *pGroup);
	void SetGroupPointNumber(long n);
	CRelation();
	virtual ~CRelation();

private:
	// The group to be calcualted:
	float *m_pGroup;
	long m_nGroupPointNumber;

	// The modal grooup:
	float *m_pGroupModal;
	long m_nGroupModalPointNumber;
};

#endif // !defined(AFX_RELATION_H__4C73D3E1_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_)
