
// NewDPRView.h : CNewDPRView 类的接口
//

#pragma once
#include "MapEngine.h"

class CMapLayerSetting;
class CNewDPRView : public CFormView
{
protected: // 仅从序列化创建
	CNewDPRView();
	DECLARE_DYNCREATE(CNewDPRView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_NEWDPR_FORM };
#endif

// 特性
public:
	CNewDPRDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CNewDPRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	bool m_bIsDrawing;
//Dwg add here
protected:
	//Dwg 
	CMapEngine	*m_pMapEngine;
	float		x_offset, y_offset;
	bool		m_bMouseDown;
	CPoint		m_ptDown;

//Setting
	CMapLayerSetting *m_pLayerSettingDlg;

public:
	afx_msg void OnDebugLoaddwg();
	afx_msg void OnDebugDebugsettings();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDebugLayerswitch();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // NewDPRView.cpp 中的调试版本
inline CNewDPRDoc* CNewDPRView::GetDocument() const
   { return reinterpret_cast<CNewDPRDoc*>(m_pDocument); }
#endif

