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

        long m_timeout;
        int m_baud;
        int m_port;
        };

#endif
