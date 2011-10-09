/*
Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>

This file is part of EnigmaNet
EnigmaNet is free software and comes with ABSOLUTELY NO WARRANTY.
See LICENSE for details.
*/

#include "net.h"
#define TCP

int main(int argc, char *argv[]) {
#ifdef TCP
 int s = net_init_tcp(NULL,"2334",0);
#else
 int s = net_init_udp("2334",0);
#endif
 if (s < 0) {
  printf("Failed to connect (%d)\n",s);
  return 0;
 }
 printf("Connected to %d\nSending: %s\n",s,"Hello World");
 int r;
 char *msg = "Hello World";
 if ((r = net_send(s,msg,strlen(msg)+1) < 0) {
  printf("Send error (%d, %d)\n",r,errno);
  return 0;
 }
 char *str;
 if ((str = net_receive(s)) == NULL)
  printf("Receive error (%d)\n",errno);
 else
  printf("Received: %s\n",str);
 closesocket(s);
 printf("Session terminated\n");
}

