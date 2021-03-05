#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}