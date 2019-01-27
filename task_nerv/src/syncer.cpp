#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <chrono>
#include <thread>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include "syncer.h"
#include "stdio.h"
#include "sys/types.h"
#include "sys/sysinfo.h"


#define PORT     15559 
#define MAXLINE 1024 


static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void start_sync(long time, long availiable_mem_mb)
{
    std::ofstream destination_file;
    destination_file.open("/home/cimet/Documents/task_nerv/src/data.csv");
    destination_file << "TIMESTAMP;CPU;RAM;UDP_DATA\n";

    for (int i = 0; i < 100; i++)
    {
        //IF UDP DATA = 0
        destination_file << get_timestamp(time) << ";" << getCurrentValue() << ";" << availiable_mem_mb << "\n";
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        //ELSE
        //destination_file << get_timestamp(time) << ";" << getCurrentValue() << ";" << availiable_mem_mb << ";" << get_udp_data <<"\n";
        //std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
   
    destination_file.close();
}

long get_timestamp(long time)
{
    using namespace std::chrono;
    milliseconds time_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    time = time_ms.count();
    return time;
}

int get_availiable_mem(long availiable_mem_mb)
{
    //struct sysinfo memInfo;
    //availiable_mem_mb = memInfo.freeram;
    //availiable_mem_mb *= memInfo.mem_unit;
    return availiable_mem_mb;
}

double getCurrentValue()
{
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

void get_udp_data()
{
    int sockfd; 
    char buffer[MAXLINE]; 
    //char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    unsigned int n, len; 
    /*
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Hello message sent.\n"); 
    */
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
} 