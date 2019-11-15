#include <iostream>
#include <thread>
#include <unistd.h>
#include "TimeControl.h"

using namespace std;

//start a thread
int main(){
    thread t1(timerControl);
    t1.join();
	
    return 0;
}
