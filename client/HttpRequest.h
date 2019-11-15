//
// Created by thinker on 7/6/16.
//

#ifndef PROJECT_HTTPREQ_H
#define PROJECT_HTTPREQ_H

#endif //PROJECT_HTTPREQ_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#define BUFSIZE 41000
#define URLSIZE 1024
#define INVALID_SOCKET -1
#define __DEBUG__

using namespace std;

class HttpRequest {
public:
    HttpRequest();
    ~HttpRequest();
    void DebugOut(const char *fmt, ...);

    int HttpGet(const char* strUrl, char* strResponse);
    int HttpPost(const char* strUrl, const char* strData, char* strResponse);

private:
    int   HttpRequestExec(const char* strMethod, const char* strUrl, const char* strData, char* strResponse);
    char* HttpHeadCreate(const char* strMethod, const char* strUrl, const char* strData);
    char* HttpDataTransmit(char *strHttpHead, const int iSockFd);

    int   GetPortFromUrl(const char* strUrl);
    char* GetIPFromUrl(const char* strUrl);
    char* GetParamFromUrl(const char* strUrl);
    char* GetHostAddrFromUrl(const char* strUrl);

    int   SocketFdCheck(const int iSockFd);

    static int m_iSocketFd;
};

int HttpRequest::HttpGet(const char* strUrl, char* strResponse) {
    return HttpRequestExec("GET", strUrl, NULL, strResponse);
}

int HttpRequest::HttpPost(const char* strUrl, const char* strData, char* strResponse) {
    return HttpRequestExec("POST", strUrl, strData, strResponse);
}


//Execute http request (GET/POST)
int HttpRequest::HttpRequestExec(const char* strMethod, const char* strUrl, const char* strData, char* strResponse) {
    //judge the url
    if((strUrl == NULL) || (0 == strcmp(strUrl, ""))) {
        DebugOut("%s %s %d\turl is empty\n", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }

    //url length limitation
    if(URLSIZE < strlen(strUrl)) {
        DebugOut("%s %s %d\turl length can not over %d\n", __FILE__, __FUNCTION__, __LINE__, URLSIZE);
        return 0;
    }

    //create the http header
    char* strHttpHead = HttpHeadCreate(strMethod, strUrl, strData);

    if(m_iSocketFd != INVALID_SOCKET) {
        //Check whether SocketFd could be written or not
        if(SocketFdCheck(m_iSocketFd) > 0) {
            char* strResult = HttpDataTransmit(strHttpHead, m_iSocketFd);
            if(NULL != strResult) {
                strcpy(strResponse, strResult);
                return 1;
            }
        }
    }

    //Create socket
    m_iSocketFd = INVALID_SOCKET;
    m_iSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_iSocketFd < 0 ) {
        DebugOut("%s %s %d\tsocket error! Error code: %d，Error message: %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        return 0;
    }

    //Bind address and port
    int iPort = GetPortFromUrl(strUrl);
    if(iPort < 0) {
        DebugOut("%s %s %d\tFailed to get the port from the url\n", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }
    char* strIP = GetIPFromUrl(strUrl);
    if(strIP == NULL) {
        DebugOut("%s %s %d\tFailed to get the ip from the url\n", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(iPort);
    if (inet_pton(AF_INET, strIP, &servaddr.sin_addr) <= 0 ) {
        DebugOut("%s %s %d\tinet_pton error! Error code: %d，Error message: %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        close(m_iSocketFd);
        m_iSocketFd = INVALID_SOCKET;
        return 0;
    }

    //Set non-blocking
    int flags = fcntl(m_iSocketFd, F_GETFL, 0);
    if(fcntl(m_iSocketFd, F_SETFL, flags|O_NONBLOCK) == -1) {
        close(m_iSocketFd);
        m_iSocketFd = INVALID_SOCKET;
        DebugOut("%s %s %d\tfcntl error! Error code: %d，Error message: %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        return 0;
    }

    //Non-blocking connection
    int iRet = connect(m_iSocketFd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(iRet == 0) {
        char* strResult = HttpDataTransmit(strHttpHead, m_iSocketFd);
        if(NULL != strResult) {
            strcpy(strResponse, strResult);
            free(strResult);
            return 1;
        } else {
            close(m_iSocketFd);
            m_iSocketFd = INVALID_SOCKET;
            free(strResult);
            return 0;
        }
    }
    else if(iRet < 0) {
        if(errno != EINPROGRESS) {
            return 0;
        }
    }

    iRet = SocketFdCheck(m_iSocketFd);
    if(iRet > 0) {
        char* strResult = HttpDataTransmit(strHttpHead, m_iSocketFd);
        if(NULL == strResult) {
            close(m_iSocketFd);
            m_iSocketFd = INVALID_SOCKET;
            return 0;
        }
        else {
            strcpy(strResponse, strResult);
            free(strResult);
            return 1;
        }
    }
    else {
        close(m_iSocketFd);
        m_iSocketFd = INVALID_SOCKET;
        return 0;
    }

    return 1;
}


//create http header
char* HttpRequest::HttpHeadCreate(const char* strMethod, const char* strUrl, const char* strData) {
    char* strHost = GetHostAddrFromUrl(strUrl);
    char* strParam = GetParamFromUrl(strUrl);


    char* strHttpHead = (char*)malloc(BUFSIZE);
    memset(strHttpHead, 0, BUFSIZE);

        strcat(strHttpHead, strMethod);
        strcat(strHttpHead, " /");
        strcat(strHttpHead, strParam);
        free(strParam);
        strcat(strHttpHead, " HTTP/1.1\r\n");
        strcat(strHttpHead, "Accept: */*\r\n");
        strcat(strHttpHead, "Accept-Language: en-us,en;q=0.5\r\n");
        //strcat(strHttpHead, "Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7\r\n");
        strcat(strHttpHead, "User-Agent: Mozilla/4.0\r\n");
        strcat(strHttpHead, "Host: ");
        strcat(strHttpHead, strHost);
        strcat(strHttpHead, "\r\n");
        strcat(strHttpHead, "Cache-Control: no-cache\r\n");
        strcat(strHttpHead, "Connection: Keep-Alive\r\n");


    if(0 == strcmp(strMethod, "POST")) {
        char len[8] = {0};
        unsigned uLen = strlen(strData);
        sprintf(len, "%d", uLen);

        strcat(strHttpHead, "Content-Type: text/html\r\n");
        strcat(strHttpHead, "Content-Length: ");
        strcat(strHttpHead, len);
        strcat(strHttpHead, "\r\n\r\n");
        strcat(strHttpHead, strData);
    }
    strcat(strHttpHead, "\r\n\r\n");

    free(strHost);
    cout << strHttpHead << endl;

    return strHttpHead;
}


//send the http request and get the response
char* HttpRequest::HttpDataTransmit(char *strHttpHead, const int iSockFd) {
    char* buf = (char*)malloc(BUFSIZE);
    memset(buf, 0, BUFSIZE);
    int ret = send(iSockFd,(void *)strHttpHead,strlen(strHttpHead)+1,0);
    free(strHttpHead);
    if (ret < 0) {
        DebugOut("%s %s %d\tsend error! Error code: %d，Error message: %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        close(iSockFd);
        return NULL;
    }

    while(1)
    {
        ret = recv(iSockFd, (void *)buf, BUFSIZE,0);
        if (ret == 0) //close the connection
        {
            close(iSockFd);
            return NULL;
        }
        else if(ret > 0) {
            return buf;
        }
        else if(ret < 0) //error
        {
            if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            else {
                close(iSockFd);
                return NULL;
            }
        }
    }
}


//get the IP from the url
char* HttpRequest::GetHostAddrFromUrl(const char* strUrl) {
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//判断有没有http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//判断有没有https://
        if(strAddr != NULL) {
            strAddr += 8;
        }
    } else {
        strAddr += 7;
    }

    if(strAddr == NULL) {
        strAddr = url;
    }
    int iLen = strlen(strAddr);
    char* strHostAddr = (char*)malloc(iLen+1);
    memset(strHostAddr, 0, iLen+1);
    for(int i=0; i<iLen+1; i++) {
        if(strAddr[i] == '/') {
            break;
        } else {
            strHostAddr[i] = strAddr[i];
        }
    }

    return strHostAddr;
}


//Get the parameters from the http request
char* HttpRequest::GetParamFromUrl(const char* strUrl) {
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//judge http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//judge https://
        if(strAddr != NULL) {
            strAddr += 8;
        }
    } else {
        strAddr += 7;
    }

    if(strAddr == NULL) {
        strAddr = url;
    }
    int iLen = strlen(strAddr);
    char* strParam = (char*)malloc(iLen+1);
    memset(strParam, 0, iLen+1);
    int iPos = -1;
    for(int i=0; i<iLen+1; i++) {
        if(strAddr[i] == '/') {
            iPos = i;
            break;
        }
    }
    if(iPos == -1) {
        strcpy(strParam, "");;
    } else {
        strcpy(strParam, strAddr+iPos+1);
    }
    return strParam;
}


//Get the port from the url
int HttpRequest::GetPortFromUrl(const char* strUrl) {
    int iPort = -1;
    char* strHostAddr = GetHostAddrFromUrl(strUrl);
    if(strHostAddr == NULL) {
        return -1;
    }

    char strAddr[URLSIZE] = {0};
    strcpy(strAddr, strHostAddr);
    free(strHostAddr);

    char* strPort = strchr(strAddr, ':');
    if(strPort == NULL) {
        iPort = 80;
    } else {
        iPort = atoi(++strPort);
    }
    return iPort;
}


//Get the IP from the url
char* HttpRequest::GetIPFromUrl(const char* strUrl) {
    char* strHostAddr = GetHostAddrFromUrl(strUrl);
    int iLen = strlen(strHostAddr);
    char* strAddr = (char*)malloc(iLen+1);
    memset(strAddr, 0, iLen+1);
    int iCount = 0;
    int iFlag = 0;
    for(int i=0; i<iLen+1; i++) {
        if(strHostAddr[i] == ':') {
            break;
        }

        strAddr[i] = strHostAddr[i];
        if(strHostAddr[i] == '.') {
            iCount++;
            continue;
        }
        if(iFlag == 1) {
            continue;
        }

        if((strHostAddr[i] >= '0') || (strHostAddr[i] <= '9')) {
            iFlag = 0;
        } else {
            iFlag = 1;
        }
    }
    free(strHostAddr);

    if(strlen(strAddr) <= 1) {
        return NULL;
    }

    if((iCount == 3) && (iFlag == 0)) {
        return strAddr;
    } else {
        struct hostent *he = gethostbyname(strAddr);
        free(strAddr);
        if (he == NULL) {
            return NULL;
        } else {
            struct in_addr** addr_list = (struct in_addr **)he->h_addr_list;
            for(int i = 0; addr_list[i] != NULL; i++) {
                return inet_ntoa(*addr_list[i]);
            }
            return NULL;
        }
    }
}

//Check the SocketFd
int HttpRequest::SocketFdCheck(const int iSockFd) {
    struct timeval timeout ;
    fd_set rset,wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_SET(iSockFd, &rset);
    FD_SET(iSockFd, &wset);
    timeout.tv_sec = 3;
    timeout.tv_usec = 500;
    int iRet = select(iSockFd+1, &rset, &wset, NULL, &timeout);
    if(iRet > 0) {
        int iW = FD_ISSET(iSockFd,&wset);
        int iR = FD_ISSET(iSockFd,&rset);
        if(iW && !iR)
        {
            char error[4] = "";
            socklen_t len = sizeof(error);
            int ret = getsockopt(iSockFd,SOL_SOCKET,SO_ERROR,error,&len);
            if(ret == 0)
            {
                if(!strcmp(error, ""))
                {
                    return iRet;
                }
                else
                {
                    DebugOut("%s %s %d\tgetsockopt error code:%d,error message:%s", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
                }
            }
            else
            {
                DebugOut("%s %s %d\tgetsockopt failed. error code:%d,error message:%s", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
            }
        }
        else
        {
            DebugOut("%s %s %d\tsockFd written：%d，is written：%d\t(0 is not exist)\n", __FILE__, __FUNCTION__, __LINE__, iW, iR);
        }
    }
    else if(iRet == 0)
    {
        return 0;//timeout
    }
    else
    {
        return -1;//select error
    }
    return -2;//errors
}

//Print
void HttpRequest::DebugOut(const char *fmt, ...) {
#ifdef __DEBUG__
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
#endif
}


int HttpRequest::m_iSocketFd = INVALID_SOCKET;
