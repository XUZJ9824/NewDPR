#pragma once
#include <iostream>

#import  "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" named_guids raw_interfaces_only auto_rename
#import  "libid:1A31287A-4D7D-413E-8E32-3B374931BD89" named_guids raw_interfaces_only auto_rename

typedef std::basic_streambuf<char, std::char_traits<char>> sbuf_char;

typedef std::basic_streambuf<wchar_t, std::char_traits<wchar_t>> sbuf_wchar;

class vs_stream : std::basic_streambuf<char, std::char_traits<char>>
{
private:
	sbuf_char *oldBuf;
	sbuf_char *oldErrbuf;
	EnvDTE::OutputWindowPane* pPane;
protected:
	std::streamsize xsputn(const std::char_traits<char>::char_type* s, std::streamsize n);
	std::char_traits<char>::int_type overflow(std::char_traits<char>::int_type c);
public:
	vs_stream();
	virtual ~vs_stream();
};

class vs_wstream : std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>
{
private:
	sbuf_wchar *oldBuf;
	sbuf_wchar *oldErrbuf;
	EnvDTE::OutputWindowPane* pPane;
protected:
	std::streamsize xsputn(const std::char_traits<wchar_t>::char_type* s, std::streamsize n);
	std::char_traits<wchar_t>::int_type overflow(std::char_traits<wchar_t>::int_type c);
public:
	vs_wstream();
	virtual ~vs_wstream();
};

HRESULT CreateVSOutputPane(EnvDTE::OutputWindowPane **ppPane);