#include "NamedPipeClient.h"


void NamedPipeClient::executeCommand(std::string command)
{
    if (command.compare("ECHO") == 0)
    {
        std::cout << "Echo cho ho o o" << std::endl;
    }
    else if (command.compare("connect") == 0)
    {
        if (this->isConnected)
        {
            std::cout << "Server is already connected" << std::endl;
            return;
        }
        this->connect();
    }
    else if (command.compare("ready") == 0)
    {
        while (true)
        {
            if (!this->isConnected || this->exitSignal == TRUE)
            {
                this->isLoopDone = TRUE;
                std::cout << "Server is not connected, stopped sending \"ready\"" << std::endl;
                break;
            }
            this->isLoopDone = FALSE;
            std::thread infiniteThreads([this]() {
                std::string response = this->sendMessage("ready");
                this->parseBuffer(response);
                });
            infiniteThreads.join();
            this->isLoopDone = TRUE;
        }
    }
    else if (command.compare("stop") == 0)
    {
        if (!this->isConnected)
        {
            std::cout << "Server is already disconnected" << std::endl;
            return;
        }
        this->stop();
    }
    else if (command.compare("exit") == 0)
    {
        this->exit();
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
}

int NamedPipeClient::connect()
{
    BOOL fSuccess = FALSE;
    LPCTSTR lpszPipename = PIPESERVERNAME;
    LPCSTR lpvMessage = "ready";
    DWORD dwMode;

    // Try to open a named pipe; wait for it, if necessary. 

    while (1)
    {
        this->hPipe = CreateFile(
            lpszPipename,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 


        // Break if the pipe handle is valid. 
        if (this->hPipe != INVALID_HANDLE_VALUE) {
            this->exitSignal = FALSE;
            this->isConnected = TRUE;
            break;
        }

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
        this->hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
        return -1;
    }

    // Send the first ready message to the pipe server.
    std::string response = this->sendMessage("ready");
    this->parseBuffer(response);

    return 0;
}

void NamedPipeClient::stop()
{
    //Signal the "ready" loop to stop
    this->isConnected = FALSE;
    //Wait for the "ready" loop thread to exit
    while (!this->isLoopDone) {}
    //Send stop message and close connection
    this->sendMessage("stop");
    
    CloseHandle(this->hPipe);
    
}

void NamedPipeClient::exit()
{
    //Set exit signal to true, so the "ready" loop can stop safely
    this->exitSignal = TRUE;
    //Wait for the "ready" loop to return
    while (!this->isLoopDone) {}
    //Cut the connection if it exists
    if (this->isConnected)
    {
        this->sendMessage("exit");
        //Close the handle
        CloseHandle(this->hPipe);
        this->isConnected = FALSE;
    }

    if (this->refToData->CountItems() > 0) {
        refToData->PrintAll();
    }

}

std::string NamedPipeClient::sendMessage(LPCSTR lpvMessage)
{
    DWORD cbToWrite, cbWritten, dwMode;
    BOOL fSuccess = FALSE;

    // The pipe is connected; change to message-read mode. 
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        this->hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
        this->isConnected = FALSE;
        return const_cast<char*>("error");
    }

    // Send a message to the pipe server. 

    cbToWrite = (lstrlenA(lpvMessage) + 1) * sizeof(char);
    printf("Sending %d byte message: \"%s\"\n", cbToWrite, lpvMessage);

    fSuccess = WriteFile(
        this->hPipe,            // pipe handle 
        lpvMessage,             // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 

    if (!fSuccess)
    {
        _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
        this->isConnected = FALSE;
        return const_cast<char*>("error");
    }

    //Don't wait for a message after sending stop or exit
    if (!strcmp(lpvMessage, "stop") || !strcmp(lpvMessage, "exit"))
    {
        return const_cast<char*>("Closing connection");
    }

    printf("\nMessage sent to server, receiving reply as follows:\n");
    return this->readBuffer();

}

std::string NamedPipeClient::readBuffer()
{
    BOOL fSuccess = FALSE;
    char chBuf[BUFSIZE];
    DWORD cbRead;
    do
    {
        // Read from the pipe. 

        fSuccess = ReadFile(
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            BUFSIZE * sizeof(char),  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA) {
            this->isConnected = FALSE;
            break;
        }
            
        if (this->exitSignal == TRUE || this->isConnected == FALSE)
        {
            std::cout << "Server has been disconnected, exiting \"ready\" thread." << std::endl;
            break;
        }
        printf("\"%s\"\n", chBuf);
    } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

    if (!fSuccess)
    {
        _tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
        this->isConnected = FALSE;
        return nullptr;
    }

    //Apparently the buffer didn't include a \0 terminator so let's add it ourselves
    //Make it into a std::string at the same time.
    char temp[BUFSIZE+1];
    memcpy(temp, (const char*)chBuf, cbRead);
    temp[cbRead] = '\0';
    std::string response = std::string(temp);
    return response;
}

void NamedPipeClient::parseBuffer(std::string str)
{
    std::vector<std::string> words;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }

    char group = words[0].at(0);
    int subgroup = std::stoi(words[1]);
        
    std::string name = std::accumulate(std::next(words.begin() + 1), std::prev(words.end(), 3), std::string(""),
        [](const std::string& a, const std::string& b) {
            return a + " " + b;
        }
    );
    name.erase(std::remove_if(name.begin(), name.end(), [](char c) { return (c == '<' || c == '>'); }), name.end());

    int day = std::stoi(words[words.size() - 3]);
    std::string s_month = words[words.size() - 2];
    int year = std::stoi(words[words.size() - 1]);

    static const std::unordered_map<std::string, int> monthMap{
       {"Jan", 1},
       {"Feb", 2},
       {"Mar", 3},
       {"Apr", 4},
       {"May", 5},
       {"Jun", 6},
       {"Jul", 7},
       {"Aug", 8},
       {"Sep", 9},
       {"Oct", 10},
       {"Nov", 11},
       {"Dec", 12}
    };

    auto it = monthMap.find(s_month);
    if (it != monthMap.end()) {
         int month = it->second;
         //std::cout << "Extracted:\n\tGroup: " << group << "\n\tSubgroup:" << subgroup << "\n\tName: " << name << "\n\tD,M,Y: " << day << month << year << std::endl;
         Item newItem = Item(group, subgroup, name, Date(day, month, year));
         this->refToData->InsertItem(group, subgroup, name, Date(day, month, year));
    }
    else {
        // Some weird month
        std::cout << "Failed to parse month";
    }
}

void NamedPipeClient::addItem(Item item)
{
}

NamedPipeClient::NamedPipeClient(Data* pData)
{
    this->refToData = pData;
}
