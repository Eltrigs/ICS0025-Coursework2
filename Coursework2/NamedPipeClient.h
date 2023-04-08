#pragma once
#include <string>

class NamedPipeClient 
{
	/*	Commands for controlling the server :
		1. To close the server type command exit.
		2. To return the server to its initial state type command reset.
		3. To block the operating type command cancel.
		4. To unblock type command resume.
	*/
	
private:
	/*connect opens the pipe file and sends the first ready message.*/
	void connect();

	/*stop sends the stop message (see above) and closes the pipe file. After this command the
	client application must stay active and the user must be able to type command connect
	once more.*/
	void disconnect();


	void reset();
	void cancel();
	void resume();

public:
	void parseCommand(std::string command);
};