#include "stdio.h"
#include "../dlog/dlog.h"

int main(int argc, char const *argv[])
{
    printf("你好！！\r\n");
    dlog_init("log", "utlinux", dlog_init_relative::MODULE, false);

    for (size_t i = 0; i < 100; i++) {
        LogI("LogI %d", i);
        LogW("LogW %d", i);
        LogE("LogE %d", i);
    }
    dlog_close();
    return 0;
}
