// DlgSetMapProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NewDPR.h"
#include "DlgSetMapProperties.h"
#include "afxdialogex.h"

#include "commonDefs.h"

// CDlgSetMapProperties dialog

IMPLEMENT_DYNAMIC(CDlgSetMapProperties, CDialogEx)

CDlgSetMapProperties::CDlgSetMapProperties(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_AprX(0)
	, m_AprY(0)
	, m_Apr_Lon(0)
	, m_Apr_Lat(0)
	, m_mapRotateAngle(0)
{

}

CDlgSetMapProperties::~CDlgSetMapProperties()
{
}

void CDlgSetMapProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_edt_Map_APRX, m_AprX);
	DDX_Text(pDX, IDC_IDC_edt_Map_APRY, m_AprY);
	DDX_Text(pDX, IDC_IDC_edt_Map_APRX_Coor, m_Apr_Lon);
	DDV_MinMaxDouble(pDX, m_Apr_Lon, -180, 180);
	DDX_Text(pDX, IDC_IDC_edt_Map_APRY_Coor, m_Apr_Lat);
	DDV_MinMaxDouble(pDX, m_Apr_Lat, -90, 90);
	DDX_Text(pDX, IDC_IDC_edt_Map_Rotate, m_mapRotateAngle);
	DDV_MinMaxDouble(pDX, m_mapRotateAngle, 0, 360);
}


BEGIN_MESSAGE_MAP(CDlgSetMapProperties, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetMapProperties::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetMapProperties::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSetMapProperties message handlers


void CDlgSetMapProperties::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(true);

	gDataCenter.mapSettings.fAprX = m_AprX;
	gDataCenter.mapSettings.fAprY = m_AprY;
	gDataCenter.mapSettings.fAprLat = m_Apr_Lat;
	gDataCenter.mapSettings.fAprLon = m_Apr_Lon;
	gDataCenter.mapSettings.fRotation = m_mapRotateAngle;

	//PostMessage(WM_COMMAND, WM_PAINT, 0);
	SendMessageToViews(GetParent()->GetSafeHwnd(), WM_COMMAND, WM_PAINT, 0);

	CDialogEx::OnOK();
}


void CDlgSetMapProperties::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL CDlgSetMapProperties::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_AprX = gDataCenter.mapSettings.fAprX;
	m_AprY = gDataCenter.mapSettings.fAprX;
	m_Apr_Lat = gDataCenter.mapSettings.fAprLat;
	m_Apr_Lon = gDataCenter.mapSettings.fAprLon;
	m_mapRotateAngle = gDataCenter.mapSettings.fRotation;

	this->UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetMapProperties::SendMessageToViews(HWND hWndParent, UINT message, WPARAM wParam, LPARAM lParam) 
{
	BeginWaitCursor();
	for (HWND hWndChild = ::GetTopWindow(hWndParent); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		// send message
		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
			AfxCallWndProc(pWnd, pWnd->m_hWnd, message, wParam, lParam);

		// send to child windows after parent
		if (::GetTopWindow(hWndChild) != NULL)
			SendMessageToViews(hWndChild, message, wParam, lParam);
	}
	EndWaitCursor();
}
