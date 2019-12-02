// CTrueRandom
// Copyright (c) 2003 Dominik Reichl
// Totally free C++ class for getting true random numbers

// Thanks to random.org for providing the random numbers

#include "StdAfx.h"
#include "TrueRandom.h"

// Constructor
CTrueRandom::CTrueRandom()
{
	m_hUrlMon = NULL;
	m_lpUrlDownload = NULL;
}

// Destructor
CTrueRandom::~CTrueRandom()
{
	if(m_hUrlMon != NULL) Close();
}

bool CTrueRandom::Initialize()
{
	// If a connection is open already, close it before continuing
	if(m_hUrlMon != NULL) Close();

	// Load the UrlMon library
	m_hUrlMon = LoadLibrary(_T("UrlMon"));
	if(m_hUrlMon == NULL) return false; // Library not available

	// Get the download-to-file-cache function
	m_lpUrlDownload = (LPURLDOWNLOAD)GetProcAddress(m_hUrlMon,
		"URLDownloadToCacheFileA");
	if(m_lpUrlDownload == NULL)
	{
		Close();
		return false;
	}

	// Try to get the first random numbers
	return _FillRandomBuffer();
}

void CTrueRandom::Close()
{
	// Free the UrlMon library
	if(m_hUrlMon != NULL)
	{
		FreeLibrary(m_hUrlMon);
		m_hUrlMon = NULL;
		m_lpUrlDownload = NULL; // Free the download function
	}
}

bool CTrueRandom::_FillRandomBuffer()
{
	char szTempFile[MAX_PATH]; // The file where the random numbers are stored
	HANDLE hFile; // Handle to the file above
	DWORD dwRead = TR_BUFFER_SIZE; // Number of bytes we were able to read

	m_dwBufPos = 0; // Reset the random buffer position

	szTempFile[0] = 0; szTempFile[1] = 0; // Reset filename

	// Download the random numbers to the Internet Explorer cache
	m_lpUrlDownload(NULL, TR_URL, szTempFile, TR_FLAG_GETLATEST, 0, NULL);

	// Open, read and close the cached file
	hFile = CreateFile(szTempFile, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) // We cannot open the file?
	{
		DeleteFile(szTempFile);
		return false;
	}
	ReadFile(hFile, m_pBuf, TR_BUFFER_SIZE, &dwRead, NULL);
	CloseHandle(hFile); // Close the file

	// Delete the cached file
	DeleteFile(szTempFile);

	// Check if we were able to read enough random bytes
	return (dwRead == TR_BUFFER_SIZE);
}

void CTrueRandom::GetRandomBuffer(BYTE *pBuffer, DWORD dwBufferSize)
{
	DWORD dwBytesAvail; // Bytes available in the buffer

	// Have we reached the end of the random buffer with the last call?
	if(m_dwBufPos == TR_BUFFER_SIZE) _FillRandomBuffer();

	while(dwBufferSize != 0)
	{
		dwBytesAvail = TR_BUFFER_SIZE - m_dwBufPos;

		// If zero bytes are available, get some new ones
		if(dwBytesAvail == 0) { _FillRandomBuffer(); continue; }

		// We have enough bytes to fill the buffer at once
		if(dwBytesAvail >= dwBufferSize)
		{
			// Fill the target buffer
			memcpy(pBuffer, &m_pBuf[m_dwBufPos], dwBufferSize);

			// Update number of remaining bytes and dwBufferSize
			m_dwBufPos += dwBufferSize;
			dwBufferSize = 0;
		}
		else // dwBytesAvail < dwBufferSize, we don't have enough random bytes
		{
			// Copy as many bytes as currently available
			memcpy(pBuffer, &m_pBuf[m_dwBufPos], dwBytesAvail);

			// Get new random numbers from the net
			_FillRandomBuffer();

			// Update buffer position and number of remaining bytes
			pBuffer += dwBytesAvail;
			dwBufferSize -= dwBytesAvail;
		}
	}
}

// Wraps the GetRandomBuffer function and returns a random byte
BYTE CTrueRandom::GetRandomByte()
{
	BYTE b;
	GetRandomBuffer(&b, 1);
	return b;
}

// Wraps the GetRandomBuffer function and returns a random word
WORD CTrueRandom::GetRandomWord()
{
	WORD w;
	GetRandomBuffer((BYTE *)&w, 2);
	return w;
}

// Wraps the GetRandomBuffer function and returns a random dword
DWORD CTrueRandom::GetRandomDWord()
{
	DWORD dw;
	GetRandomBuffer((BYTE *)&dw, 4);
	return dw;
}
