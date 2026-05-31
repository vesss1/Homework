#include <algorithm>

using namespace std;

void mergePart(int a[], int temp[], int left, int mid, int right)
{
    int i = left;
    int j = mid;
    int k = left;

    while(i < mid && j < right)
    {
        if(a[i] <= a[j])
        {
            temp[k] = a[i];
            i++;
        }
        else
        {
            temp[k] = a[j];
            j++;
        }

        k++;
    }

    while(i < mid)
    {
        temp[k] = a[i];
        i++;
        k++;
    }

    while(j < right)
    {
        temp[k] = a[j];
        j++;
        k++;
    }
}

void mergeSort(int a[], int n)
{
    if(n <= 1)
    {
        return;
    }

    int* temp = new int[n];

    for(int size = 1; size < n; size = size * 2)
    {
        for(int left = 0; left < n; left = left + size * 2)
        {
            int mid = min(left + size, n);
            int right = min(left + size * 2, n);
            mergePart(a, temp, left, mid, right);
        }

        for(int i = 0; i < n; i++)
        {
            a[i] = temp[i];
        }
    }

    delete[] temp;
}
