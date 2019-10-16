#include <iostream>
#include <thread>
#include <unistd.h>
#include "linuxCommand.h"
#include "jsonToMysql.h"
#include "HttpRequest.h"

using namespace std;

//control the monitor interval
void timerControl(){
    int timeInterval;
    cout << "Please input the time interval of this thread (s): ";
    cin >> timeInterval;
    while(1){
        linux_command();
        jsonToMysql();

        ifstream ifs;
        ifs.open("resource.json");
        Json::Value value;
        Json::Reader jsonReader;
        string jsonOut = "";

        if(jsonReader.parse(ifs, value)){
            jsonOut = value.toStyledString();
        }

        HttpRequest* Http;
        char http_return[4096] = {0};
        char http_msg[4096] = {0};

        strcpy(http_msg, "http://127.0.0.1:11111/search");

        const char* data = jsonOut.c_str();
        if(Http->HttpPost(http_msg, data, http_return)){
            cout << http_return << endl;
        }

        sleep(timeInterval);
    }
}

//start a thread
int main(){
    thread t1(timerControl);
    t1.join();

    return 0;
}
