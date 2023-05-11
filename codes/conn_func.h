#ifndef CONN_FUNC_H
#define CONN_FUNC_H

#include <cerrno>
#include <iostream>
#include <sys/types.h> 
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>

#define SEM_FOLDER "./shm"
#define PERMS 0666
#define SHM_SIZE 100

#define MY_LINE_MAX 15000 +100
#define MSG_MAX 1024 +100
#define COMMAND_MAX 256 +50
#define MY_NAME_MAX 20 +10
#define NUM_USER 30
#define MAX_USER 30

struct pnt // pipe number to
{
    ssize_t pipe_num;
    int remain;
    
    void set(ssize_t p, int r)
    {
        pipe_num = p;
        remain = r;        
    }
};

struct client_pid
{
    int id;
    int connfd = -1;
    int pid;
    char name[MY_NAME_MAX];
    char addr[20];
    int port;
    
    client_pid () {};
    client_pid (int i) {id = i;}
    client_pid (const char *n) {strcpy(name, n);}

    void reset(int i)
    // void reset()
    {
        id = i;
        connfd = -1;
        pid = -1;
        strcpy(name, "(no name)");
        strcpy(addr, "0.0.0.0");
        port = -1;
    }

    void set(int p)
    {
        pid = p;
    }

    void setaddr(int fd, sockaddr_in a)
    {
        connfd = fd;
        strcpy(addr, inet_ntoa(a.sin_addr));
        port = (int)ntohs(a.sin_port);
    }
};

extern client_pid cp[NUM_USER];
extern key_t shm_key[3]; // user data, broadcast
extern int shm_id[3];

void init();

int my_connect(int &listenfd, char *port, sockaddr_in &servaddr);
int handle_new_connection(int &connfd, const int listenfd);
bool check_usr_exist(char *name);

void close_client(int index);
void broadcast(char *msg);
void tell(char *msg, int to);

void alter_num_user(int amount);
void write_user_info(client_pid c);
bool read_user_info(client_pid &c, int id);
bool format_user_info(char *row_data, client_pid &c);
int get_shm_num(int s_id);
int get_read_user();
void update_read_user();

void err_sys(const char *err);
#endif