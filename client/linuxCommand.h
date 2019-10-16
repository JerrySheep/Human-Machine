//
// Created by yanhao on 2019/10/9.
//

#ifndef CLIENT_LINUXCOMMAND_H
#define CLIENT_LINUXCOMMAND_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "json_include/json.h"
#include <fstream>

using namespace std;

//get numbers from a string
/*
void string_to_double(string src, vector<double>& v) {
    	int size = src.size();
 	for(int i = 1; i < size; ++i){
        	if(((src.at(i - 1) < '0' || src.at(i - 1) > '9') && src.at(i - 1) != ' ' && src.at(i - 1) != '.')
           		&& (src.at(i) >= '0' && src.at(i) <= '9')){
            		src.at(i) = '|';
        	}
    	}

    	for (int i = 0; i < size; i++) {
        	if ((src.at(i) < '0' || src.at(i) > '9') && src.at(i) != '.'){
            		src[i] = ' ';
		}
    	}
    	src += " ";

    	string::size_type pos;
    	while ((pos = src.find(" ")) != string::npos) {
        	if (pos == 0) {
            		src = src.substr(pos + 1, src.length() - pos - 1);
            		continue;
        	} else {
            		string t = src.substr(0, pos);
            		int len = t.length();
            		double res = atof(t.c_str());

            		v.push_back(res);
            		src = src.substr(pos, src.length() - pos);
       		}
    	}
}
*/
void data_to_json(vector<double> cpuSourceAvailable, vector<double> cpuMHz, vector<double> cpuCores,
                  vector<double> memoryTotalInfo, vector<double> memoryAvailableInfo,
                  vector<double> diskTotalInfo, vector<double> diskAvailableInfo,
                  vector<double> ethFirstInfo, vector<double> ethSecondInfo){
    Json::Value root;

    //cpu info to json
    for(int i = 0; i < cpuMHz.size(); ++i){
        Json::Value cpu;

        cpu["id"] = i;
        cpu["cpu source"] = cpuSourceAvailable[i];
        cpu["cpu Mhz"] = cpuMHz[i];
        cpu["cpu Cores"] = cpuCores[i];

        root["cpu info"].append(cpu);
    }

    //memory info to json
    Json::Value memory;

    memory["Total"] = memoryTotalInfo[0];
    memory["Available"] = memoryAvailableInfo[0];
    memory["Used"] = 1 - (memoryAvailableInfo[0] / memoryTotalInfo[0]);

    root["memory info"].append(memory);

    //disk info to json
    double diskTotal = 0.0;
    double diskAvailable = 0.0;
    for(int i = 0; i < diskTotalInfo.size(); ++i){
        diskTotal += diskTotalInfo[i];
        diskAvailable += diskAvailableInfo[i];
    }

    Json::Value disk;

    disk["Total"] = diskTotal;
    disk["Available"] = diskAvailable;
    disk["Used"] = 1 - (diskAvailable / diskTotal);

    root["disk info"].append(disk);

    //network info to json
    for(int i = 0; i < ethFirstInfo.size(); i += 2){
        Json::Value network;

        network["id"] = (i + 1) / 2;
        network["upload rate"] = (ethSecondInfo[i] - ethFirstInfo[i]) / 1024.0;
        network["download rate"] = (ethSecondInfo[i + 1] - ethFirstInfo[i + 1]) / 1024.0;

        root["network info"].append(network);
    }

    Json::StyledWriter writer;
    ofstream os;
    os.open("resource.json");
    os << writer.write(root);
    os.close();
}

void store_result(FILE *fp, vector<double>& store){
    char buf[100];

    if(!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
        return;
    }
    while(memset(buf, 0, sizeof(buf)), fgets(buf, sizeof(buf) - 1, fp) != 0 ) {
        string s = buf;
        store.push_back(atof(s.c_str()));
        //string_to_double(s, store);
    }

    for(int i = 0; i < store.size(); ++i){
        cout << store[i] << " ";
    }
    cout << endl;
}

void exec_command(char* command, vector<double>& store){
    FILE *fp = NULL;
    fp = popen(command, "r");
    store_result(fp, store);
    pclose(fp);
}

void linux_command(){
    vector<double> cpuSourceAvailable;
    vector<double> cpuMHz;
    vector<double> cpuCores;
    vector<double> memoryTotalInfo;
    vector<double> memoryAvailableInfo;
    vector<double> diskTotalInfo;
    vector<double> diskAvailableInfo;
    vector<double> ethFirstInfo;
    vector<double> ethSecondInfo;

    //cpu resource info
    cout << endl << "cpu source available :" << endl;
    exec_command("sar -P ALL 1 1 | grep Average | grep -v all | grep -v CPU |  awk '{print $8}'", cpuSourceAvailable);

    //cpu MHz info
    cout << endl << "cpu MHz :" << endl;
    exec_command("cat /proc/cpuinfo | grep \"cpu MHz\" | awk '{print $4}'", cpuMHz);

    //cpu cores info
    cout << endl << "cpu cores :" << endl;
    exec_command("cat /proc/cpuinfo | grep \"cpu cores\" | awk '{print $4}'", cpuCores);

    //memory total info
    cout << endl << "memory info (total KB):" << endl;
    exec_command("free | grep Mem: | awk '{print $2}'", memoryTotalInfo);

    //memory available info
    cout << endl << "memory info (available KB)" << endl;
    exec_command("free | grep Mem: | awk '{print $7}'", memoryAvailableInfo);

    //disk total info
    cout << endl << "disk info (Total KB):" << endl;
    exec_command("df | grep -v Filesystem | awk '{print $2}'", diskTotalInfo);

    //disk available info
    cout << endl << "disk info (Available KB):" << endl;
    exec_command("df | grep -v Filesystem | awk '{print $4}'", diskAvailableInfo);

    //fist network bandwidth info
    cout << endl << "first network bandwidth info (Bytes):" << endl;
    exec_command("ifconfig | grep \"X packets\" | awk '{print $5}'", ethFirstInfo);

    sleep(1);
    //second network bandwidth info
    cout << endl << "second network bandwidth info (Bytes):" << endl;
    exec_command("ifconfig | grep \"X packets\" | awk '{print $5}'", ethSecondInfo);

    cout << endl << "upload rate : " << (ethSecondInfo[0] - ethFirstInfo[0]) / 1024.00 << " KB/s" << endl;
    cout << endl << "download rate : " << (ethSecondInfo[1] - ethFirstInfo[1]) / 1024.00 << " KB/s" << endl;


    data_to_json(cpuSourceAvailable, cpuMHz, cpuCores, memoryTotalInfo, memoryAvailableInfo, diskTotalInfo, diskAvailableInfo, ethFirstInfo, ethSecondInfo);
}

#endif //CLIENT_LINUXCOMMAND_H
