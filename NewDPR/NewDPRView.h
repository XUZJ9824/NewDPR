
// NewDPRView.h : CNewDPRView 类的接口
//

#pragma once


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
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NewDPRView.cpp 中的调试版本
inline CNewDPRDoc* CNewDPRView::GetDocument() const
   { return reinterpret_cast<CNewDPRDoc*>(m_pDocument); }
#endif

