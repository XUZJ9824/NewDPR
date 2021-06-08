
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
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_COMMAND(ID_DEBUG_LOADDWG, &CNewDPRView::OnDebugLoaddwg)
	ON_COMMAND(ID_DEBUG_DEBUGSETTINGS, &CNewDPRView::OnDebugDebugsettings)
	ON_WM_SIZE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CNewDPRView 构造/析构

CNewDPRView::CNewDPRView()
	: CFormView(IDD_NEWDPR_FORM),
	m_pMapEngine(NULL)
{
	// TODO: 在此处添加构造代码
	x_offset = y_offset = 0.0;
	m_bMouseDown = false;
}

CNewDPRView::~CNewDPRView()
{
	if (this->m_pMapEngine)
	{
		delete m_pMapEngine;
	}
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

	m_pMapEngine = new CMapEngine(this->GetSafeHwnd());
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


////Dwg///
void CNewDPRView::OnDebugLoaddwg()
{
	// TODO: Add your command handler code here
	CFileDialog dlgFind(TRUE, LPCTSTR("*.dwg"), NULL);

	int rt = dlgFind.DoModal();
	CString strFile = dlgFind.GetPathName();

	std::string temp = CT2A(strFile);
	m_pMapEngine->loadDwgMap(temp);

	RedrawWindow();
}


void CNewDPRView::OnDebugDebugsettings()
{
	// TODO: Add your command handler code here
}

void CNewDPRView::OnPaint() 
{
	OutputDebugString(LOCATION);
	
	//CFormView::OnPaint();
	if (m_pMapEngine)
	{
		m_pMapEngine->DoDraw();
	}
}

void CNewDPRView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (m_pMapEngine)
	{
		RECT rct;
		this->GetWindowRect(&rct);
		m_pMapEngine->SetViewSize(abs(rct.right - rct.left), abs(rct.bottom - rct.top));
	}
}


void CNewDPRView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	CFormView::OnMouseHWheel(nFlags, zDelta, pt);
}


void CNewDPRView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseDown && (((point.x - m_ptDown.x) > 10) ||
		((point.x - m_ptDown.x) < -10) ||
		((point.y - m_ptDown.y) < -10) ||
		((point.y - m_ptDown.y) < -10)
		))
	{
		if (m_pMapEngine)
		{
			x_offset += (point.x - m_ptDown.x);
			y_offset -= (point.y - m_ptDown.y);
			m_pMapEngine->SetTransform(x_offset, y_offset);
		}

		m_ptDown = point;

		RedrawWindow();
	}

	CFormView::OnMouseMove(nFlags, point);
}


BOOL CNewDPRView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	m_pMapEngine->SetZoomDelta(zDelta / 1000.0f);

	RedrawWindow();

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}


void CNewDPRView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = true;
	m_ptDown = point;

	CFormView::OnLButtonDown(nFlags, point);
}


void CNewDPRView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = false;
	CFormView::OnLButtonUp(nFlags, point);
}