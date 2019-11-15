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
#include <string>

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
Json::Value data_to_json(vector<double> cpuSourceAvailable, vector<double> cpuMHz, vector<double> cpuCores,
                  vector<double> memoryTotalInfo, vector<double> memoryAvailableInfo,
                  vector<double> diskTotalInfo, vector<double> diskAvailableInfo,
                  vector<double> ethFirstInfo, vector<double> ethSecondInfo,
                  vector<string> dockerIDInfo, vector<double> dockerCpuInfo,
                  vector<double> dockerMemoryUsageInfo, vector<double> dockerMemoryLimitInfo,
                  vector<double> dockerMemoryInfo, vector<double> macAddressId,
                  vector<string> locationInfo, vector<string> timestampInfo){
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
    cout << "dockerIDInfo size: " << dockerIDInfo.size() << endl;
    //docker info to json
    for(int i = 0; i < dockerIDInfo.size(); ++i){
	cout << "???wtf" << endl;
        Json::Value docker;

        docker["id"] = dockerIDInfo[i];
        docker["cpu"] = dockerCpuInfo[i];
        docker["memory usage"] = dockerMemoryUsageInfo[i];
        docker["memory limit"] = dockerMemoryLimitInfo[i];
        docker["memory"] = dockerMemoryInfo[i];
//        docker["network input"] = dockerNetworkInputInfo[i];
//        docker["network output"] = dockerNetworkOutputInfo[i];

        root["docker info"].append(docker);
    }

    //memory info to json
    Json::Value memory;
    memory["total"] = memoryTotalInfo[0];
    memory["available"] = memoryAvailableInfo[0];
    memory["used"] = 1 - (memoryAvailableInfo[0] / memoryTotalInfo[0]);
    root["memory info"].append(memory);

    //disk info to json
    double diskTotal = 0.0;
    double diskAvailable = 0.0;
    for(int i = 0; i < diskTotalInfo.size(); ++i){
        diskTotal += diskTotalInfo[i];
        diskAvailable += diskAvailableInfo[i];
    }

    //disk info to json
    Json::Value disk;
    disk["total"] = diskTotal;
    disk["available"] = diskAvailable;
    disk["used"] = 1 - (diskAvailable / diskTotal);
    root["disk info"].append(disk);

    //network info to json
    for(int i = 0; i < ethFirstInfo.size(); i += 2){
        Json::Value network;

        network["id"] = (i + 1) / 2;
        network["upload rate"] = (ethSecondInfo[i] - ethFirstInfo[i]) / 1024.0;
        network["download rate"] = (ethSecondInfo[i + 1] - ethFirstInfo[i + 1]) / 1024.0;

        root["network info"].append(network);
    }

    //mac address info to json
    Json::Value macAddress;
    macAddress["id"] = macAddressId[0];
    root["mac address info"].append(macAddress);

    //location info to json
    Json::Value location;
    location["address"] = locationInfo[0];
    root["location info"].append(location);

    //
    Json::Value timestamp;
    timestamp["time"] = timestampInfo[0];
    root["timestamp info"].append(timestamp);
	
    return root;
    /*Json::StyledWriter writer;
    ofstream os;
    os.open("resource.json");
    os << writer.write(root);
    os.close();*/
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

void string_exec_command(char* command, vector<string>& store){
    FILE *fp = NULL;
    fp = popen(command, "r");

    char buf[1000];

    if(!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
        return;
    }

    while(memset(buf, 0, sizeof(buf)), fgets(buf, sizeof(buf) - 1, fp) != 0 ) {
        string s = buf;
        store.push_back(s);
    }

    pclose(fp);
}

vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str)
        return res;
    char * strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}

double stringToHashCode(string s){
    double seed = 7;
    double h = 0;
    if (h == 0 && s.length() > 0) {
        for (int i = 0; i < s.length(); i++) {
            h = seed * h + s[i];
        }
    }
    return h;
}

Json::Value linux_command(string location){
    vector<double> cpuSourceAvailable;
    vector<double> cpuMHz;
    vector<double> cpuCores;
    vector<double> memoryTotalInfo;
    vector<double> memoryAvailableInfo;
    vector<double> diskTotalInfo;
    vector<double> diskAvailableInfo;
    vector<double> ethFirstInfo;
    vector<double> ethSecondInfo;
    vector<string> dockerIDInfo;
    vector<double> dockerCpuInfo;
    vector<double> dockerMemoryUsageInfo;
    vector<double> dockerMemoryLimitInfo;
    vector<double> dockerMemoryInfo;
//    vector<double> dockerNetworkInputInfo;
//    vector<double> dockerNetworkOutputInfo;
    vector<string> dockerInfo;
    vector<string> macAddress;
    vector<double> macAddressId;
    vector<string> locationInfo;
    vector<string> timestampInfo;

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

//    cout << endl << "upload rate : " << (ethSecondInfo[0] - ethFirstInfo[0]) / 1024.00 << " KB/s" << endl;
//    cout << endl << "download rate : " << (ethSecondInfo[1] - ethFirstInfo[1]) / 1024.00 << " KB/s" << endl;

    //docker info
    cout << endl << "docker info :" << endl;
    string_exec_command("docker stats --no-stream | grep -v CPU", dockerInfo);

    for(int i = 0; i < dockerInfo.size(); ++i){
        vector<string> s = split(dockerInfo[i], " ");
        dockerIDInfo.push_back(s[0]);
        dockerCpuInfo.push_back(atof(s[2].c_str()));
        dockerMemoryUsageInfo.push_back(atof(s[3].c_str()));
        if(s[3][s[3].length() - 3] == 'M')
            dockerMemoryUsageInfo[dockerMemoryUsageInfo.size() - 1] *= 1024;
        else if(s[3][s[3].length() - 3] == 'G')
            dockerMemoryUsageInfo[dockerMemoryUsageInfo.size() - 1] *= 1024 * 1024;

        dockerMemoryLimitInfo.push_back(atof(s[5].c_str()));
        if(s[5][s[5].length() - 3] == 'M')
            dockerMemoryLimitInfo[dockerMemoryLimitInfo.size() - 1] *= 1024;
        else if(s[5][s[5].length() - 3] == 'G')
            dockerMemoryLimitInfo[dockerMemoryLimitInfo.size() - 1] *= 1024 * 1024;

        dockerMemoryInfo.push_back(atof(s[6].c_str()));
//        dockerNetworkInputInfo.push_back(atof(s[7].c_str()));
//        dockerNetworkOutputInfo.push_back(atof(s[9].c_str()));
    }

    for(int i = 0; i < dockerIDInfo.size(); ++i){
        cout << dockerIDInfo[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < dockerCpuInfo.size(); ++i){
        cout << dockerCpuInfo[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < dockerMemoryUsageInfo.size(); ++i){
        cout << dockerMemoryUsageInfo[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < dockerMemoryLimitInfo.size(); ++i){
        cout << dockerMemoryLimitInfo[i] << " ";
    }
    cout << endl;

    /*
    for(int i = 0; i < dockerNetworkInputInfo.size(); ++i){
        cout << dockerNetworkInputInfo[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < dockerNetworkOutputInfo.size(); ++i){
        cout << dockerNetworkOutputInfo[i] << " ";
    }
    cout << endl;
    */

    //mac address info
    cout << endl << "mac address info :" << endl;
    string_exec_command("cat /sys/class/net/eth0/address", macAddress);
    double macId = stringToHashCode(macAddress[0]);
    macAddressId.push_back(macId);
    cout << macAddress[0] << endl;

    //server location info
    cout << endl << "server location info :" << endl;
    locationInfo.push_back(location);
    cout << locationInfo[0] << endl;

    //timestamp info
    cout << endl << "timestamp info :" << endl;
    string_exec_command("date \"+%Y-%m-%d %H:%M:%S\"", timestampInfo);
    cout << timestampInfo[0] << endl;


    return data_to_json(cpuSourceAvailable, cpuMHz, cpuCores, memoryTotalInfo, memoryAvailableInfo, diskTotalInfo, diskAvailableInfo, ethFirstInfo, ethSecondInfo,
    dockerIDInfo, dockerCpuInfo, dockerMemoryUsageInfo, dockerMemoryLimitInfo, dockerMemoryInfo, macAddressId, locationInfo, timestampInfo);
}

#endif //CLIENT_LINUXCOMMAND_H
