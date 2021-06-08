#include "stdafx.h"
#include "SrollDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawView, CFormView)

BEGIN_MESSAGE_MAP(CDrawView, CFormView)

END_MESSAGE_MAP()

CDrawView::CDrawView():
	CFormView(0)
{
}


CDrawView::~CDrawView()
{
}

void CDrawView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CDrawView::OnDraw(CDC* pDC)
{

}

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
