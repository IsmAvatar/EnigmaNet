/*
Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>

This file is part of EnigmaNet
EnigmaNet is free software and comes with ABSOLUTELY NO WARRANTY.
See LICENSE for details.
*/

#include "net.h"
#ifdef _WIN32
 #define DLLEXPORT extern "C" __declspec(dllexport)
#else
 #define DLLEXPORT extern "C"
#endif

DLLEXPORT double dllInit() { return net_init(); }

double doConnect(char *ip, double port, double mode, bool serve, bool udp)
{
 char sport[6]; //can't exceed 65535
 snprintf(sport,6,"%f",port);
 int s = net_connect(ip,sport,serve,udp);
 if (s < 0) return -1;
 net_blocking(s,(char) mode);
 return s;
}

DLLEXPORT double tcpconnect(char*ip, double port, double mode) { return doConnect(ip,port,mode,true,false); }

DLLEXPORT double tcplisten(double port, double max, double mode) { return doConnect(NULL,port,mode,false,false); }

DLLEXPORT double udpconnect(double port, double mode) { return doConnect(NULL,port,mode,false,true); }

DLLEXPORT double closesock(double sockid) { return closesocket((int)sockid) == 0 ? 1 : -1; }

DLLEXPORT double setsync(double sockid, double mode) { net_blocking((int)sockid,(char)mode) == 0 ? 1 : -1; }

DLLEXPORT double tcpaccept(double sockid, double mode)
{
 int s = net_accept((int) sockid);
 if (s >= 0) net_blocking(s,(char) mode);
 return s;
}

