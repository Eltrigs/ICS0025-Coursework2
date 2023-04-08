#pragma once
#include <Windows.h>
#include <string>
#include "Data.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <unordered_map>

#define BUFSIZE 1024
#define PIPESERVERNAME L"\\\\.\\pipe\\ICS0025"

class NamedPipeClient 
{
	/*	Commands for controlling the server in it's own application:
		1. exit
		2. reset
		3. cancel
		4. resume
	*/

	/* Client commands, aka this application:
	*  1. connect
	*  2. ready
	*  3. stop
	*  4. exit
	*/
	
private:
	HANDLE hPipe;
	Data* refToData;
	BOOL isConnected = FALSE;
	BOOL isLoopDone = TRUE;
	BOOL exitSignal = FALSE;
	/*connect opens the pipe file and sends the first ready message.*/
	int connect();

	/* stop sends the stop message (see above) and closes the pipe file. After this command the
	 * client application must stay active and the user must be able to type command connect
	 * once more.
	 */
	void stop();

	/* exit forces the client to print the contents of data structure and exit the application. The
	 * client application must obey this command at any moment.
	 */
	void exit();

	std::string sendMessage(LPCSTR lpvMessage);
	std::string readBuffer();
	void addItemFromString(std::string response);

public:
	NamedPipeClient(Data* pData);
	void executeCommand(std::string command);
};