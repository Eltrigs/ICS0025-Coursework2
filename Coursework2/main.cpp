#include "Main.h"

static const char* NamesFile = "Birds.txt"; // http://www.jimpalt.org/birdwatcher/name.html 941 names of different birds, ASCII code, OD OA after each row
default_random_engine Generator;
static uniform_int_distribution<int> NameDistribution;
static vector<string> Names;
uniform_int_distribution<int> GroupDistribution('A', 'Z');
uniform_int_distribution<int> SubgroupDistribution(0, 99);

int main()
{
	//Create a new Data() to which the pipeserver is going to write information
	Data* pData = new Data();

	//Create the pipeserver object and give it a reference to the DataStructure
	NamedPipeClient pipeClient = NamedPipeClient(pData);

	std::thread commandThread;
	std::string command;
	//Start asking for inputs from the console to the pipeserver
	while (true)
	{
		std::cin >> command;

		//To get back from this infinite loop the user has to type exit in the console
		//Then the final commands will be sent and the application will exit safely
		if (command.compare("exit") == 0)
		{
			commandThread = std::thread([&] { pipeClient.executeCommand(command); });
			commandThread.join();
			break;
		}

		//Since everything needs to be run concurrently, start a thread for every command
		commandThread = std::thread ([&] { pipeClient.executeCommand(command); });
		
		// Detaching might actually be dangerous:
		/* When a program terminates (ie, main returns) the remaining detached threads 
		 * executing in the background are not waited upon; 
		 * instead their execution is suspended and their thread-local objects are not destructed.
		 * https://stackoverflow.com/questions/22803600/when-should-i-use-stdthreaddetach
		 */
		commandThread.detach();
	}

	return 0;
}


void PrepareNames()
{
	ifstream file;
	file.open(NamesFile, fstream::in);
	if (!file.good())
		throw exception("Unable to open the names file");
	char buf[100];
	while (file.good())
	{
		file.getline(buf, 100);
		if (buf[0])
			Names.push_back(string(buf));
	}
	file.close();
	uniform_int_distribution<int> distr(0, Names.size() - 1);
	NameDistribution = distr;
}

string CreateRandomName()
{
	int i = NameDistribution(Generator);
	return Names[i];
}