
// NewDPRView.cpp : CNewDPRView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NewDPR.h"
#endif

#include "NewDPRDoc.h"
#include "NewDPRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewDPRView

IMPLEMENT_DYNCREATE(CNewDPRView, CFormView)

BEGIN_MESSAGE_MAP(CNewDPRView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNewDPRView 构造/析构

CNewDPRView::CNewDPRView()
	: CFormView(IDD_NEWDPR_FORM)
{
	// TODO: 在此处添加构造代码

}

CNewDPRView::~CNewDPRView()
{
}

void CNewDPRView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CNewDPRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CNewDPRView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CNewDPRView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNewDPRView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNewDPRView 诊断

#ifdef _DEBUG
void CNewDPRView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNewDPRView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNewDPRDoc* CNewDPRView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewDPRDoc)));
	return (CNewDPRDoc*)m_pDocument;
}
#endif //_DEBUG


// CNewDPRView 消息处理程序
