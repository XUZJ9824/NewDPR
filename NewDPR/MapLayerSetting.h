#pragma once
#include "dx_data.h"
#include <vector>

#define MAX_LAYERS 50

typedef enum
{
	cmdLayers = 0,
	cmdGraphicalTypes,
	cmdGraphicalOptions
}settingCommands;

typedef enum
{
	optNumHatchArc = 0,
	optFillPolygen,
	optDrawHatchArcCounterClockWise,
	optDrawHatchArcClockWise,
	optNumText,
	optRotation
}commandOpts;

// CMapLayerSetting dialog

class CMapLayerSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CMapLayerSetting)

public:
	CMapLayerSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapLayerSetting();

	/*set Layer List information from DWG for display setting purpose*/
	void SetDwgLayers(std::list<DRW_Layer> * pLayers);
	void SendMessageToViews(HWND hWndParent, UINT message, WPARAM wParam, LPARAM lParam);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MapLayer };
#endif

	afx_msg void OnRangeButtonClick(UINT nID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	std::vector<CButton*>     m_arrChkBoxes;

	void UpdateLayerOptions();
	void DestroyLayerChecks();
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
