// Cpt S 223 Homework 5 - MyString class
// (put your name here)

#include "MyString.h"

using namespace std;

MyString::MyString()
{
    // Make an empty string. Note that this is different from a NULL string.
	m_str = new char[1];
	m_str[0] = 0;
}

// Copy Constructor
MyString::MyString(const MyString &rhs)
{
	int length = rhs.Length();
	m_str = new char[length+1];
	int i = 0;
	while(i <= length)
	{
		m_str[i] = rhs.m_str[i];
		i++;
	}
}

MyString::MyString(const char* str)
{
    // Special case for a null string
	if (!str)
	{
		m_str = new char[1];
		m_str[0] = 0;
		return;
	}
	
	int len = Length(str);    
    m_str = new char[len + 1];
    Copy(m_str, str, len + 1);
}

// This is a private constructor that assumes both parameters are valid, 
// thus there isn't any error checking done on these parameters.
MyString::MyString(const char* str, int charCount)
{
	m_str = new char[charCount + 1];
	Copy(m_str, str, charCount);
	m_str[charCount] = 0;
}

MyString::~MyString()
{
    if (m_str)
	{
		delete [] m_str;
		m_str = NULL;
	}
#if defined(DEBUG) || defined(_DEBUG)
	else
	{
		std::cout << "DEBUG: WARNING, destructor called when m_str is NULL!" << std::endl;
	}
#endif
}

// Compares the desired number of characters in two strings and returns true if
// they are all equal. If the null-terminator on either string is encountered
// before the desired number of characters have been compared, then false
// will be returned.
bool MyString::AreEqual(const char* str1, const char* str2, int numCharsToCompare)
{
    while (numCharsToCompare--)
    {
        if (!str1[0])
        {
            return (0 == str2[0] && 0 == numCharsToCompare);
        }
        
        if (str1[0] != str2[0])
        {
            return false;
        }
        
        str1++;
        str2++;
    }
    
    // Coming here means we compared the desired number of characters
    // and didn't encounter a mismatch, so we return true.
    return true;
}

// Copies the specified number of characters from the source to destination. 
// No null terminators are used for anything. The specified number of characters 
// will be copied exactly and this could potentially go past a null terminator.
void MyString::Copy(char* destination, const char* source, int count)
{
	while (count--)
	{
		*destination++ = *source++;
	}
}

void MyString::countLetters(int *arr, int length) const
{
	for(int i = 0; i < length; i++)
	{
		if(islower(m_str[i]))
		{
			arr[m_str[i]-'a']++;
		}
		else if(isupper(m_str[i]))
		{
			arr[m_str[i]-'A']++;
		}
	}
}

int MyString::GetAnagrams(vector<MyString>& candidates, vector<MyString>& output)
{
    // You must implement this function
	// Return the number of strings that you add to the output vector
	int length = Length();
	int numAnagrams = 0;
	int count[26];
	int arr[26];
	for(int i = 0; i < 26; i++) //Set every index of count to 0
	{
		count[i] = 0;
	}
	//Create array of each letter count from original string
	countLetters(count, length);

	vector<MyString>::const_iterator iter;
	for(iter = candidates.begin(); iter != candidates.end(); iter++)
	{
		if(iter->Length() == length)
		{
			int i = 0;
			for(i = 0; i < 26; i++)
			{
				arr[i] = 0;
			}
			iter->countLetters(arr, length);
			bool match = true;
			int j = 0;
			while(match && (j < 26))
			{
				if(count[j] != arr[j])
				{
					match = false;
				}
				j++;
			}
			if(match)
			{
				output.push_back(*iter);
				numAnagrams++;
			}
		}
	}
	return numAnagrams;
}

int MyString::IndexOf(char c, int startIndex) const
{
    // Implement this function
	// Start the search for the character at the specified index
	// Return the index of the character if it is found, -1 if it isn't
	int index = -1;
	while(index == -1 && m_str[startIndex])
	{
		if(c == m_str[startIndex])
		{
			index = startIndex;
		}
		startIndex++;
	}
	return index;
}

int MyString::IndexOf(const char* str, int startIndex) const
{
	// Implement this function
	// Start the search for the string at the specified index
	// Return the starting index of the string if it is found, -1 if it isn't
	int index = 0, length = 0, size = 0;
	length = Length();
	bool found = false;
	const char *temp = str;
	while(*temp++)
	{
		size++;
	}
	while(!found && index <= length-size)
	{
		if(AreEqual((m_str+index), str, size))
		{
			found = true;	
		}
		if(!found)
		{
			index++;
		}
	}
	if(!found)
	{
		index = -1;
	}
	return index;
}

int MyString::Length() const
{
    return Length(m_str);
}

int MyString::Length(const char* str)
{
    int len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
}

// Sets the string to a new string value. This is the kind of function 
// that only a mutable string class would have.
void MyString::Set(const char* str)
{
	// First delete the old string
	if (m_str)
	{
		delete [] m_str;
	}

	if (str)
	{
		int len = Length(str);    
		m_str = new char[len + 1];
		char* dst = m_str;
		while (*dst++ = *str++);
	}
	else
	{
		m_str = new char[1];
		m_str[0] = 0;
	}
}

void MyString::Split(char c, std::vector<MyString>& outputVector) const
{
	// You must implement the split function
	// Examples:
	// 1. If the string is "This is a string" then Split(' ') adds 4 strings to 
	//    the output vector:
	//      "This"
	//      "is"
	//      "a"
	//      "string"
	// 2. If the string is "Cpt S 223 is a prerequisite for 322" then Split('3') 
	//    adds 3 strings to the output vector:
	//      "Cpt S 22"
	//      " is a prerequisite for "
	//      "22"

	int start = 0, end = 0;
	int length = Length();
	while (start < length)
	{
		while(m_str[end] != c && m_str[end])
		{
			end++;
		}

		MyString strTemp;
		Substring(start, end-start, strTemp);
		outputVector.push_back(strTemp);
		start = end+1;
		while(m_str[start] == c)
		{
			start++;
		}
		end = start;
	}
}

// Gets a substring from this string and stores it in the output string. If 
// the substring cannot be parsed, then false is returned and the output 
// string is not modified.
bool MyString::Substring(int startIndex, int length, MyString& output) const
{
	// You must implement this function
	if((startIndex + length) > Length())
	{
		return false;
	}
	else
	{
		int i = 0;
		char *temp = new char[length+1];
		for(i = 0; i < length; i++)
		{
			temp[i] = m_str[startIndex++]; 
		}
		temp[i] = '\0';
		output.m_str = temp;
	}
	return true;
}

void MyString::Write(std::ostream& stream)
{
	stream << m_str;
}