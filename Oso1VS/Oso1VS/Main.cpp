#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
using namespace std;

struct ProcessStep
{
	string Command;
	int Time;
};

class System
{
public:
	bool CPU1, CPU2, CPU3, CPU4, DISK, INPUT; // Cores: 1-4 Disk 5: Input:6
	queue<ProcessStep> ReadyQueue, DiskQueue, InputQueue;
	double AVGBusyCores;

	System();
	void busyComponent(int component);
	void idleComponent(int component);
};
System::System()
{
	CPU1, CPU2, CPU3, CPU4, DISK, INPUT = false;
	AVGBusyCores = 0.0;
}
void System::busyComponent(int component)
{
	switch (component)
	{
	case 1:
		CPU1 = true;
		break;
	case 2:
		CPU2 = true;
		break;
	case 3:
		CPU3 = true;
		break;
	case 4:
		CPU4 = true;
		break;
	case 5:
		DISK = true;
		break;
	case 6:
		INPUT = true;
		break;
	default:
		cout << "Error: Check busyComponent function";
		break;
	}
}
void System::idleComponent(int component)
{
	switch (component)
	{
	case 1:
		CPU1 = false;
		break;
	case 2:
		CPU2 = false;
		break;
	case 3:
		CPU3 = false;
		break;
	case 4:
		CPU4 = false;
		break;
	case 5:
		DISK = false;
		break;
	case 6:
		INPUT = false;
		break;
	default:
		cout << "Error: Check idleComponent function";
		break;
	}
}

class Process
{
public:

	int _Name;
	vector<ProcessStep> _Priori;
	int _Start;
	int _Timer;
	string _Status; /// 1: Running, 2: Waiting, 0: Terminated
	int _CPUtime;
	
	Process();
	void addProcessStep(ProcessStep data);
	void addName(int name);
	void updateTimer(int time);
	void setTimer(int time);
	void updateCPUtime(int time);
	void updateStatus(int status);


};
Process::Process()
{
	_Timer = 0;
	_Name = 0;
	_Status = "";
	_CPUtime = 0;
	_Start = 0;
}
void Process::addProcessStep(ProcessStep data)
{
	_Priori.push_back(data);
}
void Process::addName(int name)
{
	_Name = name;
}
void Process::updateTimer(int time)
{
	_Timer += time;
}
void Process::setTimer(int time)
{
	_Start = time;
	_Timer = time;
}
void Process::updateCPUtime(int time)
{
	_CPUtime += time;
}
void Process::updateStatus(int status)
{
	switch (status)
	{
	case 0:
		_Status = "Terminated";
		break;
	case 1:
		_Status = "Running";
		break;
	case 2:
		_Status = "Waiting";
		break;
	default:
		cout << "Error: Check updateStatus function";
		break;
	}
}

vector<ProcessStep> retrieveData()
{
	vector<ProcessStep> fileContents;
	string temp;
	stringstream _DATA;
	ifstream inputTxt;
	ProcessStep line;

	inputTxt.open("input1.txt.txt");

	if (!inputTxt.is_open())
	{
		cout << "Failed to Open File";
	}

	//Getting contents from the file stored into the vector
	while (!inputTxt.eof())
	{
		getline(inputTxt, temp);
		_DATA.str("");
		_DATA.clear();
		_DATA << temp;

		_DATA >> line.Command;
		_DATA >> line.Time;

		fileContents.push_back(line);
	}
	inputTxt.close();

	return fileContents;
};


int main()
{
	int processNumber = -1;
	Process processTable[10]; //**MAY NEED TO BE CHANGED
	vector<ProcessStep> fileContents = retrieveData();
	int currentTime = 0;
	int minimumTime = -1;
	string nextProcess = "";

	///Parses the data == Creates proceess table
	for (int i = 0; i < fileContents.size(); i++)
	{
		/// only if its NEW
		if (fileContents[i].Command == "NEW")
		{
			processNumber++;
			processTable[processNumber].addName(fileContents[i].Time);
		}
		/// only if its START
		else if (fileContents[i].Command == "START")
		{
			processTable[processNumber].setTimer(fileContents[i].Time);
		}
		/// everyone else
		else		
		{
			processTable[processNumber].addProcessStep(fileContents[i]);
		}
	}

	/// Check who begins
	for (int i = 0; i <= processNumber; i++)
	{
		if (processTable[i]._Start < minimumTime)
		{
			minimumTime = processTable[i]._Start;
			nextProcess = processTable[i]._Name;
		}
	}


	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}


