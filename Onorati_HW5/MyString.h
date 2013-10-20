// Cpt S 223 Homework 5 - MyString class
// (put your name here)

#ifndef MYSTRINGH
#define MYSTRINGH

#include <iostream>
#include <vector>
// Do NOT include <string> or <string.h> in this class. You must implement all the 
// string functionality without the use of the STL string or any string library 
// functions.

class MyString
{
private:
    char* m_str;

	MyString(const char* str, int charCount);
    
    static bool AreEqual(const char* str1, const char* str2, int numCharsToCompare);

	// Copies the specified number of characters from the source to destination
	static void Copy(char* destination, const char* source, int count);
    
    // Utility function to compute the length of a string. The null-terminating 
	// character is NOT counted.
    static int Length(const char* str);
    
public:
    MyString();
	// You must add a copy constructor
	MyString(const MyString &rhs);
	MyString(const char* str);
	~MyString();
    
	void countLetters(int *arr, int length) const;
    int GetAnagrams(std::vector<MyString>& candidates, std::vector<MyString>& output);
	int IndexOf(char c, int startIndex = 0) const;
	int IndexOf(const char* str, int startIndex = 0) const;
    int Length() const;
	void Set(const char* str);
	void Split(char c, std::vector<MyString>& outputVector) const;
	bool Substring(int startIndex, int length, MyString& output) const;
	void Write(std::ostream& stream);
};
#endif