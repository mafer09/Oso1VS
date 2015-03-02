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
	int _ComponentInUse; // -4: ReadyQueue : -5 DiskQueue : -6 InputQueue
	bool isCommandComplete;
	
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

int findProcessInQueue(int queueValue, int totalProcesses, Process processTable[])
{
	for (int i = 0; i <= totalProcesses; i++)
	{
		int processPlacement = processTable[i]._ComponentInUse;
		if (queueValue == processPlacement)
		{
			return processTable[processPlacement]._Name;
		}
	}
	return 204; //204 is an error
}

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
			if (component < -1)
			{
				cout << "Error in terminatedReplications";
			}
			else
			{
				SystemComponents[component] = false;  //set component to idle
			}
			
			//** Fix this if the component is any of the queues
			processTable[i].setCommandCompleteness(true);
			processTable[i].updateCurrentPriori(1);
		}
	}
};



int findNextCommand(int numberOfProcesses, Process processTable[], int processesLeft[])
{
	int minimumTime = 20000;
	int processPosition = -1;
	int processesLeftCounter = 0;
	int lastProcess = -1;

	for (int i = 0; i < 10; i++)
	{
		if (processesLeft[i] != 99)
		{
			processesLeftCounter++;
			lastProcess = processesLeft[i];
		}
	}

	if (processesLeftCounter == 1)
	{
		processPosition = lastProcess;
	}
	else
	{
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
	}
	return processPosition;
};
void executeCommand(int processLocation, Process processTable[], bool systemComponents[], int systemComponentsAvailability[], queue<ProcessStep>& ReadyQueue, queue<ProcessStep>& DiskQueue, queue<ProcessStep>& InputQueue)
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
			InputQueue.push(temporaryStep);//push prioristep into the queue 
			CommandToExecute = "InputQueue"; //change the command to be in inputqueue
			CurrentProcess.setCommandCompleteness(false); //set that command is finilized
			CurrentProcess.updateComponentInUse(-6); // save the component being used
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
	else if (CommandToExecute == "I/O")
	{
		LocationOfComponent = checkComponentAvailability(systemComponents, 4);
		CurrentProcess.setStatus(2); // set status to waiting

		if (LocationOfComponent == -3) //Disk is busy
		{
			CurrentProcess.QueueTimeChange(systemComponentsAvailability[4]); 
			temporaryStep = CurrentProcess._Priori[LineNumToExecute];
			DiskQueue.push(temporaryStep);
			CommandToExecute = "DiskQueue";
			CurrentProcess.setCommandCompleteness(false);
			CurrentProcess.updateComponentInUse(-5);

		}
		else //disk is free
		{
			CurrentProcess.updateTimer(msToExecute); //update timer
			systemComponents[LocationOfComponent] = true; //update component state
			systemComponentsAvailability[LocationOfComponent] = CurrentProcess._Timer;
			CurrentProcess.setCommandCompleteness(false); //set that command isnt finilized
			CurrentProcess.updateComponentInUse(LocationOfComponent); //save the component being used
		}
	}

	processTable[processLocation] = CurrentProcess;

	cout << "processing process #: " << CurrentProcess._Name << endl;
	cout << "doing command: " << CommandToExecute << endl;
	cout << "it will take to execute: " << msToExecute << " ms" << endl;
	cout << "with component " << LocationOfComponent << endl;
	cout << "The process timer is " << CurrentProcess._Timer << endl<<endl;
};
void printReport(Process terminatedProcess, Process processTable[], int totalProcesses, bool SystemComponents[], int SystemComponentsAvailability[], queue<ProcessStep>& ReadyQueue, queue<ProcessStep>& DiskQueue, queue<ProcessStep>& InputQueue)
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

	if (ReadyQueue.empty() == true)
	{
		cout << "Ready queue is Empty" << endl;
	}
	else
	{
		cout << "Ready queue contains: "<<findProcessInQueue(-4, totalProcesses, processTable) <<endl;

	}
	if (DiskQueue.empty() == true)
	{
		cout << "Disk queue is Empty" << endl;
	}
	else
	{
		cout << "Disk queue contains: " << findProcessInQueue(-5, totalProcesses, processTable) << endl;
	}
	if (InputQueue.empty() == true)
	{
		cout << "Input queue is Empty" << endl;
	}
	else
	{
		cout << "Input queue contains: " << findProcessInQueue(-6, totalProcesses, processTable) << endl;
	}

	cout << "Process ID|Start Time|CPU Time|Status" << "\n";
	cout << terminatedProcess._Name << "        |  " << terminatedProcess._Start << "  |  " <<
		terminatedProcess._CPUtime << "  |  " << terminatedProcess._Status << endl;

	for (int i = 0; i <= totalProcesses; i++)
	{
		if (processTable[i]._Name != terminatedProcess._Name && processTable[i]._Status != "Terminated")
		{
			cout << processTable[i]._Name << "        |  " << processTable[i]._Start << "  |  " <<
				processTable[i]._CPUtime << "  |  " << processTable[i]._Status << endl;
		}
		totalCPUTime += processTable[i]._CPUtime;
	}

	cout << "Average number of BUSY Cores: " << totalCPUTime / terminatedProcess._Timer <<endl<<endl;
};

void completedProcess(int numberOfPRocesses, Process processTable[], bool systemComponents[], int systemComponentAvailability[], queue<ProcessStep>& ReadyQueue, queue<ProcessStep>& DiskQueue, queue<ProcessStep>& InputQueue, int processesLeft[])
{
	int lowestExecutionTime = processTable[0]._Timer;
	int lowestProcessLocation = -1;
	int component = -10;
	int currentPriori = -1;
	vector<int> allExecutionTimes;
	ProcessStep temoraryStep;

	int processesLeftCounter = 0;
	int lastProcess = -1;

	for (int i = 0; i < 10; i++)
	{
		if (processesLeft[i] != 99)
		{
			processesLeftCounter++;
			lastProcess = processesLeft[i];
		}
	}
	if (processesLeftCounter == 1)
	{
		lowestProcessLocation = lastProcess;
	}
	else
	{
		for (int i = 0; i <= numberOfPRocesses; i++)
		{
			allExecutionTimes.push_back(processTable[i]._Timer);

			if (processTable[i]._Timer <= lowestExecutionTime)
			{
				lowestExecutionTime = processTable[i]._Timer;
				lowestProcessLocation = i;
			}
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
						if (DiskQueue.empty() == false)
						{
							temoraryStep = DiskQueue.front();
							DiskQueue.pop();
							processTable[lowestProcessLocation]._Priori[currentPriori].Command = "I/O";
							processTable[lowestProcessLocation].setCommandCompleteness(true);
						}
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
			printReport(processTable[lowestProcessLocation], processTable, numberOfPRocesses, systemComponents, systemComponentAvailability,ReadyQueue,DiskQueue,InputQueue);
			//cout << "completed process was " << lowestProcessLocation<<endl;
			processesLeft[lowestProcessLocation] = 99;
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

	//SYSTEM Components
	bool _SystemComponents[6] = { false }; //0: CPU1 |1: CPU2 |2: CPU3 |3: CPU4 ||4: Disk |5:Input 
	int _SystemComponentsAvailability[6] = { 0 };
	queue<ProcessStep> ReadyQueue;
	queue<ProcessStep> DiskQueue;
	queue<ProcessStep> InputQueue;

	int _ProcessesLeft[10];

	for (int i = 0; i < 10; i++)
	{
		_ProcessesLeft[i] = 99;
	}

	///Parses the data == Creates proceess table
	for (int i = 0; i < _FileContents.size(); i++)
	{
		/// only if its NEW
		if (_FileContents[i].Command == "NEW")
		{
			numberOfProcesses++;
			_ProcessTable[numberOfProcesses].addName(_FileContents[i].Time);
			_ProcessesLeft[numberOfProcesses] = numberOfProcesses;
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

	for (int i = 0; i < totalExecutableLines; i++)
	{
		int processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
		executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
		completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);
	}

	/*int processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue,_ProcessesLeft);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);
	
	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, InputQueue, DiskQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, InputQueue, DiskQueue, _ProcessesLeft);
	//Process 1 Terminates

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);
	//cpu is executed

	
	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);
	// i/o is executed

	processLocation = findNextCommand(numberOfProcesses, _ProcessTable, _ProcessesLeft);
	executeCommand(processLocation, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue);
	completedProcess(numberOfProcesses, _ProcessTable, _SystemComponents, _SystemComponentsAvailability, ReadyQueue, DiskQueue, InputQueue, _ProcessesLeft);
	*/
	//cout << "up next process # " << processLocation << " command (is = 0 isNOT =1) .." << _ProcessTable[processLocation].isCommandComplete << ".. complete" << endl;

	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}

