#include "ArraySorter.h"


ArraySorter::ArraySorter(void)
{
}

void ArraySorter::InsertionSort(int* arr, int n)
{
	InsertionSort(arr, n, 0, 1);
}

void ArraySorter::InsertionSort(int* arr, int n, int startIndex, int gap)
{
	// You must implement this function
	for(int i = startIndex; i < n; i++)
	{
		int temp = arr[i];
		int j = i;
		while(j > 0 && temp < arr[j-1])
		{
			arr[j] = arr[j-1];
			j = j-1;
		}
		arr[j] = temp;
	}
}

// Checks to see if the array is in sorted order
bool ArraySorter::IsSorted(int* arr, int n)
{
	for (int i = 1; i < n; i++)
	{
		// If an item is less than the one before it, then we're not sorted
		if (arr[i] < arr[i - 1])
		{
			return false;
		}
	}

	return true;
}

void ArraySorter::MergeSort(int* arr, int n)
{
	MergeSort2(arr, 0, n-1);
}

void ArraySorter::HeapSort(int* arr, int n)
{
	//Put array into heap order
	CreateHeap(arr, n);
}

void ArraySorter::CreateHeap(int* arr, int n)
{
	// Percolate down for each parent node
	int start = (n-1)/2;
	while(start >= 0)
	{
		PercolateDown(arr, start, n-1);
		start--;
	}

	// Remove max from heap (and place at end of array)
	int end = n-1;
	int max = 0;
	for(; end > 0; end--)
	{
		max = DeleteMax(arr, end);
		arr[end] = max;
	}
}

int ArraySorter::DeleteMax(int *arr, int end)
{
	int max = arr[0];
	arr[0] = arr[end];
	PercolateDown(arr, 0, end-1);
	return max;
}

void ArraySorter::PercolateDown(int* arr, int start, int end)
{
	int child;
	int temp = arr[start];

	for(; start*2 <= end; start = child)
	{
		child = start*2;
		if(child != end && arr[child + 1] > arr[child])
		{
			child++;
		}
		if(arr[child] > temp)
		{
			arr[start] = arr[child];
		}
		else
		{
			break;
		}
	}
	arr[start] = temp;
}

void ArraySorter::MergeSort2(int *arr, int i, int j)
{
	if(i < j)
	{
		int k = (i + j)/2;
		MergeSort2(arr, i, k);
		MergeSort2(arr, k+1, j);
		Merge(arr, i, k, j);
	}
}

void ArraySorter::Merge(int *arr, int i, int k, int j)
{
	// Create temporary left and right arrays
	int *tempLeft = new int[k-i+1];
	int *tempRight = new int[j-k];

	// Copy data from arr into temporary arrays
	for(int x = 0; x < (k-i+1); x++)
	{
		tempLeft[x] = arr[x + i]; //Start from beginning of left side of arr (at i)
	}
	for(int y = 0; y < (j-k); y++)
	{
		tempRight[y] = arr[k+1+y]; //Start from one past the middle of arr and move to end (at j)
	}

	// Marge right and left back into arr
	int x = 0, y = 0, z = i;
	while( x < (k-i+1) && y < (j-k))
	{
		if(tempLeft[x] <= tempRight[y])
		{
			arr[z] = tempLeft[x];
			x++;
		}
		else
		{
			arr[z] = tempRight[y];
			y++;
		}
		z++;
	}
	// Copy any leftover values from Right or Left array
	while( x < (k-i+1))
	{
		arr[z] = tempLeft[x];
		x++;
		z++;
	}
	while(y < (j-k))
	{
		arr[z] = tempRight[y];
		y++;
		z++;
	}
}

void ArraySorter::QuickSort(int* arr, int n)
{
	// You must implement this function
	QuickSort2(arr, 0, n-1);
}

void ArraySorter::QuickSort2(int *arr, int left, int right)
{
	if((left + 10) <= right)
	{
		int pivot = Median3(arr, left, right);
		int i = left, j = right-1;
		for(;;)
		{
			while(arr[++i] < pivot) 
			{
				// Nada mucho
			}
			while(pivot < arr[--j]) 
			{
				// Empty
			}
			
			if( i < j )
			{
				swap(arr, i, j);
			}
			else
			{
				break;
			}
		}
		swap(arr, i, right-1);
		QuickSort2(arr, left, i-1);
		QuickSort2(arr, i+1, right);
	}
	else //Run insertion sort
	{
		InsertionSort(arr, right+1, left, 0);
	}
}

int ArraySorter::Median3(int *arr, int left, int right)
{
	int center = (left + right)/2;

	// Order left right and center items
	if(arr[center] < arr[left])
	{
		swap(arr, left, center);
	}
	if(arr[left] > arr[right])
	{
		swap(arr, left, right);
	}
	if(arr[right] < arr[center])
	{
		swap(arr, right, center);
	}
	//Place pivot (center item) at right-1
	swap(arr, center, right-1);
	return arr[right-1];
}

// Swaps two values in array
// i & j are indices in array to be swapped
void ArraySorter::swap(int *arr, int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

// This is implemented for you, but you need to implement the InsertionSort 
// function that it calls.
void ArraySorter::ShellSort(int* arr, int n, int* gapVals, int gapValsCount)
{
	// Do an insertion sort pass for each of the gap values
	int i;
	for (i = 0; i < gapValsCount; i++)
	{
		for (int j = 0; j < gapVals[i]; j++)
		{
			InsertionSort(arr, n, j, gapVals[i]);
		}
	}

	// We always need to finish with a pass using gap=1
	if (1 != gapVals[i - 1])
	{
		InsertionSort(arr, n);
	}
}
