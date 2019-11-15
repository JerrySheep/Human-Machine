//
// Created by yanhao on 2019/11/9.
//

#ifndef CLIENT_DOCKERDEPLOY_H
#define CLIENT_DOCKERDEPLOY_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

void exec_command(char* command){
    FILE *fp = NULL;
    fp = popen(command, "r");
    if(!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    pclose(fp);
}

void dockerDeploy(string dockerImage){
    char* downloadCommand = (char*)("docker pull " + dockerImage).c_str();
    exec_command(downloadCommand);
    cout << "docker download success" << endl;
    char* deployCommand = (char*)("docker run " + dockerImage).c_str();
    exec_command(deployCommand);
    cout << "docker start success" << endl;
}


#endif //CLIENT_DOCKERDEPLOY_H
