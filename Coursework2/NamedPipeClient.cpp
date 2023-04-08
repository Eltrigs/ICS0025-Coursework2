#include "NamedPipeClient.h"

int NamedPipeClient::connect()
{
    HANDLE hPipe;
    BOOL   fSuccess = FALSE;
    LPCTSTR lpszPipename = PIPESERVERNAME;

    while (true)
    {
        HANDLE hPipe;
        LPCSTR lpvMessage = "ready";
        char chBuf[BUFSIZE];
        BOOL fSuccess = FALSE;
        DWORD cbRead, cbToWrite, cbWritten, dwMode;
        LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\ICS0025");

        // Try to open a named pipe; wait for it, if necessary. 

        while (1)
        {
            hPipe = CreateFile(
                lpszPipename,   // pipe name 
                GENERIC_READ |  // read and write access 
                GENERIC_WRITE,
                0,              // no sharing 
                NULL,           // default security attributes
                OPEN_EXISTING,  // opens existing pipe 
                0,              // default attributes 
                NULL);          // no template file 

            // Break if the pipe handle is valid. 

            if (hPipe != INVALID_HANDLE_VALUE)
                break;

            // Exit if an error other than ERROR_PIPE_BUSY occurs. 

            if (GetLastError() != ERROR_PIPE_BUSY)
            {
                _tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
                return -1;
            }

            // All pipe instances are busy, so wait for 20 seconds. 

            if (!WaitNamedPipe(lpszPipename, 20000))
            {
                printf("Could not open pipe: 20 second wait timed out.");
                return -1;
            }
        }

        // The pipe connected; change to message-read mode. 

        dwMode = PIPE_READMODE_MESSAGE;
        fSuccess = SetNamedPipeHandleState(
            hPipe,    // pipe handle 
            &dwMode,  // new pipe mode 
            NULL,     // don't set maximum bytes 
            NULL);    // don't set maximum time 
        if (!fSuccess)
        {
            _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
            return -1;
        }

        // Send a message to the pipe server. 

        cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
        printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

        fSuccess = WriteFile(
            hPipe,                  // pipe handle 
            lpvMessage,             // message 
            cbToWrite,              // message length 
            &cbWritten,             // bytes written 
            NULL);                  // not overlapped 

        if (!fSuccess)
        {
            _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
            return -1;
        }

        printf("\nMessage sent to server, receiving reply as follows:\n");

        do
        {
            // Read from the pipe. 

            fSuccess = ReadFile(
                hPipe,    // pipe handle 
                chBuf,    // buffer to receive reply 
                BUFSIZE * sizeof(char),  // size of buffer 
                &cbRead,  // number of bytes read 
                NULL);    // not overlapped 

            if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
                break;

            printf("\"%s\"\n", chBuf);
        } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

        if (!fSuccess)
        {
            _tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
            return -1;
        }

        printf("\n<End of message, press ENTER to terminate connection and exit>");
        _getch();

        CloseHandle(hPipe);
    }
}

void NamedPipeClient::disconnect()
{
}

void NamedPipeClient::reset()
{
}

void NamedPipeClient::cancel()
{
}

void NamedPipeClient::resume()
{
}

void NamedPipeClient::sendMessage(std::string message)
{
}

std::string NamedPipeClient::readBuffer()
{
    return std::string();
}

Item NamedPipeClient::parseBuffer(std::string)
{
    return Item();
}

void NamedPipeClient::addItem(Item item)
{
}

NamedPipeClient::NamedPipeClient(Data* pData)
{
    this->refToData = pData;
}

void NamedPipeClient::executeCommand(std::string command)
{
    if (command.compare("ECHO") == 0)
    {
        std::cout << "Echo cho ho o o" << std::endl;
    }
    else if (command.compare("connect") == 0)
    {
        this->connect();
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
}
