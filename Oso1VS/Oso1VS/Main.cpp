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

//SYSTEM Functions

int checkCoreAvailability( bool Cores[], int CoresAvailability[])
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
		return locationOfFreeCore;
	}

};
int checkComponentAvailability(bool component)
{
	if (component == true)
	{
		return -3;
	}
	else
	{
		return 0;
	}
};
string determineState(bool state)
{
	if (state)
	{
		return "BUSY";
	}
	else
	{
		return "IDLE";
	}
};

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

/*void printReport(int currentProcessPosition, int totalProcesses, Process processTable[])
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
	cout << "Average number of BUSY Cores: " << components.getTotalCores() / processTable[currentProcessPosition]._Timer << "\n";
	components.resetTotalCores();
};*/
void executeProcess(int processLocation, Process processTable[], int totalProcesses, bool Cores[], int CoresAvailability[], bool Disk, int DiskAvailability, bool Input, int InputAvailability)
{
	int availableResult = -1;
	int executionTime = -1;
	int prioriPosition = processTable[processLocation]._currentPriori; // get the next step to process from the priori

	if (prioriPosition == (processTable[processLocation]._Priori.size())-1 ) // check that the process hasnt terminated
	{
		processTable[processLocation].updateStatus(0);
		//printReport(processLocation, totalProcesses, processTable);
	}
	else
	{
		string component = processTable[processLocation]._Priori[prioriPosition].Command; // get the next component to be used 

		if (component == "CPU")
		{
			availableResult = checkCoreAvailability(Cores, CoresAvailability); // check for free cores
			if (availableResult == -3) //All cores are busy
			{
				//laptop.ReadyQueue.push(processTable[processLocation]._Priori[prioriPosition]); //place process in the ready queue
				processTable[processLocation].updateStatus(3);	//update status of the process to ready
				// check when next is free FUNCTION
				//FIX READY QUEUES
			}
			// a core is idle
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;

				Cores[availableResult] = true;
				CoresAvailability[availableResult] += executionTime;  //make the idle core busy and place how long it would be unavailable
				processTable[processLocation].updateStatus(1); // update status of the process to running
				processTable[processLocation].updateCPUtime(executionTime); // add onto time in cpu
				processTable[processLocation].updateTimer(executionTime); // add onto process timer

			}
		}
		else if (component == "I/O")
		{
			availableResult = checkComponentAvailability(Disk);
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				//laptop.DiskQueue.push(processTable[processLocation]._Priori[prioriPosition]);
				// check when next is free FUNCTION
				// FIX THIS!
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
					Disk = true;
					DiskAvailability += executionTime;	//make disk busy and how long
					processTable[processLocation].updateTimer(executionTime); // add onto process timer
				}
			}
		}
		else
		{
			availableResult = checkComponentAvailability(Input);
			processTable[processLocation].updateStatus(2);
			if (availableResult == -3)
			{
				//laptop.InputQueue.push(processTable[processLocation]._Priori[prioriPosition]);
				// check when next is Free FUNCTION
				//FIX THIS!
			}
			else
			{
				executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
				Input = true; // make input busy and how long
				InputAvailability += executionTime;
				processTable[processLocation].updateTimer(executionTime);
			}
		}

		processTable[processLocation].updateCurrentPriori(prioriPosition++);
	}
};

int main()
{
	vector<ProcessStep> fileContents = retrieveData();
	Process processTable[10]; //**MAY NEED TO BE CHANGED
	
	int processNumber = -1;
	int totalExecutableLines = 0;
	int nextProcess = -1;
	
	//SYSTEM Components
	bool Cores[4] = { false }; //false = idle || true = busy
	bool DISK = false;
	bool INPUT = false; // Cores: 1-4 Disk 5: Input:6
	int CoresAvailability[4] = { 0 };
	int DISK_available = 0;
	int INPUT_available = 0;
	queue<ProcessStep> ReadyQueue;
	queue<ProcessStep> DiskQueue;
	queue<ProcessStep> InputQueue;
	int totalCores = 0;


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
		executeProcess(nextProcess, processTable, processNumber, Cores, CoresAvailability,
			DISK, DISK_available, INPUT, INPUT_available);
	}


	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}
