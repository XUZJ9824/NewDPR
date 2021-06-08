#include "stdafx.h"
#include "vsstream.h"

std::streamsize vs_stream::xsputn(const std::char_traits<char>::char_type* s, std::streamsize n)
{
	pPane->OutputString(CComBSTR(s));
	return n;
}

std::char_traits<char>::int_type vs_stream::overflow(std::char_traits<char>::int_type c)
{
	char str[2]; str[0] = c; str[1] = 0;
	pPane->OutputString(CComBSTR(str));
	return std::char_traits<char>::int_type(c);
}

vs_stream::vs_stream()
{
	HRESULT hr = CoInitialize(NULL);
	hr = CreateVSOutputPane(&pPane);
	hr = pPane->Activate();
	oldBuf = std::cout.rdbuf();
	oldErrbuf = std::cerr.rdbuf();
	std::cout.rdbuf(this);
	std::cerr.rdbuf(this);
}
vs_stream::~vs_stream()
{
	std::cout.rdbuf(oldBuf);
	std::cerr.rdbuf(oldErrbuf);
	pPane->Release();
	CoUninitialize();
}

std::streamsize vs_wstream::xsputn(const std::char_traits<wchar_t>::char_type* s, std::streamsize n)
{
	pPane->OutputString(CComBSTR(s));
	return n;
}

std::char_traits<wchar_t>::int_type vs_wstream::overflow(std::char_traits<wchar_t>::int_type c)
{
	wchar_t str[2]; str[0] = c; str[1] = 0;
	pPane->OutputString(CComBSTR(str));
	return std::char_traits<wchar_t>::int_type(c);
}

vs_wstream::vs_wstream()
{
	HRESULT hr = CoInitialize(NULL);
	hr = CreateVSOutputPane(&pPane);
	hr = pPane->Activate();
	oldBuf = std::wcout.rdbuf();
	oldErrbuf = std::wcerr.rdbuf();
	std::wcout.rdbuf(this);
	std::wcerr.rdbuf(this);
}
vs_wstream::~vs_wstream()
{
	std::wcout.rdbuf(oldBuf);
	std::wcerr.rdbuf(oldErrbuf);
	pPane->Release();
	CoUninitialize();
}

HRESULT CreateVSOutputPane(EnvDTE::OutputWindowPane **ppPane)
{
	HRESULT hr;
	CLSID clsid;
	CComQIPtr<EnvDTE80::DTE2, &EnvDTE80::IID_DTE2> pDTE2;
	CComPtr<EnvDTE::Windows> pWindows;
	CComPtr<IUnknown> pUnk;
	CComQIPtr<EnvDTE::OutputWindow> pOutputWin;
	CComPtr<EnvDTE::Window> pWin;
	CComPtr<IDispatch> pOutDisp;
	CComPtr<EnvDTE::OutputWindowPanes> pPanes;
	CComPtr<EnvDTE::OutputWindowPane> pPane;

	CLSIDFromProgID(L"VisualStudio.DTE", &clsid);

	hr = GetActiveObject(clsid, nullptr, &pUnk);
	pDTE2 = pUnk;

	hr = pDTE2->get_Windows(&pWindows);
	hr = pWindows->Item(CComVariant(EnvDTE::vsWindowKindOutput), &pWin);
	hr = pWin->get_Object(&pOutDisp);
	pOutputWin = pOutDisp;
	hr = pOutputWin->get_OutputWindowPanes(&pPanes);
	CComVariant vRedirected(L"Redirected cout");
	hr = pPanes->Item(vRedirected, &pPane);
	if (!pPane)
		hr = pPanes->Add(CComBSTR(L"Redirected cout"), &pPane);
	*ppPane = pPane.Detach();
	return hr;
}