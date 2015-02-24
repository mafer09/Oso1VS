#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> retrieveData(string fileName);

void main()
{
	string fileName = "input1.txt.txt";
	vector<string> fileData = retrieveData(fileName);




	//test to print contents
	for (vector<string>::const_iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		cout << *i << '\n';
	}
	

	system("pause");
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