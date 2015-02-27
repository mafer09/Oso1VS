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
	ProcessStep priori;
	int _timer;
	//void addProcess(string line);


};

/*void Process::addProcess(ProcessStep )
{

}*/

vector<ProcessStep> retrieveData(string fileName)
{
	vector<ProcessStep> fileContents;
	string temp;
	stringstream _DATA;
	ifstream inputTxt;
	ProcessStep priori;

	inputTxt.open(fileName);

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

		_DATA >> priori.Command;
		_DATA >> priori.Time;

		fileContents.push_back(priori);
	}
	inputTxt.close();

	return fileContents;
};


int main()
{
	string _fileName = "input1.txt.txt";
	vector<ProcessStep> trial = retrieveData(_fileName);



	//parseFileData(_fileData);


	//test to print contents
	
	/*for (int i = 0; i< 10; ++i)
	{
		cout << trial[i].Command<<" "<<trial[i].Time << '\n';
	}*/
	

	system("pause"); //TAKE SYSTEM PAUSE OUT!
	return 0;
}


