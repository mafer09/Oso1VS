#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct ProcessStep
{
	string Command;
	int Time;
};
class Process
{
public:
	vector<ProcessStep> _Priori;
	int _Timer;
	int _Name;

	Process();
	void addProcessStep(ProcessStep data);
	void addName(int name);
	void updateTimer(int time);
	void setTimer(int time);


};
Process::Process()
{
	_Timer = 0;
	_Name = 0;
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
	_Timer = time;
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
	vector<ProcessStep> fileContents= retrieveData();

	//Parses the data
	int processNumber = -1;
	Process allProcesses[10]; //**MAY NEED TO BE CHANGED

	for (int i = 0; i < fileContents.size(); i++)
	{
		// only if its NEW
		if (fileContents[i].Command == "NEW")
		{
			processNumber++;
			allProcesses[processNumber].addName(fileContents[i].Time);
		}
		// only if its START
		else if (fileContents[i].Command == "START")
		{
			allProcesses[processNumber].setTimer(fileContents[i].Time);
		}

		// everyone else
		allProcesses[processNumber].addProcessStep(fileContents[i]);
	}

	//test to print contents
	
	/*for (int i = 0; i< 10; ++i)
	{
		cout << trial[i].Command<<" "<<trial[i].Time << '\n';
	}*/

	system("pause"); //**TAKE SYSTEM PAUSE OUT!
	return 0;
}


