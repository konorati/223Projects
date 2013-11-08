#pragma once

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Student{
	string name;
	double grade;
	Student* next;
};

// As usual, add private helper functions in this class if you need to
class Gradebook
{
private:
	Student* hTable[1007];
	int Hash(string& studentName);
public:
	Gradebook(void);
	void AddEntry(std::string& studentName, double grade);
	bool GetGrade(std::string& studentName, double& out_gradeIfFound);
};

