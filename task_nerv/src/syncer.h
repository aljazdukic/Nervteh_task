#include "sys/sysinfo.h"
#include "sys/types.h"

struct sysinfo;

long get_timestamp(long time);
int get_availiable_mem(long availiable_mem_mb);
double getCurrentValue(); 
void get_udp_data();

void start_sync(long time, long availiable_mem_mb);