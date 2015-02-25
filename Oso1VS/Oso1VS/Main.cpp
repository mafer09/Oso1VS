#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> retrieveData(string fileName);
void parseFileData(vector<string> fileData);

void main()
{
	string _fileName = "input1.txt.txt";
	vector<string> _fileData = retrieveData(_fileName);

	parseFileData(_fileData);


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
vector<string> retrieveData(string fileName)
{
	vector<string> fileContents;
	string temp;
	ifstream inputTxt;
	inputTxt.open(fileName);

	if (!inputTxt.is_open())
	{
		cout<<"Failed to Open File";
	}

	fileContents.begin();

	//Getting contents from the file stored into the vector
	while (!inputTxt.eof())
	{
		getline(inputTxt, temp);
		fileContents.push_back(temp);
	}
	inputTxt.close();
	fileContents.end();

	return fileContents;
}

class Process
{
private:
	vector<string> priori;
public:
	int _timer;
	void addProcess(string line)
	{
		priori.begin();
		priori.push_back(line);
		priori.end();
	}
};