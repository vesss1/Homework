#include <algorithm>

using namespace std;

void percDown(int a[], int i, int n)
{
    int child;
    int temp = a[i];

    while(i * 2 + 1 < n)
    {
        child = i * 2 + 1;

        if(child != n - 1 && a[child] < a[child + 1])
        {
            child++;
        }

        if(temp < a[child])
        {
            a[i] = a[child];
        }
        else
        {
            break;
        }

        i = child;
    }

    a[i] = temp;
}

void heapSort(int a[], int n)
{
    for(int i = n / 2 - 1; i >= 0; i--)
    {
        percDown(a, i, n);
    }

    for(int j = n - 1; j > 0; j--)
    {
        swap(a[0], a[j]);
        percDown(a, 0, j);
    }
}
