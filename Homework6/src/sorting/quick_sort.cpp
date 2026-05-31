#include <algorithm>

using namespace std;

int medianOfThree(int a[], int left, int right)
{
    int mid = (left + right) / 2;

    if(a[mid] < a[left])
    {
        swap(a[left], a[mid]);
    }

    if(a[right] < a[left])
    {
        swap(a[left], a[right]);
    }

    if(a[right] < a[mid])
    {
        swap(a[mid], a[right]);
    }

    swap(a[mid], a[right - 1]);
    return a[right - 1];
}

void smallInsertionSort(int a[], int left, int right)
{
    for(int i = left + 1; i <= right; i++)
    {
        int temp = a[i];
        int j = i;

        while(j > left && temp < a[j - 1])
        {
            a[j] = a[j - 1];
            j--;
        }

        a[j] = temp;
    }
}

void quickSortRange(int a[], int left, int right)
{
    if(left + 10 <= right)
    {
        int pivot = medianOfThree(a, left, right);
        int i = left;
        int j = right - 1;

        while(true)
        {
            while(a[++i] < pivot)
            {
            }

            while(a[--j] > pivot)
            {
            }

            if(i < j)
            {
                swap(a[i], a[j]);
            }
            else
            {
                break;
            }
        }

        swap(a[i], a[right - 1]);

        quickSortRange(a, left, i - 1);
        quickSortRange(a, i + 1, right);
    }
    else
    {
        smallInsertionSort(a, left, right);
    }
}

void quickSort(int a[], int n)
{
    if(n > 1)
    {
        quickSortRange(a, 0, n - 1);
    }
}
