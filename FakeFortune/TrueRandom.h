// CTrueRandom
// Copyright (c) 2003 Dominik Reichl
// Totally free C++ class for getting true random numbers

// Thanks to random.org for providing the random numbers

#ifndef ___TRUE_RANDOM_H___
#define ___TRUE_RANDOM_H___

#include <windows.h>

// You can change the amount of read bytes per internet download here
#define TR_BUFFER_SIZE 8192
#define TR_URL "http://random.org/cgi-bin/randbyte?nbytes=8192&format=file"

// URLDownloadToCacheFile API function in UrlMon.dll
typedef HRESULT (WINAPI *LPURLDOWNLOAD)(LPVOID pCaller, LPCSTR szUrl,
	LPSTR szFileName, DWORD dwBufLength, DWORD dwReserved, LPVOID lpfnCB);

// TR_FLAG_GETLATEST == URLOSTRM_GETNEWESTVERSION
#define TR_FLAG_GETLATEST 0x3

class CTrueRandom
{
public:
	CTrueRandom();
	virtual ~CTrueRandom();

	bool Initialize(); // Call this function to initialize CTrueRandom
	void Close(); // Call this function when you don't need CTrueRandom any more

	// This function writes dwBufferSize random bytes to pBuffer
	void GetRandomBuffer(BYTE *pBuffer, DWORD dwBufferSize);

	// Some wrapper functions for GetRandomBuffer
	BYTE GetRandomByte();
	WORD GetRandomWord();
	DWORD GetRandomDWord();

private:
	// This function will connect to the internet and download the random
	// file from the server
	bool _FillRandomBuffer();

	HINSTANCE m_hUrlMon; // An instance handle to the UrlMon library
	LPURLDOWNLOAD m_lpUrlDownload; // This is the download function

	BYTE m_pBuf[TR_BUFFER_SIZE]; // Internal buffer containing random bytes
	DWORD m_dwBufPos; // The current position in m_pBuf
};

#endif // ___TRUE_RANDOM_H___
