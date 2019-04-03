#include "stdio.h"
#include "../dlog/dlog.h"

int main(int argc, char const *argv[])
{
    printf("你好！！\r\n");
    dlog_init("utlinux", "utlinux", dlog_init_relative::MODULE, false);
    return 0;
}
