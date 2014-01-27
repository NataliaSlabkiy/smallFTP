#include <stdio.h>
#include <stdlib.h>
#include "SrvConfig.h"
int main(){
    srv_config sCon, slCon;
    usr_config uCon;

    char login_name[64];
    char password_name[64];
    printf("Hello! Enter your login:\n");
    scanf("%s", &login_name);
    printf("Enter your password:\n");
    scanf("%s", &password_name);
    uCon.name = login_name;
    uCon.password = password_name;

    sCon.ip = "127.0.0.1";
    sCon.port = 21;

    gen_baseinfo(&sCon);
    connection_init(&sCon);
    login(&sCon, &uCon);
    ls(&sCon, &slCon);
    pwd(&sCon);
    cwd(&sCon, "/ftp");
    pwd(&sCon);
    rmd(&sCon, "2");
    dele(&sCon, "2.jpg");
    ls(&sCon, &slCon);
    return 0;
}
