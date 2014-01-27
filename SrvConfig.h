#ifndef SRVCONFIG_H_INCLUDED
#define SRVCONFIG_H_INCLUDED
#include <winsock2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char *name;
    char *password;
}usr_config;

typedef struct{
    SOCKET sock;
    WSADATA wsa;
    struct sockaddr_in addr;
    char *ip;
    short int port;
}srv_config;

int connection_init(srv_config *sCon);
int gen_baseinfo(srv_config *sCon);
char *recv_data(SOCKET sCon);
int send_data(srv_config *sCon, char *data, int data_size);


void login(srv_config *sCon, usr_config *uCon);
void ls(srv_config *sCon, srv_config *spCon);
void pwd(srv_config *sCon);
void pasv(srv_config *sCon, srv_config *spCon);
void retr(srv_config *sCon, srv_config *spCon, char *filename);
void cwd(srv_config *sCon, char *path);
void dele(srv_config *sCon, char *path);
void rmd(srv_config *sCon, char *dir);

int Extract(char *a, char *buff, const int from, const char to);
char *CharToChar(char *a, const char b[]);
void CharReplace(char *a, const char what, const char on);
#endif // SRVCONFIG_H_INCLUDED
