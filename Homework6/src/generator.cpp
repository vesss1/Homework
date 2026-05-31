#include <algorithm>
#include <random>

using namespace std;

void makeSorted(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        a[i] = i + 1;
    }
}

void makeReverse(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        a[i] = n - i;
    }
}

void makeRandomPermutation(int a[], int n)
{
    static mt19937 gen(41343133);

    for(int i = 0; i < n; i++)
    {
        a[i] = i + 1;
    }

    for(int i = n - 1; i > 0; i--)
    {
        uniform_int_distribution<int> dis(0, i);
        int j = dis(gen);
        swap(a[i], a[j]);
    }
}

void mergeWorstBuild(int a[], int temp[], int left, int right)
{
    if(right - left <= 1)
    {
        return;
    }

    int k = left;

    for(int i = left; i < right; i = i + 2)
    {
        temp[k] = a[i];
        k++;
    }

    for(int i = left + 1; i < right; i = i + 2)
    {
        temp[k] = a[i];
        k++;
    }

    for(int i = left; i < right; i++)
    {
        a[i] = temp[i];
    }

    int mid = (left + right) / 2;
    mergeWorstBuild(a, temp, left, mid);
    mergeWorstBuild(a, temp, mid, right);
}

void makeMergeWorst(int a[], int n)
{
    makeSorted(a, n);

    int* temp = new int[n];
    mergeWorstBuild(a, temp, 0, n);
    delete[] temp;
}

void copyArray(int from[], int to[], int n)
{
    for(int i = 0; i < n; i++)
    {
        to[i] = from[i];
    }
}

bool checkSorted(int a[], int n)
{
    for(int i = 1; i < n; i++)
    {
        if(a[i - 1] > a[i])
        {
            return false;
        }
    }

    return true;
}
