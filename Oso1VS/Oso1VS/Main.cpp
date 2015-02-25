#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct processStep
{
	string Command;
	int Time;
};

void retrieveData(string fileName);
void parseFileData(vector<string> fileData);

void main()
{
	string _fileName = "input1.txt.txt";
	retrieveData(_fileName);

	//parseFileData(_fileData);


	//test to print contents
	/*
	for (vector<string>::const_iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		cout << *i << '\n';
	}
	*/

	system("pause");
}
void parseFileData(vector<string> fileData)
{
	int counter = 0;
	if (find(fileData.begin(), fileData.end(), "NEW") != fileData.end())
	{
		counter++;
	}
	
	cout << counter;
}
void retrieveData(string fileName)
{
	//vector<string> fileContents;
	string temp;
	stringstream _DATA;
	ifstream inputTxt;
	processStep *priori = new processStep[3];

	inputTxt.open(fileName);

	if (!inputTxt.is_open())
	{
		cout<<"Failed to Open File";
	}

	//fileContents.begin();

	

	//Getting contents from the file stored into the vector
	while (!inputTxt.eof())
	{
		getline(inputTxt, temp);
		_DATA.str("");
		_DATA.clear();

		_DATA << temp;
		string command;
		_DATA >> command;
		int time;
		_DATA >> time;
		
		//this is to test _DATA
		//cout << command << time<< "\n";
	}
	inputTxt.close();
	//fileContents.end();

}

/*class Process
{
public:
	processStep priori;
	int _timer;

	void addProcess(string line)
	{

	}
};*/
