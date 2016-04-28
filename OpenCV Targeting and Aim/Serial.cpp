#include "Serial.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define uint32 unsigned int
Serial::Serial(WCHAR *PORT)
{
	this->isConnected = false;

	this->serialHandler = CreateFile(PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->serialHandler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Handle not attached, port not avail");
		}
		else
			printf("error");
	}
	else
	{
		DCB dcbSParam = { 0 };

		if (!GetCommState(this->serialHandler, &dcbSParam/*Not pointer?*/))
			printf("can't get ser param");
		else
		{
			dcbSParam.BaudRate = CBR_9600;
			dcbSParam.ByteSize = 8;//?
			dcbSParam.StopBits = ONESTOPBIT; //thanks msdn documentation for the wonderful help
			dcbSParam.Parity = NOPARITY;
			dcbSParam.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(serialHandler, &dcbSParam))
				printf("can't set ser param");
			else
			{
				this->isConnected = true;
				PurgeComm(this->serialHandler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(2000);
			}
		}

	}
}

Serial::~Serial()
{
	if (this->isConnected)
	{
		this->isConnected = false;
		CloseHandle(this->serialHandler);
	}
}

int Serial::read(char *buffer, unsigned int numBytes)
{
	DWORD bytesRead;
	uint32 nextRead;

	ClearCommError(this->serialHandler, &this->err, &this->stat);
	if (this->stat.cbInQue > 0)
	{
		if (this->stat.cbInQue > numBytes)
			nextRead = numBytes;
		else
			nextRead = this->stat.cbInQue;
		if (ReadFile(this->serialHandler, buffer, nextRead, &bytesRead, NULL))
			return bytesRead;
	}
	return 0;
}

bool Serial::write(char *buffer, unsigned int numBytes)
{
	DWORD bytesToSend;

	if (!WriteFile(this->serialHandler, (void *)buffer, numBytes, &bytesToSend, 0))
	{
		ClearCommError(this->serialHandler, &this->err, &this->stat);
		return false;
	}
	else
		return true;
}