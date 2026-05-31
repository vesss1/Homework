void insertionSort(int a[], int n);
void mergeSort(int a[], int n);

void compositeSort(int a[], int n)
{
    if(n <= 30)
    {
        insertionSort(a, n);
    }
    else
    {
        mergeSort(a, n);
    }
}
