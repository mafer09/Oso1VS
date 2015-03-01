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

	int _Name; //Before
	vector<ProcessStep> _Priori; //Before
	int _currentPriori;
	int _Start; //Before
	int _Timer; //Before and updated
	string _Status; /// 1: Running, 2: Waiting, 0: Terminated 3:Ready /
	int _CPUtime;
	
	// -4: ReadyQueue : -5 DiskQueue : -6 InputQueue
	int _ComponentInUse; 
	bool isCommandComplete;
	int _queueInUse;
	
	Process();
	void addProcessStep(ProcessStep data);
	void addName(int name);
	void updateTimer(int time);
	void setStart(int time);
	void updateCPU(int cpu, int time);
	void setStatus(int status);
	void updateCurrentPriori(int time);
	string getStatus();
	void setCommandCompleteness(bool value);
	void updateComponentInUse(int value);
	void QueueTimeChange(int time);

};
Process::Process()
{
	_Timer = 0;
	_Start = 0;
	_CPUtime = 0;
	_currentPriori = 0;
	_Name = 0;
	_Status = "";
	_ComponentInUse = 0;
	isCommandComplete = true;
}

// Process Functions
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
void Process::setStart(int time)
{
	_Start = time;
	_Timer = time;
}
void Process::updateCPU(int cpu,int time)
{
	_CPUtime += time;
	_ComponentInUse = cpu;
}
void Process::setStatus(int status)
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
void Process::setCommandCompleteness(bool value)
{
	isCommandComplete = value;
}
void Process::updateComponentInUse(int value)
{
	_ComponentInUse = value;
}
void Process::QueueTimeChange(int time)
{
	_Timer = time;
}

//SYSTEM Functions
int checkCoreAvailability( bool Cores[], int CoresAvailability[])
{
	vector<int> freeCores;
	int locationOfFreeCore = -1;
	int lowestExecutionTime = 20000;

	for (int i = 0; i < 4; i++) //place free cores in the freeCoresVector
	{
		if (Cores[i] == false)
		{
			freeCores.push_back(i);
		}
	}
	
	if (freeCores.empty()) // Return if the there are no free cores
	{
		return -4;
	}
	else
	{
		for (int i = 0; i < freeCores.size(); i++) //go though the stored free cores
		{
			if (CoresAvailability[freeCores[i]] < lowestExecutionTime) //return the ones with the lowestexcutiontime
			{
				lowestExecutionTime = CoresAvailability[i];
				locationOfFreeCore = freeCores[i];
			}
		}
		return locationOfFreeCore; //send back the location of the free core in the array
	}
	//**POSSIBLE issue with core assignment
};
int checkComponentAvailability(bool Components[], int location)
{
	if (Components[location] == true)
	{
		return -3;
	}
	else
	{
		return location;
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

void hasProcessTerminated(int totalProcesses, Process processTable[])
{
	for (int i = 0; i <= totalProcesses; i++)
	{
		int currentPriori = processTable[i]._currentPriori;
		int totalLines = processTable[i]._Priori.size();
		totalLines--;
		if (currentPriori == totalLines)
		{
			processTable[i].setStatus(0);
		}
	}
};

void terminatedReplications(int numberOfProcesses, Process processTable[], int terminatedProcess, bool SystemComponents[])
{
	int component = -1;
	for (int i = 0; i <= numberOfProcesses; i++)
	{
		if (processTable[i]._Name != processTable[terminatedProcess]._Name && processTable[i]._Status != "Terminated")
		{
			component = processTable[i]._ComponentInUse;
			SystemComponents[component] = false;  //set component to idle
			//** Fix this if the component is any of the queues
			processTable[i].setCommandCompleteness(true);
			processTable[i].updateCurrentPriori(1);
		}
	}
};

int findNextCommand(int numberOfProcesses, Process processTable[])
{
	int minimumTime = 20000;
	int processPosition = -1;

	for (int i = 0; i <= numberOfProcesses; i++)
	{
		if (processTable[i].isCommandComplete == true && processTable[i]._Status != "Terminated")
		{
			if (processTable[i]._Timer < minimumTime)
			{
				minimumTime = processTable[i]._Timer;
				processPosition = i;
			}
		}
	}
	return processPosition;
};
void executeCommand(int processLocation, Process processTable[], bool systemComponents[], int systemComponentsAvailability[], const queue<ProcessStep>& ReadyQueue, const queue<ProcessStep>& DiskQueue, queue<ProcessStep>& InputQueue)
{
	Process CurrentProcess = processTable[processLocation];
	int LineNumToExecute = CurrentProcess._currentPriori;
	string CommandToExecute = CurrentProcess._Priori[LineNumToExecute].Command;
	int msToExecute = CurrentProcess._Priori[LineNumToExecute].Time;
	int LocationOfComponent = -1;
	ProcessStep temporaryStep;

	if (CommandToExecute == "CPU")
	{
		LocationOfComponent = checkCoreAvailability(systemComponents, systemComponentsAvailability);
		
		if (LocationOfComponent == -4)// all cores are busy
		{
			cout << CommandToExecute << "is trying to go into the REAdyQUEUe"<<endl;
			cout << " this is the processID : "<<CurrentProcess._Name<<endl;
			CurrentProcess.updateComponentInUse(LocationOfComponent);
		}
		else //there is a core free
		{
			CurrentProcess.setStatus(1); //set status to running
			CurrentProcess.updateTimer(msToExecute); //update timer
			CurrentProcess.updateCPU(LocationOfComponent, msToExecute); //update cpu counter
			systemComponents[LocationOfComponent] = true; //update component state
			systemComponentsAvailability[LocationOfComponent] = msToExecute; //update component timer
			CurrentProcess.setCommandCompleteness(false); // set if the command is finilized
		}
	}

	else if (CommandToExecute == "INPUT")
	{
		LocationOfComponent = checkComponentAvailability(systemComponents, 5);
		CurrentProcess.setStatus(2); // set status to waiting

		if (LocationOfComponent == -3) //input is busy
		{
			CurrentProcess.QueueTimeChange(systemComponentsAvailability[5]); //set the timer of the process the same as the time the process will execute
			temporaryStep = CurrentProcess._Priori[LineNumToExecute]; 
			InputQueue.push(temporaryStep);
			//InputQueue.push(temporaryStep); //push prioristep into the queue
			CommandToExecute = "InputQueue"; //change the command to be in inputqueue
			CurrentProcess.setCommandCompleteness(false); //set that command is finilized
			CurrentProcess.updateComponentInUse(-6); // save the component being used
			//CurrentProcess._ComponentInUse = -6; //label the queue its in
		}
		else // input is free
		{
			CurrentProcess.updateTimer(msToExecute); //update timer
			systemComponents[LocationOfComponent] = true; //update component state
			systemComponentsAvailability[LocationOfComponent] = CurrentProcess._Timer; //update component timer
			CurrentProcess.setCommandCompleteness(false); //set if command is finilized
			CurrentProcess.updateComponentInUse(LocationOfComponent); // save the component being used
		}
	}

	processTable[processLocation] = CurrentProcess;

	cout << "processing process #: " << CurrentProcess._Name << endl;
	cout << "doing command: " << CommandToExecute << endl;
	cout << "it will take to execute: " << msToExecute << " ms" << endl;
	cout << "with component " << LocationOfComponent << endl;
	cout << "The process timer is " << CurrentProcess._Timer << endl<<endl;
};
void printReport(Process terminatedProcess, Process processTable[], int totalProcesses, bool SystemComponents[], int SystemComponentsAvailability[])
{
	int coreNumber = 1;
	double totalCPUTime = 0;

	cout << "Process " << terminatedProcess._Name
		<< " terminates at t=" << terminatedProcess._Timer << "\n";
	for (int i = 0; i < 4; i++)
	{
		cout << "Core " << coreNumber << " is " << determineState(SystemComponents[i]) << "\n";
		coreNumber++;
	}
	cout << "Disk is " << determineState(SystemComponents[4]) << "\n";
	cout << "Input is " << determineState(SystemComponents[5]) << "\n";

	//Deal with Core contents later
	//cout << "Ready Queue contains: " << components.determineState(components.ReadyQueue.empty()) << "\n";
	//cout << "Disk Queue contains: " << components.determineState(components.DiskQueue.empty()) << "\n";
	//cout << "Input Queue contains: " << components.determineState(components.InputQueue.empty()) << "\n\n";

	cout << "Process ID|Start Time|CPU Time|Status" << "\n";
	cout << terminatedProcess._Name << "  |  " << terminatedProcess._Start << "  |  " <<
		terminatedProcess._CPUtime << "  |  " << terminatedProcess._Status << endl;

	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Name != terminatedProcess._Name && processTable[i]._Status != "Terminated")
		{
			cout << processTable[i]._Name << "  |  " << processTable[i]._Start << "  |  " <<
				processTable[i]._CPUtime << "  |  " << processTable[i]._Status << endl;
		}
		totalCPUTime += processTable[i]._CPUtime;
	}

	cout << "Average number of BUSY Cores: " << totalCPUTime / terminatedProcess._Timer <<endl<<endl;
};

void completedProcess(int numberOfPRocesses, Process processTable[], bool systemComponents[], int systemComponentAvailability[], const queue<ProcessStep>& ReadyQueue, const queue<ProcessStep>& DiskQueue, queue<ProcessStep>& InputQueue)
{
	int lowestExecutionTime = processTable[0]._Timer;
	int lowestProcessLocation = -1;
	int component = -10;
	int currentPriori = -1;
	vector<int> allExecutionTimes;
	ProcessStep temoraryStep;

	for (int i = 0; i <= numberOfPRocesses; i++)
	{
		allExecutionTimes.push_back(processTable[i]._Timer);

		if (processTable[i]._Timer <= lowestExecutionTime)
			{
				lowestExecutionTime = processTable[i]._Timer;
				lowestProcessLocation = i;
			}
	}

	component = processTable[lowestProcessLocation]._ComponentInUse; //Get the component its was working with
	currentPriori = processTable[lowestProcessLocation]._currentPriori;
	if (component < -1)
	{
		for (int i = 0; i < allExecutionTimes.size(); i++)
		{
			if (allExecutionTimes[i] == lowestExecutionTime)
			{
				int QueueInUse = processTable[lowestProcessLocation]._ComponentInUse;
				
				switch (QueueInUse)
				{
					case -4:
						cout << "trying to access ReadyQueue" << endl;
						break;
					case -5:
						cout << "trying to access DiskQueue" << endl;
						break;
					case -6:
						if (InputQueue.empty() == false)
						{
							temoraryStep = InputQueue.front();
							InputQueue.pop();
							processTable[lowestProcessLocation]._Priori[currentPriori].Command = "INPUT";
							processTable[lowestProcessLocation].setCommandCompleteness(true);
						}
						break;
					default:
						cout << "Error: in completedPRocess queuemess" << endl;
						break;
				}
			}
		}
	}
	else
	{
		systemComponents[component] = false; //set the component to idle
		processTable[lowestProcessLocation].setCommandCompleteness(true); //set the command is complete
		processTable[lowestProcessLocation].updateCurrentPriori(1);

		int totalLines = processTable[lowestProcessLocation]._Priori.size(); //get the totalnumber of executable lines in the process
		totalLines--; 

		if (currentPriori == totalLines) //if the current line is the same value as the total number of executable lines in the process, 
			//that means that the process has terminated and do not update the priori and instead update the process status.
		{
			processTable[lowestProcessLocation].setStatus(0);
			printReport(processTable[lowestProcessLocation], processTable, numberOfPRocesses, systemComponents, systemComponentAvailability);
			cout << "completed process was " << lowestProcessLocation<<endl;
			terminatedReplications(numberOfPRocesses, processTable, lowestProcessLocation, systemComponents);
		}

	}

	cout << "component that was in use " << component << "is now " << systemComponents[component]<<endl;
	cout << "lowest process is " << lowestProcessLocation << "with priori " << processTable[lowestProcessLocation]._currentPriori <<endl;
};


int main()
{
	vector<ProcessStep> _FileContents = retrieveData();
	Process _ProcessTable[10];

	int numberOfProcesses = -1;
	int totalExecutableLines = 0;
	//int nextProcess = -1;
	//int globalTimer = 0;

	//SYSTEM Components
	bool _SystemComponents[6] = { false }; //0: CPU1 |1: CPU2 |2: CPU3 |3: CPU4 ||4: Disk |5:Input 
	int _SystemComponentsAvailability[6] = { 0 };
	//vector<queue<ProcessStep>> InputQueue; //0: ReadyQueue |1:DiskQueue |2:InputQueue
	queue<ProcessStep> ReadyQueue;
	queue<ProcessStep> DiskQueue;
	queue<ProcessStep> InputQueue;

	//InputQueue.push_back(queue<ProcessStep>());
	//InputQueue[0].push
	//Queues.push_back(queue<ProcessStep>());
	//Queues.push_back(queue<ProcessStep>());

	/*bool Cores[4] = { false }; //false = idle || true = busy
	bool DandI[2] = { false }; //0: disk 1: input
	int CoresAvailability[4] = { 0 };
	int DandIAvailability[2] = { 0 };
*/

	///Parses the data == Creates proceess table
	for (int i = 0; i < _FileContents.size(); i++)
	{
		/// only if its NEW
		if (_FileContents[i].Command == "NEW")
		{
			numberOfProcesses++;
			_ProcessTable[numberOfProcesses].addName(_FileContents[i].Time);
		}
		/// only if its START
		else if (_FileContents[i].Command == "START")
		{
			_ProcessTable[numberOfProcesses].setStart(_FileContents[i].Time);
		}
		/// everyone else
		else
		{
			_ProcessTable[numberOfProcesses].addProcessStep(_FileContents[i]);
			totalExecutableLines++;
		}
	}

	int minimumTime = 20000; //to make sure it gets process with the smallest amount of time
	int firstProcessLocation = -1;
	for (int i = 0; i <= numberOfProcesses; i++) 
	{
		if (_ProcessTable[i]._Start < minimumTime)
		{
			minimumTime = _ProcessTable[i]._Start;
			firstProcessLocation = i;
		}
	}

	executeCommand(firstProcessLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);

	int processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	
	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, InputQueue, DiskQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, InputQueue, DiskQueue);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable);
	//executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	//completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);


	//cout << "up next process # " << processLocation << " command (is = 0 isNOT =1) .." << _ProcessTable[processLocation].isCommandComplete << ".. complete" << endl;

	
	//cout << "incomplete process " << firstProcessLocation << " with " << _ProcessTable[firstProcessLocation].isCommandComplete;

	//int currentTime = minimumTime; //#2
	 //#3
	//currentTime += _ProcessTable[firstProcessLocation]._Timer;

	//cout << firstProcessLocation<<"__"<<currentTime << endl; //TESET

	//_CompleteTracker[firstProcessLocation] = false; //#4
	//_ProcessTable[firstProcessLocation].setCommandCompleteness(_CompleteTracker[firstProcessLocation]);

	//NEW untested
	 //#1
	//if (_ProcessTable[processLocation]._Timer < currentTime)
	//{ 
	//	currentTime = _ProcessTable[processLocation]._Timer; //#2
	//}

	//cout << processLocation << "__" << currentTime << endl; //TESET

	 //#3
	//currentTime += _ProcessTable[processLocation]._Timer;
	
	//cout << processLocation << "__" << currentTime << endl; //TESET

	//updateCompleteness(_ProcessTable, numberOfProcesses, currentTime, _CompleteTracker); //#4
	
	//for (int i = 0; i <= numberOfProcesses; i++) // #4
	//{
	//	_ProcessTable[firstProcessLocation].setCommandCompleteness(_CompleteTracker[i]);
	//}


	

	/*//who starts the simulation
	int minimumTime = 20000;
	int tempPRocess = -1;
	for (int i = 0; i <= processNumber; i++)
	{
		if (processTable[i]._Start < minimumTime)
		{
			minimumTime = processTable[i]._Start;
			tempPRocess = i;
		}
	}
	*/
	/*
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
			}
		}
	}*/


	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}
/*
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
void printReport(int currentProcessPosition, int totalProcesses, Process processTable[], bool Cores[], int CoresAvailability[], bool DI[], int DIAvailability[], queue<ProcessStep> ReadyQ, queue<ProcessStep> DiskQ, queue<ProcessStep> InputQ)
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
void executeProcess(int processLocation, Process processTable[], int totalProcesses, bool Cores[], int CoresAvailability[], bool DI[], int DIAvailability[], queue<ProcessStep> ReadyQ, queue<ProcessStep> DiskQ, queue<ProcessStep> InputQ)
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
*/
