#include <chrono>

using namespace std::chrono;

double getElapsedTime(high_resolution_clock::time_point start,
                      high_resolution_clock::time_point end)
{
    duration<double> elapsed = end - start;
    return elapsed.count();
}
