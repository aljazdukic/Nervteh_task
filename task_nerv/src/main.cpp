#include <iostream>
#include "syncer.h"
#include "sys/sysinfo.h"
#include "stdlib.h"


int main()
{
    long time;

    struct sysinfo memInfo;
    sysinfo (&memInfo);

    long availiable_mem_mb;
    availiable_mem_mb = memInfo.freeram;
    availiable_mem_mb *= memInfo.mem_unit;
    availiable_mem_mb /= (1024 * 1024);

    start_sync(time, availiable_mem_mb);

    return 0;
}