#include <stdio.h>
#include "pcserial.h"

CPCSerial::~CPCSerial()
        {
        Close();
        }

CPCSerial::CPCSerial()
{
    m_hComm = 0;
    m_timeout = -1;
    m_baud = 0;
    m_port = 0;
}


int CPCSerial::GetPort()
    {
    HKEY hSERIALCOMM;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &hSERIALCOMM) == ERROR_SUCCESS)
        {
        DWORD dwMaxValueNameLen=0x200, dwMaxValueLen=0x200;
        DWORD dwType, dwIndex = 0;
        WCHAR szValueName[0x200];
        WCHAR value[0x200];
        while(1)
            {
            dwMaxValueNameLen=0x200, dwMaxValueLen=0x200;
            if (RegEnumValue(hSERIALCOMM, dwIndex, szValueName, &dwMaxValueNameLen, NULL, &dwType, (BYTE *)value, &dwMaxValueLen)!=ERROR_SUCCESS)
                break;
            if (wcsstr(szValueName, L"VCP") && wcsstr(value, L"COM"))
                return _wtoi(wcsstr(value, L"COM")+3);
             dwIndex++;
            }
        }
    return 0;
    }

int CPCSerial::Open(unsigned int comport, unsigned int baud)
        {
        DCB dcb;
        WCHAR wbuf[0x100];

        m_baud = baud;
        m_port = comport;

        swprintf(wbuf, L"COM%d", comport);
        m_hComm = CreateFileW(wbuf,
                GENERIC_READ | GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                0,
                0);
        if (m_hComm==INVALID_HANDLE_VALUE)
             return -1;

        GetCommState(m_hComm, &dcb);
        ZeroMemory(&dcb, sizeof(DCB));
        dcb.DCBlength = sizeof(DCB);
        dcb.BaudRate = baud;
        dcb.ByteSize = 8;
        dcb.fDtrControl = 1;
        dcb.fAbortOnError = 1;
        dcb.fBinary = 1;
        dcb.fRtsControl = 1;
        dcb.fTXContinueOnXoff = 1;
        dcb.XoffLim = 512;
        dcb.XonLim = 2048;
        dcb.XoffChar = 19;
        dcb.XonChar = 17;
        // Set new state.
        if (!SetCommState(m_hComm, &dcb))
                return -1;

        // success
        return 0;
        }

int CPCSerial::Close()
        {
        if (m_hComm)
        {
            if (CloseHandle(m_hComm)==0)
                return -1;
            m_hComm = 0;
        }
        else
            return -1;
        return 0;
        }

int CPCSerial::Send(const char *data, unsigned int len, unsigned short timeoutMs)
        {
        DWORD dwWrite;

        if (!WriteFile(m_hComm, data, len, &dwWrite, 0))
            return -1;

        return dwWrite;
        }

int CPCSerial::Receive(char *data, unsigned int len, unsigned short timeoutMs)
        {
        COMMTIMEOUTS timeout;
        DWORD dwRead;

        if (timeoutMs != m_timeout)
                {
                timeout.ReadIntervalTimeout = 0;
                timeout.ReadTotalTimeoutConstant = timeoutMs;
                timeout.ReadTotalTimeoutMultiplier = 0;
                timeout.WriteTotalTimeoutConstant = 0;
                timeout.WriteTotalTimeoutMultiplier = 0;
                if (!SetCommTimeouts(m_hComm, &timeout))
                    return -1;

                m_timeout = timeoutMs;
                }

        if (!ReadFile(m_hComm, data, len, &dwRead, 0))
                return -1;

        return dwRead;
        }


