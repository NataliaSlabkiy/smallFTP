#include "SrvConfig.h"
const unsigned int buff_size = 512;

int gen_baseinfo(srv_config *sCon){
    if (WSAStartup(MAKEWORD(1,1), &sCon->wsa) == SOCKET_ERROR) return -1;
    if ((sCon->sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) return -1;
    sCon->addr.sin_port = htons(sCon->port);
    sCon->addr.sin_family = AF_INET;
    sCon->addr.sin_addr.S_un.S_addr = inet_addr(sCon->ip);
    return 0;
}

int connection_init(srv_config *sCon){
    if (connect(sCon->sock, &sCon->addr, sizeof(sCon->addr)) == SOCKET_ERROR) return -1;
    char *data = recv_data(sCon->sock);
    printf(data);
    return 0;
}

char *recv_data(SOCKET sCon){
    int data;
    char buff[buff_size];
    char fdata[2048]={'\0'};
    struct timeval tm = {1, 0};
    int rc, size_data=0;
    fd_set fd;
    FD_ZERO(&fd);
    FD_SET(sCon, &fd);
    do{
        memset(buff, '\0', buff_size);
        data = recv(sCon, &buff, buff_size, 0);
        rc = select(0, &fd, NULL, NULL, &tm);
        if (data == -1 || data == 0) {return fdata;};
        strcat(fdata, buff);
    }while(rc);
    return fdata;
}

int send_data(srv_config *sCon, char *data, int data_size){
    if (send(sCon->sock, data, data_size, 0) == INVALID_SOCKET) return 0;
    char *recv_datas = recv_data(sCon->sock);
    printf(recv_datas);
    return 1;
}

void login(srv_config *sCon, usr_config *uCon){
    char buff[512];
    sprintf(buff, "USER %s\r\n", uCon->name);
    send_data(sCon, buff, strlen(buff));

    sprintf(buff, "PASS %s\r\n", uCon->password);
    send_data(sCon, buff, strlen(buff));
}

void ls(srv_config *sCon, srv_config *spCon){
    printf("ls\n");
    pasv(sCon, spCon);
    char *buff = "LIST\r\n";
    send_data(sCon, buff, strlen(buff));
    connection_init(spCon);
    closesocket(spCon->sock);
}

void dele(srv_config *sCon, char *path){
    printf("dele\n");
    char buff[512];
    sprintf(buff, "DELE %s\r\n", path);
    send_data(sCon, buff, strlen(buff));
}

void rmd(srv_config *sCon, char *dir){
    printf("rmd\n");
    char buff[512];
    sprintf(buff, "RMD %s\r\n", dir);
    send_data(sCon, buff, strlen(buff));
}

void pwd(srv_config *sCon){
    printf("pwd\n");
    char *buff = "PWD\r\n";
    send_data(sCon, buff, strlen(buff));
}

void pasv(srv_config *sCon, srv_config *spCon){
    printf("pasv\n");
    char pasv_data[512];
    char *buff = "PASV\r\n";
    if (send(sCon->sock, buff, strlen(buff), 0) == INVALID_SOCKET) return 0;
    char *recv_datas = recv_data(sCon->sock);
    printf(recv_datas);
    char *ip_tmp = strstr(recv_datas, "(");
    char ip[512];
    int port;
    char port_buffer[512];
    int pos = Extract(ip_tmp, ip, 1, ',');
    int i;
    for (i=0; i!=3; i++)
        if ((pos = Extract(ip_tmp, ip, pos, ',')) ==-1) exit(-1);
    ip[strlen(ip)-1] = '\0';
    CharReplace(ip, ',', '.');
    if ((pos = Extract(ip_tmp, port_buffer, pos, ',')) ==-1) exit(-1);
    port_buffer[strlen(port_buffer)-1] = '\0';
    port = atoi(port_buffer)*256;
    memset(port_buffer, '\0', strlen(port_buffer));
    if ((pos = Extract(ip_tmp, port_buffer, pos, ')')) ==-1) exit(-1);
    port_buffer[strlen(port_buffer)-1] = '\0';
    port = port+atoi(port_buffer);
    spCon->ip = ip;
    spCon->port = port;
    gen_baseinfo(spCon);
    printf("IP: %s Port: %d\n", spCon->ip, spCon->port);
}

//================RETV does not work========================================
void retr(srv_config *sCon, srv_config *spCon, char *filename){
    /*char buff[512];
    sprintf(buff, "RETR %s\r\n", filename);
    send_data(sCon, buff, strlen(buff));
    printf("fàfafa");
    connection_init(spCon);
    closesocket(spCon->sock);
    printf("retr\n");
    srv_config ssCon;
    ssCon.ip = "192.168.1.34";
    ssCon.port = 5000;
    char buff[512];
    sprintf(buff, "RETR %s\r\n", filename);
    if (send(sCon->sock, buff, strlen(buff), 0) == INVALID_SOCKET);
    char *recv_datas = recv_data(ssCon.sock);
    printf(recv_datas);
    gen_baseinfo(&ssCon);
    connection_init(&ssCon);
    closesocket(spCon->sock);*/
    printf("retr\n");
    srv_config ssCon;
    ssCon.ip = "192.168.1.34";
    ssCon.port = 5000;
    char buff[512];

    char temp[] = "TYPE I\r\n";
    send_data(sCon, temp, strlen(temp));
    pasv(&sCon, &ssCon);
    sprintf(buff, "RETR %s\r\n", filename);
    if (send(sCon->sock, buff, strlen(buff), 0) == INVALID_SOCKET);
    gen_baseinfo(&ssCon);
    if (connect(ssCon.sock, &ssCon.addr, sizeof(ssCon.addr)) == SOCKET_ERROR) return -1;
    printf("Connected\n");
    char buff2[512];
    recv(sCon, &buff2, strlen(buff2), 0);
    printf("!!!DATA!!!\n%s\n!!!DATA END!!!", buff2);
    printf("Recv complete\n");
}

//================RETV does not work========================================

void cwd(srv_config *sCon, char *path){
    printf("cwd\n");
    char buff[512];
    sprintf(buff, "CWD %s\r\n", path);
    send_data(sCon, buff, strlen(buff));
}


void CharReplace(char *a, const char what, const char on){
    int i;
    for (i=0; a[i]!='\0'; i++)
        if (a[i] == what) a[i] = on;
}

int Extract(char *a, char *buff, const int from, const char to){
    int i;
    for(i=from; a[i]!=to; i++){
        if (a[i] == '\0') return -1;
        buff[strlen(buff)] = a[i];
        buff[strlen(buff)+1] = '\0';
    }
    buff[strlen(buff)] = to;
    buff[strlen(buff)+1] = '\0';
    return ++i;
}
