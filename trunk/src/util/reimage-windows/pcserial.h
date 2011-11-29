#ifndef _PCSERIAL_H
#define _PCSERIAL_H

#include <windows.h>

class CPCSerial
{
public:
        CPCSerial();
        ~CPCSerial();
        int Open(unsigned int comport, unsigned int baud);
        int Close();

        int Send(const char *data, unsigned int len, unsigned short timeoutMs);
        int Receive(char *data, unsigned int len, unsigned short timeoutMs);

        static int GetPort();
private:
        HANDLE m_hComm;
        int m_baud;
        unsigned int m_comport;
        long m_timeout;
        };

#endif
