// MapLayerSetting.cpp : implementation file
//

#include "stdafx.h"
#include "NewDPR.h"
#include "MapLayerSetting.h"
#include "afxdialogex.h"
#include "commonDefs.h"

// CMapLayerSetting dialog

IMPLEMENT_DYNAMIC(CMapLayerSetting, CDialogEx)

CMapLayerSetting::CMapLayerSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MapLayer, pParent)
{

}

CMapLayerSetting::~CMapLayerSetting()
{
}

void CMapLayerSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapLayerSetting, CDialogEx)
	ON_COMMAND_RANGE(IDC_chkLayerStart, IDC_chkLayerStart + MAX_LAYERS, &CMapLayerSetting::OnRangeButtonClick)
	ON_BN_CLICKED(IDOK, &CMapLayerSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CMapLayerSetting message handlers
void CMapLayerSetting::OnRangeButtonClick(UINT nID)
{
	int iChkStatus = ((CButton*)GetDlgItem(nID))->GetCheck();
	unsigned int index = nID - IDC_chkLayerStart;

	if (index < gDataCenter.arrDwgLayers.size())
	{
		gDataCenter.arrDwgLayers[index].bVisible = iChkStatus;
	}
	else 
	{
		ASSERT(0);
	}

	//SendMessageToViews(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(nID, iChkStatus), cmdLayers);
}

void CMapLayerSetting::SetDwgLayers(std::list<DRW_Layer> * pLayers)
{
	gDataCenter.arrDwgLayers.clear();
	
	for (std::list<DRW_Layer>::const_iterator it = pLayers->begin(); it != pLayers->end(); it++) {
		tDwgLayer tempLayer;
		std::wstring stemp = Ansi2WChar((*it).name);
		
		tempLayer.sLayer = stemp;
		tempLayer.bVisible = 1;

		gDataCenter.arrDwgLayers.push_back(tempLayer);
	}

	UpdateLayerOptions();
}

void CMapLayerSetting::UpdateLayerOptions() {

	if (gDataCenter.arrDwgLayers.size() > 0)
	{
		//clear existed checkbox if already have
		DestroyLayerChecks();

		std::wstring strLayer;
		int cx_start = 20;
		int cy_start = 30;
		int height = 20;
		int width = 250;
		CRect recCurr(cx_start, cy_start, cx_start + width, cy_start + height);
		for (int i = 0; i < gDataCenter.arrDwgLayers.size() && (i < MAX_LAYERS); i++)
		{
			strLayer = gDataCenter.arrDwgLayers[i].sLayer;

			CButton *pNewButton = new CButton();
			CString tmp(strLayer.c_str());
			pNewButton->Create((LPCTSTR)(tmp),
				WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
				recCurr,
				this,
				IDC_chkLayerStart + i);
			pNewButton->SetCheck(gDataCenter.arrDwgLayers[i].bVisible);
			pNewButton->ShowWindow(SW_NORMAL);
			m_arrChkBoxes.push_back(pNewButton);

			if (i > (MAX_LAYERS / 2)) {
				//switch to another column
				recCurr.left = cx_start + width;
				recCurr.top = cy_start + (i - MAX_LAYERS / 2)*height;
				recCurr.bottom = recCurr.top + height;
				recCurr.right = recCurr.left + width;
			}
			else {
				recCurr.top += height;
				recCurr.bottom += height;
			}
		}
	}
}

BOOL CMapLayerSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateLayerOptions();
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapLayerSetting::DestroyLayerChecks() 
{
	if (m_arrChkBoxes.size()) 
	{
		for (std::vector<CButton*>::const_iterator it = m_arrChkBoxes.begin(); it != m_arrChkBoxes.end(); it++) 
		{
			CButton *pbt = *it;
			pbt->DestroyWindow();
			delete pbt;
		}

		m_arrChkBoxes.clear();
	}
}




void CMapLayerSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
