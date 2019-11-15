//
// Created by yanhao on 2019/11/9.
//

#ifndef CLIENT_TIMECONTROL_H
#define CLIENT_TIMECONTROL_H

#include <thread>
#include <unistd.h>
#include "LinuxCommand.h"
#include "jsonToMysql.h"
#include "HttpRequest.h"
#include "DockerDeploy.h"

/*
void dockerStart(string url){
    HttpRequest* Http;
    char http_return[4096] = {0};
    char http_msg[4096] = {0};
    string temp = "http://172.24.83.26" + url;
    strcpy(http_msg, "http://172.24.83.26");

    if(Http->HttpGet(http_msg, http_return)){
        cout << http_return << endl;
    }
}
*/

//control the monitor interval
void timerControl(){
    string location;
    cout << "Please input the location of your server: ";
    cin >> location;

    int timeInterval;
    cout << "Please input the time interval of this thread (s): ";
    cin >> timeInterval;

    Json::Value diskStore;
    Json::Value memoryStore;
    Json::Value dockerStore;

    while(1){
        Json::Value root = linux_command(location);
        //jsonToMysql();
	
	/*
        ifstream ifs;
        ifs.open("resource.json");
        Json::Value value;
        Json::Reader jsonReader;
        string jsonOut = "";

        if(jsonReader.parse(ifs, value)){
            jsonOut = value.toStyledString();
        }*/
	if(diskStore == root["disk info"] && memoryStore == root["memory info"] && root.isMember("docker info") && dockerStore == root["docker info"]){
	    continue;
	}
	else if(diskStore == root["disk info"] && memoryStore == root["memory info"] && root.isMember("docker info")){
	    continue;
	}
	else if(root.isMember("docker info")){
	    diskStore = root["disk info"];
	    memoryStore = root["memory info"];
	    dockerStore = root["docker info"];
	    cout << "store info changed (docker not null)" << endl;
	}
	else{
	    diskStore = root["disk info"];
            memoryStore = root["memory info"];
            cout << "store info changed (docker null)" << endl;
	}

	/*
	if(!root.isMember("docker info")){
	    continue;
	}
	else if(dockerStore == root["docker info"]){
	   continue;
	}
	else{
	    dockerStore = root["docker info"];
	}
	*/

	string jsonOut = "";
	jsonOut = root.toStyledString();

        HttpRequest* Http;
        char http_return[4096] = {0};
        char http_msg[4096] = {0};

        strcpy(http_msg, "http://172.24.83.25:8080");

        const char* data = jsonOut.c_str();
        if(Http->HttpPost(http_msg, data, http_return)){
            cout << http_return << endl;
        }

        string response = http_return;
        string sub = "POST request.\n";
        int index = response.find(sub, index);
        index += sub.length();

        string dockerImage = response.substr(index, response.length() - index);

        if(dockerImage != "null"){
//	    dockerDeploy(dockerImage);
	    thread dockerRun(dockerDeploy, dockerImage);
            dockerRun.detach();
        }

        sleep(timeInterval);
    }
}

#endif //CLIENT_TIMECONTROL_H
