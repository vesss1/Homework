#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

// function declarations

double getElapsedTime(high_resolution_clock::time_point start,
    high_resolution_clock::time_point end);

void makeSorted(int a[], int n);
void makeReverse(int a[], int n);
void makeRandomPermutation(int a[], int n);
void makeMergeWorst(int a[], int n);
void copyArray(int from[], int to[], int n);
bool checkSorted(int a[], int n);

void insertionSort(int a[], int n);
void quickSort(int a[], int n);
void mergeSort(int a[], int n);
void heapSort(int a[], int n);
void compositeSort(int a[], int n);

typedef void (*SortFunction)(int[], int);

void buildWorstData(string name, int a[], int n)
{
    if(name == "InsertionSort")
    {
        makeReverse(a, n);
    }
    else if(name == "MergeSort")
    {
        makeMergeWorst(a, n);
    }
    else
    {
        makeRandomPermutation(a, n);
    }
}

double testOneTime(SortFunction sortFunction, int data[], int n)
{
    int* a = new int[n];
    copyArray(data, a, n);

    auto start = high_resolution_clock::now();
    sortFunction(a, n);
    auto end = high_resolution_clock::now();

    if(!checkSorted(a, n))
    {
        cout << "sort error" << endl;
    }

    delete[] a;

    return getElapsedTime(start, end);
}

double worstTime(string name, SortFunction sortFunction, int n)
{
    int* data = new int[n];
    double answer = 0.0;

    if(name == "QuickSort" || name == "HeapSort")
    {
        for(int i = 0; i < 20; i++)
        {
            makeRandomPermutation(data, n);
            double now = testOneTime(sortFunction, data, n);

            if(now > answer)
            {
                answer = now;
            }
        }
    }
    else
    {
        buildWorstData(name, data, n);
        answer = testOneTime(sortFunction, data, n);
    }

    delete[] data;

    return answer;
}

double averageTime(SortFunction sortFunction, int n)
{
    int* data = new int[n];
    double total = 0.0;
    int times = 20;

    for(int i = 0; i < times; i++)
    {
        makeRandomPermutation(data, n);
        total = total + testOneTime(sortFunction, data, n);
    }

    delete[] data;

    return total / times;
}

void runOneSort(ofstream& fout, string name, SortFunction sortFunction, int n)
{
    double worst = worstTime(name, sortFunction, n);
    double average = averageTime(sortFunction, n);

    cout << name << "\t"
         << n << "\t"
         << worst << "\t"
         << average << endl;

    fout << name << "," << n << ",Worst," << worst << endl;
    fout << name << "," << n << ",Average," << average << endl;
}

int main()
{
    int testSize[] = {500, 1000, 2000, 3000, 4000, 5000};
    int sizeCount = 6;

    ofstream fout("result.csv");
    fout << "Sort,Size,Case,Time" << endl;

    cout << "Sort\tSize\tWorst\tAverage" << endl;

    for(int i = 0; i < sizeCount; i++)
    {
        int n = testSize[i];

        runOneSort(fout, "InsertionSort", insertionSort, n);
        runOneSort(fout, "QuickSort", quickSort, n);
        runOneSort(fout, "MergeSort", mergeSort, n);
        runOneSort(fout, "HeapSort", heapSort, n);
        runOneSort(fout, "CompositeSort", compositeSort, n);

        cout << endl;
    }

    fout.close();

    cout << "result.csv finished" << endl;

    return 0;
}
