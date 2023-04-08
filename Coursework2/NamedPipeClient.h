#pragma once
#include <Windows.h>
#include <string>
#include "Data.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 1024
#define PIPESERVERNAME L"\\\\.\\pipe\\ICS0025"

class NamedPipeClient 
{
	/*	Commands for controlling the server :
		1. To close the server type command exit.
		2. To return the server to its initial state type command reset.
		3. To block the operating type command cancel.
		4. To unblock type command resume.
	*/
	
private:
	HANDLE hPipe;
	Data* refToData;
	
	/*connect opens the pipe file and sends the first ready message.*/
	int connect();

	/*stop sends the stop message (see above) and closes the pipe file. After this command the
	client application must stay active and the user must be able to type command connect
	once more.*/
	void disconnect();
	void reset();
	void cancel();
	void resume();

	void sendMessage(std::string message);
	std::string readBuffer();
	Item parseBuffer(std::string);
	void addItem(Item item);

public:
	NamedPipeClient(Data* pData);
	void executeCommand(std::string command);
};