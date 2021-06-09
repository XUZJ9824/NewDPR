// MapLayerSetting.cpp : implementation file
//

#include "stdafx.h"
#include "NewDPR.h"
#include "MapLayerSetting.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CMapLayerSetting message handlers
void CMapLayerSetting::OnRangeButtonClick(UINT nID)
{
	int iChkStatus = ((CButton*)GetDlgItem(nID))->GetCheck();
	//SendMessageToViews(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(nID, iChkStatus), cmdLayers);
}

void CMapLayerSetting::SetDwgLayers(std::list<DRW_Layer> * pLayers)
{
	this->m_arrLayers.clear();
	for (std::list<DRW_Layer>::const_iterator it = pLayers->begin(); it != pLayers->end(); it++) {
		std::wstring stemp((*it).name.begin(), (*it).name.end());
		m_arrLayers.push_back(stemp);
	}
	UpdateLayerOptions();
}

void CMapLayerSetting::UpdateLayerOptions() {

	if (m_arrLayers.size() > 0)
	{
		//clear existed checkbox if already have
		DestroyLayerChecks();

		std::wstring strLayer;
		int cx_start = 20;
		int cy_start = 30;
		int height = 20;
		int width = 250;
		CRect recCurr(cx_start, cy_start, cx_start + width, cy_start + height);
		for (int i = 0; i < m_arrLayers.size() && (i < MAX_LAYERS); i++)
		{
			strLayer = m_arrLayers[i];

			CButton *pNewButton = new CButton();
			CString tmp(strLayer.c_str());
			pNewButton->Create((LPCTSTR)(tmp),
				WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
				recCurr,
				this,
				IDC_chkLayerStart + i);
			pNewButton->SetCheck(1);
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

int CMapLayerSetting::IsLayerVisible(std::wstring sLayer)
{
	int i = -1;
	int rt = 0;
	for (std::vector<std::wstring>::const_iterator it = m_arrLayers.begin(); it != m_arrLayers.end(); it++) 
	{
		i++;
		if (*it == sLayer) break;
	}

	if (i >= 0) 
	{
		rt = m_arrChkBoxes[i]->GetCheck();
	}

	return rt;
}
