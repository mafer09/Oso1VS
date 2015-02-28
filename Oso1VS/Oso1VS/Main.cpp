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

class MotherBoard
{
public:

	MotherBoard();
	void setBusyComponent(int component, int time);
	void setIdleComponent(int component);
	int checkCoreAvailability();
	int checkDiskAvailability();
	int checkInputAvailability();
	string determineState(bool state);
	void updateTotalCores(int cores);
	bool getComponentState(int component);
	int getComponentUsage(int component);
	//ADD QUEUE FUNCTIONS
	int getTotalCores();
	void resetTotalCores();

private:
	bool Cores[4]; //false = idle || true = busy
	bool DISK, INPUT; // Cores: 1-4 Disk 5: Input:6
	int CoresAvailability[4];
	int DISK_available, INPUT_available;
	queue<ProcessStep> ReadyQueue, DiskQueue, InputQueue;
	int totalCores;
};
MotherBoard::MotherBoard()
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
void MotherBoard::setBusyComponent(int component, int time)
{
	switch (component)
	{
	case 1:
		Cores[0] = true;
		CoresAvailability[0] += time;
		break;
	case 2:
		Cores[1] = true;
		CoresAvailability[1] += time;
		break;
	case 3:
		Cores[2] = true;
		CoresAvailability[2] += time;
		break;
	case 4:
		Cores[3] = true;
		CoresAvailability[3] += time;
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
void MotherBoard::setIdleComponent(int component)
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
int MotherBoard::checkCoreAvailability()
{
	vector<int> freeCores;
	int locationOfFreeCore = -1;
	int lowestTime = 20000;
	
	for (int i = 0; i < 4; i++)
	{
		if (Cores[i] == false)
		{
			freeCores.push_back(i);
		}
	}
	if (freeCores.empty()) // Return if the there are no free cores
	{
		return -3;
	}
	else
	{
		for (int i = 0; i < freeCores.size(); i++)
		{

			if (CoresAvailability[freeCores[i]] < lowestTime)
			{
				lowestTime = CoresAvailability[i];
				locationOfFreeCore = freeCores[i];
			}
		}
		return locationOfFreeCore + 1;
	}
	/*if (!Cores[0] && lowestTime == CoresAvailability[0])
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
	}*/
}
int MotherBoard::checkDiskAvailability()
{
	if (DISK == false)
	{
		return 5;
	}
	else
	{
		return -3;
	}
}
int MotherBoard::checkInputAvailability()
{
	if (INPUT == false)
	{
		return 6;
	}
	else
	{
		return -3;
	}
}
string MotherBoard::determineState(bool state)
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
void MotherBoard::updateTotalCores(int cores)
{
	totalCores += cores;
}
bool MotherBoard::getComponentState(int component)
{
	switch (component)
	{
	case 1:
		return Cores[0];
		break;
	case 2:
		return Cores[1];
		break;
	case 3:
		return Cores[2];
		break;
	case 4:
		return Cores[3];
		break;
	case 5:
		return DISK;
		break;
	case 6:
		return INPUT;
		break;
	default:
		cout << "Error: Check getComponentState function";
		return 0;
		break;
	}
}
int MotherBoard::getComponentUsage(int component)
{
	switch (component)
	{
	case 1:
		return CoresAvailability[0];
		break;
	case 2:
		return CoresAvailability[1];
		break;
	case 3:
		return CoresAvailability[2];
		break;
	case 4:
		return CoresAvailability[3];
		break;
	case 5:
		return DISK_available;
		break;
	case 6:
		return INPUT_available;
		break;
	default:
		cout << "Error: Check busyComponent function";
		return -3;
		break;
	}
}
int MotherBoard::getTotalCores()
{
	return totalCores;
}
void MotherBoard::resetTotalCores()
{
	totalCores = 0;
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

void printReport(int currentProcessPosition, int totalProcesses, Process processTable[], MotherBoard components)
{
	int num = 1;
	cout << "Process " << processTable[currentProcessPosition]._Name
		<< " terminates at t=" << processTable[currentProcessPosition]._Timer << "\n";
	for (int i = 1; i < 5; i++)
	{
		cout << "Core " << num << " is " << components.determineState(components.getComponentState(i)) << "\n";
		num++;
	}
	cout << "Disk is " << components.determineState(components.getComponentState(5)) << "\n";
	cout << "Input is " << components.determineState(components.getComponentState(6)) << "\n";
	//ADD WHAT IT CONTAINS!!
	/*cout << "Ready Queue contains: " << components.determineState(components.ReadyQueue.empty()) << "\n";
	cout << "Disk Queue contains: " << components.determineState(components.DiskQueue.empty()) << "\n";
	cout << "Input Queue contains: " << components.determineState(components.InputQueue.empty()) << "\n\n";*/

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
	cout << "Average number of BUSY Cores: " << components.getTotalCores() / processTable[currentProcessPosition]._Timer << "\n";
	components.resetTotalCores();
};
void executeProcess(int processLocation, Process processTable[], MotherBoard laptop, int totalProcesses)
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
			availableResult = laptop.checkCoreAvailability(); // check for free cores
			//all cores are busy
			if (availableResult == -3)
			{
				//laptop.ReadyQueue.push(processTable[processLocation]._Priori[prioriPosition]); //place process in the ready queue
				processTable[processLocation].updateStatus(3);	//update status of the process to ready
				// check when next is free FUNCTION
			}
			// a core is idle
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;

				laptop.setBusyComponent(availableResult, executionTime); //make the idle core busy and place how long it would be unavailable
				processTable[processLocation].updateStatus(1); // update status of the process to running
				processTable[processLocation].updateCPUtime(executionTime); // add onto time in cpu
				processTable[processLocation].updateTimer(executionTime); // add onto process timer

			}
		}
		else if (component == "I/O")
		{
			availableResult = laptop.checkDiskAvailability();
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				//laptop.DiskQueue.push(processTable[processLocation]._Priori[prioriPosition]);
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
					laptop.setBusyComponent(availableResult, executionTime);  //make disk busy and how long
					processTable[processLocation].updateTimer(executionTime); // add onto process timer
				}
			}
		}
		else if (component == "INPUT")
		{
			availableResult = laptop.checkInputAvailability();
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				//laptop.InputQueue.push(processTable[processLocation]._Priori[prioriPosition]);
				// check when next is Free FUNCTION
			}
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
				laptop.setBusyComponent(availableResult, executionTime); // make input busy and how long
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
	int totalExecutableLines = 0;
	int nextProcess = -1;
	MotherBoard laptop;

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
			totalExecutableLines++;
		}
	}


	// simulation
	for (int j = 0; j < totalExecutableLines; j++)
	{
		nextProcess = findLowestExecutionTime(processNumber, processTable);
		executeProcess(nextProcess, processTable, laptop, processNumber);
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
