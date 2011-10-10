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
 int s = net_connect_tcp("0","2334",1);
#else
 int s = net_connect_udp("2334",1);
#endif
 if (s < 0) {
  printf("Failed to serve (%d)\n",s);
  return 0;
 }
 printf("%d serving...\n",s);
#ifdef TCP
 int c = net_accept(s);
 if (c < 0) {
  printf("Client not found (%d)\n",c);
  return 0;
 }
 printf("Connected %d\n",c);
 char *str = net_receive(c);
 if (str == NULL)
  printf("Receive error (%d)\n",errno);
 else
  printf("Received: %s\nSending: %s\n",str,"Goodbye Cruel World");
 char *msg = "Goodbye Cruel World";
 if (net_send(c,msg,strlen(msg)+1) < 0)
  printf("Send error (%d)\n",errno);
 closesocket(c);
#else
 net_bounce(s);
#endif
 closesocket(s);
 printf("Session terminated\n");
}

