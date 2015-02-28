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
	bool Cores[4]; 
	bool DISK, INPUT; // Cores: 1-4 Disk 5: Input:6
	int CoresAvailability[4];
	int DISK_available, INPUT_available;
	queue<ProcessStep> ReadyQueue, DiskQueue, InputQueue;
	int totalCores;

	System();
	void busyComponent(int component, int time);
	void idleComponent(int component);
	int coreAvailability();
	int diskAvailability();
	int inputAvailability();
	string determineState(bool state);
	void updateTotalCores(int cores);
};
System::System()
{
	Cores[4] = { false};
	CoresAvailability[4] = { 0 };
	DISK = false;
	INPUT = false;
	DISK_available = 0;
	INPUT_available = 0;
	totalCores = 0;
	queue<ProcessStep> ReadyQueue;
	queue<ProcessStep> DiskQueue;
	queue<ProcessStep> InputQueue;
}
void System::busyComponent(int component, int time)
{
	switch (component)
	{
	case 1:
		Cores[0] = true;
		CoresAvailability[0] += time;
		break;
	case 2:
		Cores[1] = true;
		CoresAvailability[0] += time;
		break;
	case 3:
		Cores[2] = true;
		CoresAvailability[0] += time;
		break;
	case 4:
		Cores[3] = true;
		CoresAvailability[0] += time;
		break;
	case 5:
		DISK = true;
		DISK_available += time;
		break;
	case 6:
		INPUT = true;
		INPUT_available += time;
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
		Cores[0] = false;
		break;
	case 2:
		Cores[1] = false;
		break;
	case 3:
		Cores[2] = false;
		break;
	case 4:
		Cores[3] = false;
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
int System::coreAvailability()
{
	int lowestTime = 1;
	for (int i = 0; i < 4; i++)
	{
		if (CoresAvailability[i] < lowestTime)
		{
			lowestTime = CoresAvailability[i];
		}
	}
	if (!Cores[0] && lowestTime == CoresAvailability[0])
	{
		return 1;
	}
	else if (!Cores[1] && lowestTime == CoresAvailability[1])
	{
		return 2;
	}
	else if (!Cores[2] && lowestTime == CoresAvailability[2])
	{
		return 3;
	}
	else if (!Cores[3] && lowestTime == CoresAvailability[3])
	{
		return 4;
	}
	else
	{
		return -3;
	}
}
int System::diskAvailability()
{
	if (!DISK)
	{
		return 5;
	}
	else
	{
		return -3;
	}
}
int System::inputAvailability()
{
	if (!INPUT)
	{
		return 6;
	}
	else
	{
		return -3;
	}
}
string System::determineState(bool state)
{
	if (state)
	{
		return "BUSY";
	}
	else
	{
		return "IDLE";
	}
}
void System::updateTotalCores(int cores)
{
	totalCores += cores;
}

class Process
{
public:

	int _Name;
	vector<ProcessStep> _Priori;
	int _currentPriori;
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
	void updateCurrentPriori(int time);


};
Process::Process()
{
	_Timer = 0;
	_Start = 0;
	_CPUtime = 0;
	_currentPriori = 0;
	_Name = 0;
	_Status = "";
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
	case 3:
		_Status = "Ready";
		break;
	default:
		cout << "Error: Check updateStatus function";
		break;
	}
}
void Process::updateCurrentPriori(int time)
{
	_currentPriori = time;
}

vector<ProcessStep> retrieveData()
{
	vector<ProcessStep> fileContents;
	string temp;
	stringstream _DATA;
	ifstream inputTxt;
	ProcessStep line;
	int numberOfLines = 0;

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
		numberOfLines++;
	}
	inputTxt.close();

	return fileContents;
};
int findLowestExecutionTime(int totalProcesses, Process processTable[])
{
	int lowestExecutionTime = processTable[0]._Timer;
	int nextProcessLocation = -1;
	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Timer <= lowestExecutionTime)
		{
			lowestExecutionTime = processTable[i]._Timer;
			nextProcessLocation = i;
		}
	}
	return nextProcessLocation;
};

void printReport(int currentProcessPosition, int totalProcesses, Process processTable[], System components)
{
	int num = 1;
	cout << "Process " << processTable[currentProcessPosition]._Name
		<< " terminates at t=" << processTable[currentProcessPosition]._Timer << "\n";
	for (int i = 0; i < 4; i++)
	{
		cout << "Core " << num << " is " << components.determineState(components.Cores[i]) << "\n";
		num++;
	}
	cout << "Disk is " << components.determineState(components.DISK) << "\n";
	cout << "Input is " << components.determineState(components.INPUT) << "\n";
	//ADD WHAT IT CONTAINS!!
	cout << "Ready Queue contains: " << components.determineState(components.ReadyQueue.empty()) << "\n";
	cout << "Disk Queue contains: " << components.determineState(components.DiskQueue.empty()) << "\n";
	cout << "Input Queue contains: " << components.determineState(components.InputQueue.empty()) << "\n\n";

	cout << "Process ID  | Start Time | CPU Time   |  Status" << "\n";
	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Status == "Terminated" && i != currentProcessPosition)
		{
			cout << processTable[i]._Name << "  |  " << processTable[i]._Start << "  |  " <<
				processTable[i]._CPUtime << "   |  " << processTable[i]._Status << "\n";
		}
		components.updateTotalCores(processTable[i]._CPUtime);
	}
	cout << "Average number of BUSY Cores: " << components.totalCores / processTable[currentProcessPosition]._Timer << "\n";
	components.totalCores = 0;
};
void executeProcess(int processLocation, Process processTable[], System laptop, int totalProcesses)
{
	int availableResult = -1;
	int executionTime = -1;
	int prioriPosition = processTable[processLocation]._currentPriori; // get the next step to process from the priori

	if (prioriPosition == (processTable[processLocation]._Priori.size())-1 ) // check that the process hasnt terminated
	{
		processTable[processLocation].updateStatus(0);
		printReport(processLocation, totalProcesses, processTable, laptop);
	}
	else
	{
		string component = processTable[processLocation]._Priori[prioriPosition].Command; // get the next component to be used 

		if (component == "CPU")
		{
			availableResult = laptop.coreAvailability(); // check for free cores
			//all cores are busy
			if (availableResult == -3)
			{
				laptop.ReadyQueue.push(processTable[processLocation]._Priori[prioriPosition]); //place process in the ready queue
				processTable[processLocation].updateStatus(3);	//update status of the process to ready
				// check when next is free FUNCTION
			}
			// a core is idle
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;

				laptop.busyComponent(availableResult, executionTime); //make the idle core busy and place how long it would be unavailable
				processTable[processLocation].updateStatus(1); // update status of the process to running
				processTable[processLocation].updateCPUtime(executionTime); // add onto time in cpu
				processTable[processLocation].updateTimer(executionTime); // add onto process timer

			}
		}
		else if (component == "I/O")
		{
			availableResult = laptop.diskAvailability();
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				laptop.DiskQueue.push(processTable[processLocation]._Priori[prioriPosition]);
				// check when next is free FUNCTION
			}
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
				if (executionTime == 0)
				{
					//DO EVERYTHING AGAIN (go back to cpu)
				}
				else
				{
					laptop.busyComponent(availableResult, executionTime);  //make disk busy and how long
					processTable[processLocation].updateTimer(executionTime); // add onto process timer
				}
			}
		}
		else if (component == "INPUT")
		{
			availableResult = laptop.inputAvailability();
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				laptop.InputQueue.push(processTable[processLocation]._Priori[prioriPosition]);
				// check when next is Free FUNCTION
			}
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
				laptop.busyComponent(availableResult, executionTime); // make input busy and how long
				processTable[processLocation].updateTimer(executionTime);
			}
		}
		else
		{
			cout << "Error: in execute process function";
		}
		processTable[processLocation].updateCurrentPriori(prioriPosition++);
	}
};

int main()
{
	int processNumber = -1;
	Process processTable[10]; //**MAY NEED TO BE CHANGED
	vector<ProcessStep> fileContents = retrieveData();
	int minimumTime = 1;
	int nextProcess = -1;
	System laptop;

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

	//verify starts the simulation
	/*for (int i = 0; i <= processNumber; i++)
	{
		if (processTable[i]._Start < minimumTime)
		{
			minimumTime = processTable[i]._Start;
			nextProcess = i;
		}
	}
	executeProcess(nextProcess, processTable, laptop, processNumber);*/

	// simulation
	for (int j = 0; j < fileContents.size(); j++)
	{
		nextProcess = findLowestExecutionTime(processNumber, processTable);
		executeProcess(nextProcess, processTable, laptop, processNumber);
	}

	/*/// Check who begins

	//Test printed store data

	/*cout<<processTable[i]._Name<<": \n";
	for (int j = 0; j < processTable[i]._Priori.size(); j++)
	{
	cout << processTable[i]._Priori[j].Command << " " << processTable[i]._Priori[j].Time << "\n";
	}
	}*/


	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}
