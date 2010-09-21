#include "net.c"
#define TCP

int main(int argc, char *argv[]) {
 getIp();
}

int main2(int argc, char *argv[]) {
#ifdef TCP
 int s = mplay_init_tcp("0","2334",1);
#else
 int s = mplay_init_udp("2334",1);
#endif
 if (s < 0) {
  printf("Failed to serve (%d)\n",s);
  return 0;
 }
 printf("%d serving...\n",s);
#ifdef TCP
 int c = mplay_accept(s);
 if (c < 0) {
  printf("Client not found (%d)\n",c);
  return 0;
 }
 printf("Connected %d\n",c);
 char *str = mplay_receive(c);
 if (str == NULL)
  printf("Receive error (%d)\n",errno);
 else
  printf("Received: %s\nSending: %s\n",str,"Goodbye Cruel World");
 char *msg = "Goodbye Cruel World";
 if (mplay_send(c,msg,strlen(msg)+1) < 0)
  printf("Send error (%d)\n",errno);
 closesocket(c);
#else
 mplay_bounce(s);
#endif
 closesocket(s);
 printf("Session terminated\n");
}

