#include "net.c"
#define TCP

int main(int argc, char *argv[]) {
#ifdef TCP
 int s = mplay_init_tcp(NULL,"2334",0);
#else
 int s = mplay_init_udp("2334",0);
#endif
 if (s < 0) {
  printf("Failed to connect (%d)\n",s);
  return 0;
 }
 printf("Connected to %d\nSending: %s\n",s,"Hello World");
 int r;
 char *msg = "Hello World";
 if ((r = mplay_send(s,msg,strlen(msg)+1) < 0) {
  printf("Send error (%d, %d)\n",r,errno);
  return 0;
 }
 char *str;
 if ((str = mplay_receive(s)) == NULL)
  printf("Receive error (%d)\n",errno);
 else
  printf("Received: %s\n",str);
 closesocket(s);
 printf("Session terminated\n");
}

