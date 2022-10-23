#include <unistd.h> 
#include <iostream>
#include <future>

using namespace std;

void sleep_for(int x)
    {
    sleep(x);
    }

int main()
    {
    cout << "Start" << endl;

    // sleep(10);
    auto result = std::async(std::launch::async, sleep, 10);
    result.get();

    cout << "End" << endl;

    return 0;
    }