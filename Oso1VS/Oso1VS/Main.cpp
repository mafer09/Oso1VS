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
	string _Status; /// 1: Running, 2: Waiting, 0: Terminated 3:Ready
	int _CPUtime;
	
	Process();
	void addProcessStep(ProcessStep data);
	void addName(int name);
	void updateTimer(int time);
	void setTimer(int time);
	void updateCPUtime(int time);
	void updateStatus(int status);
	void updateCurrentPriori(int time);
	string getStatus();

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
	_currentPriori += time;
}
string Process::getStatus()
{
	return _Status;
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
	if (state == true)
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
int findLowestExecutionTime(int totalProcesses, Process processTable[], bool Cores[], int CoresAvailability[], bool DandI[], int DandIAvailability[])
{
	int lowestExecutionTime = processTable[0]._Timer;
	int nextProcessLocation = -1;
	string command = "";
	int currentPriori = -1;
	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Timer <= lowestExecutionTime)
		{
			lowestExecutionTime = processTable[i]._Timer;
			nextProcessLocation = i;
		}
	}
	if (processTable[nextProcessLocation]._Status != "Terminated")
	{
		currentPriori = processTable[nextProcessLocation]._currentPriori;
		command = processTable[nextProcessLocation]._Priori[currentPriori].Command;

		if (command == "CPU" || command == "ReadyQueue")
		{
			for (int i = 0; i < 4; i++)
			{
				if (CoresAvailability[i] == lowestExecutionTime)
				{
					Cores[i] = false;
				}
			}
		}
		else if (command == "I/O" || command == "DiskQueue")
		{
			if (DandIAvailability[0] == lowestExecutionTime)
			{
				DandI[0] = false;
			}
		}
		else if (command == "INPUT" || command == "InputQueue")
		{
			if (DandIAvailability[1] == lowestExecutionTime)
			{
				DandI[1] = false;
			}
		}
		else
		{
			cout << "Error: check findlowestexecution time function";
		}
	}

	return nextProcessLocation;
};
bool isProcessTerminated(Process processTable[],int currentProcess)
{
	if (processTable[currentProcess].getStatus() == "Terminated")
	{
		return true;
	}
	else
	{
		return false;
	}
};


void printReport(int currentProcessPosition, int totalProcesses, Process processTable[], bool Cores[], int CoresAvailability[], 
	bool DI[], int DIAvailability[], queue<ProcessStep> ReadyQ, queue<ProcessStep> DiskQ, queue<ProcessStep> InputQ)
{
	int coreNumber = 1;
	int totalCores = 0;

	cout << "Process " << processTable[currentProcessPosition]._Name
		<< " terminates at t=" << processTable[currentProcessPosition]._Timer << "\n";
	for (int i = 0; i < 4; i++)
	{
		cout << "Core " << coreNumber << " is " << determineState(Cores[i])<< "\n";
		coreNumber++;
	}
	cout << "Disk is " << determineState(DI[0]) << "\n";
	cout << "Input is " << determineState(DI[1]) << "\n";
	//ADD WHAT IT CONTAINS!!
	/*cout << "Ready Queue contains: " << components.determineState(components.ReadyQueue.empty()) << "\n";
	cout << "Disk Queue contains: " << components.determineState(components.DiskQueue.empty()) << "\n";
	cout << "Input Queue contains: " << components.determineState(components.InputQueue.empty()) << "\n\n";
	*/
	cout << "Process ID  | Start Time | CPU Time   |  Status" << "\n";
	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Status == "Terminated" && i != currentProcessPosition)
		{
			cout << processTable[i]._Name << "  |  " << processTable[i]._Start << "  |  " <<
				processTable[i]._CPUtime << "   |  " << processTable[i]._Status << "\n";
		}
		totalCores += processTable[i]._CPUtime;
	}
	cout << "Average number of BUSY Cores: " << totalCores / processTable[currentProcessPosition]._Timer << "\n";
};
void executeProcess(int processLocation, Process processTable[], int totalProcesses, 
	bool Cores[], int CoresAvailability[], bool DI[], int DIAvailability[], queue<ProcessStep> ReadyQ, queue<ProcessStep> DiskQ, queue<ProcessStep> InputQ)
{
	int availableResult = -1;
	int executionTime = -1;
	int totalLines = processTable[processLocation]._Priori.size();
	totalLines--;
	int prioriPosition = processTable[processLocation]._currentPriori; // get the next step to process from the priori
	string component = processTable[processLocation]._Priori[prioriPosition].Command; // get the next component to be used 
	ProcessStep temporaryStep;

	if (component == "CPU")
	{
		availableResult = checkCoreAvailability(Cores, CoresAvailability); // check for free cores
		if (availableResult == -3) //All cores are busy
		{
			processTable[processLocation].updateStatus(3);	//update status of the process to ready
			ReadyQ.push(processTable[processLocation]._Priori[prioriPosition]); //place process in the ready queue
			//processTable[processLocation].setTimer();
				
				
			// check when next is free FUNCTION
			//FIX READY QUEUES
		}
		else // a Core is idle
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
		availableResult = checkComponentAvailability(DI[0]); // check if component is free or busy
		processTable[processLocation].updateStatus(2); // update status to waiting


		if (availableResult == -3) // component is busy
		{
			processTable[processLocation].setTimer(DIAvailability[0]);
			DiskQ.push(processTable[processLocation]._Priori[prioriPosition]);
			
		}
		else //component is free
		{
			executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
			DI[0] = true;
			DIAvailability[0] += executionTime;	//make disk busy and how long
			processTable[processLocation].updateTimer(executionTime); // add onto process timer

		}
	}
	else if (component == "INPUT")
	{
		availableResult = checkComponentAvailability(DI[1]);
		processTable[processLocation].updateStatus(2);
		if (availableResult == -3)
		{
			processTable[processLocation].setTimer(DIAvailability[1]);
			temporaryStep = processTable[processLocation]._Priori[prioriPosition];
			InputQ.push(temporaryStep);
			processTable[processLocation]._Priori[prioriPosition].Command = "DiskQueue";
			
		}
		else
		{
			executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
			DI[1] = true; // make input busy and how long
			DIAvailability[1] += executionTime;
			processTable[processLocation].updateTimer(executionTime);
		}
	}
	else if (component == "ReadyQueue")
	{
		if (ReadyQ.empty() == false)
		{
			temporaryStep = ReadyQ.front();
			ReadyQ.pop();
			//FIX THIS LATER
		}

	}
	else if (component == "DiskQueue")
	{
		if (DiskQ.empty() == false)
		{
			temporaryStep = DiskQ.front();
			DiskQ.pop();

			processTable[processLocation].updateStatus(2); // update status to waiting
			executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
			DI[0] = true;
			DIAvailability[0] += executionTime;	//make disk busy and how long
			processTable[processLocation].updateTimer(executionTime); // add onto process timer
			processTable[processLocation]._Priori[prioriPosition].Command = "DISK";
		}
	}
	else if (component == "InputQueue")
	{
		if (InputQ.empty() == false)
		{
			temporaryStep = InputQ.front();
			InputQ.pop();

			processTable[processLocation].updateStatus(2);
			executionTime = processTable[processLocation]._Priori[prioriPosition].Time;
			DI[1] = true; // make input busy and how long
			DIAvailability[1] += executionTime;
			processTable[processLocation].updateTimer(executionTime);
			processTable[processLocation]._Priori[prioriPosition].Command = "INPUT";
		}
	}
	else 
	{
		cout << "Error: executeProcess function";
	}
	if (processTable[processLocation]._Priori[prioriPosition].Command == "CPU" || processTable[processLocation]._Priori[prioriPosition].Command == "I/O" 
		|| processTable[processLocation]._Priori[prioriPosition].Command == "INPUT")
	{
		processTable[processLocation].updateCurrentPriori(1);
	}

	if (prioriPosition == totalLines) // check that the process hasnt terminated
	{
		processTable[processLocation].updateStatus(0);
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
	bool DandI[2] = { false }; //0: disk 1: input
	int CoresAvailability[4] = { 0 };
	int DandIAvailability[2] = { 0 };
	queue<ProcessStep> ReadyQueue;
	queue<ProcessStep> DiskQueue;
	queue<ProcessStep> InputQueue;

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
		nextProcess = findLowestExecutionTime(processNumber, processTable, Cores, CoresAvailability, DandI, DandIAvailability);
		
		int currentPriori = processTable[nextProcess]._currentPriori;
		if (isProcessTerminated(processTable, nextProcess) == true)
		{ 
			printReport(nextProcess, processNumber, processTable, Cores, CoresAvailability, DandI, DandIAvailability,ReadyQueue, DiskQueue, InputQueue);
		}
		else
		{
			//setComponentIdle(Cores, CoresAvailability, DandI, DandIAvailability, processTable[nextProcess]._Timer, processTable[nextProcess]._Priori[currentPriori].Command);
			executeProcess(nextProcess, processTable, processNumber, Cores, CoresAvailability,DandI, DandIAvailability, ReadyQueue, DiskQueue, InputQueue);

			/*if (processTable[nextProcess]._Priori[currentPriori].Command == "I/O" &&
			processTable[nextProcess]._Priori[currentPriori].Time == 0)// go to the next process
			{
			processTable[nextProcess].updateCurrentPriori(1);
			}*/
		}
	}


	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}
