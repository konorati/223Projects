#include "Gradebook.h"



Gradebook::Gradebook(void)
{
	for(int i = 0; i < 1007; i++)
	{
		hTable[i] = NULL;
	}
}

void Gradebook::AddEntry(string& studentName, double grade)
{
	// Implement this function
	// You must add the student and grade to the hash table UNLESS 
	// a student with the specified name already exists in the table

	// Get hash table index for studentName
	int hash = Hash(studentName);

	// Check if student already exists in table
	Student *pCurrent = hTable[hash];
	Student *pPrevious = NULL;
	bool found = false;
	while(pCurrent)
	{
		if(pCurrent->name == studentName)
		{
			found = true;
		}
		pPrevious = pCurrent;
		pCurrent = pCurrent->next;
	}
	if(found)
	{
		return;
	}
	Student *pTemp = new Student;
	pTemp->name = studentName;
	pTemp->grade = grade;
	pTemp->next = NULL;
	if(!pPrevious) // No entries
	{
		hTable[hash] = pTemp;
	}
	else
	{
		pPrevious->next = pTemp;
	}
}

bool Gradebook::GetGrade(std::string& studentName, double& out_gradeIfFound)
{
	// Implement
	// Hash studentName
	int hash = Hash(studentName);

	// Search for student in hash table
	Student* pCurrent = hTable[hash];
	bool found = false;
	while(pCurrent)
	{
		if(pCurrent->name == studentName)
		{
			found = true;
			break;
		}
		pCurrent = pCurrent->next;
	}
	if(found)
	{
		out_gradeIfFound = pCurrent->grade;
	}
	return found;
}

int Gradebook::Hash(string& studentName)
{
	int len = studentName.length();
	int num = 0;
	if(len > 1)
	{
		num = (studentName.c_str()[len-2])<<4;
		num += studentName.c_str()[len-1];
	}
	else
	{
		for(int i = 0; i < len; i++)
		{
			num += studentName.c_str()[i];
		}
	}
	int hash = 7;
	hash += (num<<5) + num;
	hash %= 1007;
	return hash;
}