//
// Created by yanhao on 2019/10/9.
//

#ifndef CLIENT_JSONTOMYSQL_H
#define CLIENT_JSONTOMYSQL_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <fstream>
#include "json_include/json.h"
#include "mysql_include/mysql.h"

using namespace std;

void outputInfo(vector<double> temp){
    for(int i = 0; i < temp.size(); ++i){
        cout << temp[i] << " ";
    }
    cout << endl;
}
//home/yanhao/local/mysql-5.5.37/bin/mysql -h localhost -u root
void jsonToMysql(){
    vector<double> cpuSourceAvailable;
    vector<double> cpuMHz;
    vector<double> cpuCores;
    vector<double> memoryInfo;
    vector<double> diskInfo;
    vector<double> ethUploadInfo;
    vector<double> ethDownloadInfo;
    vector<string> dockerIDInfo;
    vector<double> dockerCpuInfo;
    vector<double> dockerMemoryUsageInfo;
    vector<double> dockerMemoryLimitInfo;
    vector<double> dockerMemoryInfo;
//    vector<double> dockerNetworkInputInfo;
//    vector<double> dockerNetworkOutputInfo;
    vector<double> madAddressInfo;

    ifstream ifs;
    ifs.open("resource.json");
    Json::Value value;
    Json::Reader jsonReader;

    if(jsonReader.parse(ifs, value)){
        Json::Value cpu = value["cpu info"];
        for(int i = 0; i < cpu.size(); ++i){
            cpuSourceAvailable.push_back(cpu[i]["cpu source"].asDouble());
            cpuMHz.push_back(cpu[i]["cpu Mhz"].asDouble());
            cpuCores.push_back(cpu[i]["cpu Cores"].asDouble());
        }

        Json::Value memory = value["memory info"];
        memoryInfo.push_back(memory[0]["Total"].asDouble());
        memoryInfo.push_back(memory[0]["Available"].asDouble());

        Json::Value disk = value["disk info"];
        diskInfo.push_back(disk[0]["Total"].asDouble());
        diskInfo.push_back(disk[0]["Available"].asDouble());

        Json::Value network = value["network info"];
        for(int i = 0; i < network.size(); ++i){
            ethUploadInfo.push_back(network[i]["upload rate"].asDouble());
            ethDownloadInfo.push_back(network[i]["download rate"].asDouble());
        }

        Json::Value docker = value["docker info"];
        for(int i = 0; i < dockerIDInfo.size(); ++i){
            dockerIDInfo.push_back(docker[i]["id"].asString());
            dockerCpuInfo.push_back(docker[i]["cpu"].asDouble());
            dockerMemoryUsageInfo.push_back(docker[i]["memory usage"].asDouble());
            dockerMemoryLimitInfo.push_back(docker[i]["memory limit"].asDouble());
            dockerMemoryInfo.push_back(docker[i]["memory"].asDouble());
//            dockerNetworkInputInfo.push_back(docker[i]["network input"].asDouble());
//            dockerNetworkOutputInfo.push_back(docker[i]["network output"].asDouble());
        }

        Json::Value macAddress = value["mac address info"];
        memoryInfo.push_back(macAddress[0]["id"].asDouble());
	
    }

    /*outputInfo(cpuSourceAvailable);
    outputInfo(cpuMHz);
    outputInfo(cpuCores);
    outputInfo(memoryInfo);
    outputInfo(diskInfo);
    outputInfo(ethUploadInfo);
    outputInfo(ethDownloadInfo);*/

    /*
    MYSQL conn;
    int res;
    mysql_init(&conn);
//    if(mysql_real_connect(&conn,"localhost","root","","linux_resource",0,"/home/yanhao/local/mysql-5.5.37/mysql.sock",0) == NULL){
//        fprintf(stderr, "error: %s",mysql_error(&conn));
//    }
    if(mysql_real_connect(&conn, "localhost", "root", "", "linux_resource", 0, "/home/yanhao/local/mysql-5.5.37/mysql.sock", 0)){
        cout << "connect success!" << endl;
        res = mysql_query(&conn, "delete from cpu");
        res = mysql_query(&conn, "delete from disk");
        res = mysql_query(&conn, "delete from memory");
        res = mysql_query(&conn, "delete from network");
        if(res){
            cout << "error" << endl;
        }
        else{
            cout << "delete OK" << endl;
        }

        char* s = new char[100];
        string temp = "";
        for(int i = 0; i < cpuMHz.size(); ++i){
            temp = "insert into cpu values('" + to_string(i) + "','" + to_string(cpuSourceAvailable[i]) + "','" + to_string(cpuMHz[i]) + "','" + to_string(cpuCores[i]) + "')";
            strcpy(s, temp.c_str());
            res = mysql_query(&conn, s);
            if(res){
                cout << "error" << endl;
            }
            else{
                cout << "cpu insert OK" << endl;
            }

        }

        temp = "insert into memory values('" + to_string(memoryInfo[0]) + "','" + to_string(memoryInfo[1]) + "','" + to_string(1 - memoryInfo[1] / memoryInfo[0]) + "')";
        strcpy(s, temp.c_str());
        res = mysql_query(&conn, s);
        if(res){
            cout << "error" << endl;
        }
        else{
            cout << "memory insert OK" << endl;
        }

        temp = "insert into disk values('" + to_string(diskInfo[0]) + "','" + to_string(diskInfo[1]) + "','" + to_string(1 - diskInfo[1] / diskInfo[0]) + "')";
        strcpy(s, temp.c_str());
        res = mysql_query(&conn, s);
        if(res){
            cout << "error" << endl;
        }
        else{
            cout << "disk insert OK" << endl;
        }

        for(int i = 0; i < ethUploadInfo.size(); ++i){
            temp = "insert into network values('" + to_string(i) + "','" + to_string(ethUploadInfo[i]) + "','" + to_string(ethDownloadInfo[i]) + "')";
            strcpy(s, temp.c_str());
            res = mysql_query(&conn, s);
            if(res){
                cout << "error" << endl;
            }
            else{
                cout << "network insert OK" << endl;
            }

        }

        mysql_close(&conn);
    }
    else{
        cout << "connect fail" << endl;
    }
     */
}

#endif //CLIENT_JSONTOMYSQL_H
