#pragma once
class Serial
{
public:
	Serial(WCHAR *);
	~Serial();
	HANDLE serialHandler;
	bool isConnected;

	DWORD err;
	int read(char *, unsigned int);
	bool write(char *, unsigned int);
	COMSTAT stat;
	
};

 