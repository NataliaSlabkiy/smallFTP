#include <stdio.h>
#include <stdlib.h>
#include "SrvConfig.h"
int main()
{
    srv_config sCon, slCon;
    usr_config uCon;
    uCon.name = "test";
    uCon.password = "111";
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
    ls(&sCon, &slCon);
    return 0;
}
