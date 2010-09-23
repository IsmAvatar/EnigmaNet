/*
Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>

This file is part of EnigmaNet
EnigmaNet is free software and comes with ABSOLUTELY NO WARRANTY.
See LICENSE for details.
*/

#include <stdio.h>
#include <string.h>

#ifdef WIN32
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #pragma comment (lib,"ws2_32")
#else
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <fcntl.h>

 #define SOCKET_ERROR -1
 #define closesocket(s) close(s)
#endif

int net_init(char*,char*,char,char);
int net_init_tcp(char*,char*,char);
int net_init_udp(char*,char);
int net_accept(int);
char *net_receive(int);
//int net_send(int,char*);
int net_get_port(int);
int net_blocking(int,char);

