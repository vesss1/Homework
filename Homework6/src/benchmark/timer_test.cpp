#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

double getElapsedTime(high_resolution_clock::time_point start,
                      high_resolution_clock::time_point end);

int main()
{
    auto start = high_resolution_clock::now();

    for(long long i = 0; i < 100000000; i++)
    {
    }

    auto end = high_resolution_clock::now();

    cout << "Elapsed Time = "
         << getElapsedTime(start, end)
         << " seconds" << endl;

    return 0;
}
