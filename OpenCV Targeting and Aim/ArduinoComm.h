#pragma once
class ArduinoComm
{
public:
	ArduinoComm();
	~ArduinoComm();
	 Serial *Arduino;
	std::string _PORT;
};

