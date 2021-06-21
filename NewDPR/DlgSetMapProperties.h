#pragma once


// CDlgSetMapProperties dialog

class CDlgSetMapProperties : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetMapProperties)

public:
	CDlgSetMapProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetMapProperties();

	void SendMessageToViews(HWND hWndParent, UINT message, WPARAM wParam, LPARAM lParam);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//地图中心点屏幕坐标
	double m_AprX;
	//地图中心点屏幕坐标
	double m_AprY;
	// 地图中心点经度坐标
	double m_Apr_Lon;
	// 地图中心点纬度坐标
	double m_Apr_Lat;
	// 地图旋转角度
	double m_mapRotateAngle;
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
