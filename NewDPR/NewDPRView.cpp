
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
#include "MapLayerSetting.h"

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
	ON_COMMAND(ID_DEBUG_LAYERSWITCH, &CNewDPRView::OnDebugLayerswitch)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CNewDPRView 构造/析构

CNewDPRView::CNewDPRView()
	: CFormView(IDD_NEWDPR_FORM),
	m_pMapEngine(NULL),
	m_pLayerSettingDlg(NULL)
{
	// TODO: 在此处添加构造代码
	x_offset = y_offset = 0.0;
	m_bMouseDown = false;
	m_bIsDrawing = false;
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

	if (!m_pLayerSettingDlg)
	{
		m_pLayerSettingDlg = new CMapLayerSetting(this);

		m_pLayerSettingDlg->Create(IDD_DLG_MapLayer);

		m_pLayerSettingDlg->ShowWindow(SW_HIDE);
	}
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

	CWaitCursor wait;
	std::string temp = CT2A(strFile);
	m_pMapEngine->loadDwgMap(temp);
	//m_pLayerSettingDlg->SetDwgLayers(m_arrLayers);
	if (m_pLayerSettingDlg) 
	{
		m_pLayerSettingDlg->SetDwgLayers(m_pMapEngine->GetDwgLayers());
	}

	RedrawWindow();

	//Restore the Wait cursor.
	wait.Restore();
}


void CNewDPRView::OnDebugDebugsettings()
{
	// TODO: Add your command handler code here
}

void CNewDPRView::OnPaint()
{
	OutputDebugString(LOCATION);

	CPaintDC dc(this);
	// device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages

	if (!m_bIsDrawing)
	{
		m_bIsDrawing = true;
		if (m_pMapEngine)
		{
			m_pMapEngine->DoDraw();
		}
		m_bIsDrawing = false;
	}
	else 
	{
		OutputDebugString(_T("OnPaint Skip\r\n"));
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
	if (m_bMouseDown && (((point.x - m_ptDown.x) > 20) ||
		((point.x - m_ptDown.x) < -20) ||
		((point.y - m_ptDown.y) < -20) ||
		((point.y - m_ptDown.y) < -20)
		))
	{
		if (m_pMapEngine)
		{
			x_offset += (point.x - m_ptDown.x);
			y_offset -= (point.y - m_ptDown.y);
			m_pMapEngine->SetTransform(x_offset, y_offset);
		}

		m_ptDown = point;

		OutputDebugString(LOCATION);

		RedrawWindow();
	}
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

void CNewDPRView::OnDebugLayerswitch()
{
	// TODO: Add your command handler code here
	if (!m_pLayerSettingDlg)
	{
		m_pLayerSettingDlg = new CMapLayerSetting(this);

		m_pLayerSettingDlg->Create(IDD_DLG_MapLayer);

		m_pLayerSettingDlg->ShowWindow(SW_HIDE);
	}

	m_pLayerSettingDlg->ShowWindow(SW_NORMAL);
}

/*ref: http://pages.cs.wisc.edu/~tlabonne/d3dmfc.html
That's it. But there's one small problem left. If you run this now you'll see some wicked flickering. 
This is caused by a call to the CView OnEraseBkgnd messing things up. So let's override it and simply 
code it to return false. If you look in the Docs, OnEraseBkgnd returns nonzero if it erased the background.
Since we are erasing the background ourselves with a device->Clear call in Direct3D, we will return 0 to cause the 
window to remain unpainted and then we can sneak in and paint it ourselves with our Render method in OnDraw.*/
BOOL CNewDPRView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return 0;
}
