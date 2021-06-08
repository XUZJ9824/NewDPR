#pragma once
class CDrawView :	public CFormView
{
protected: // create from serialization only
	CDrawView();
	DECLARE_DYNCREATE(CDrawView)

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	~CDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
};

