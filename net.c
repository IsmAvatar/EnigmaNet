#include "net.h"
#include <errno.h>

int mplay_init(char *addr, char *port, char serve, char udp) {

#ifdef WIN32
 // Make sure windows is using Winsock version request 2.2
 WSADATA wsaData;
 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return -1;
 if (LOBYTE(wsaData.wVersion) != 2
 ||  HIBYTE(wsaData.wVersion) != 2) {
  WSACleanup();
  return -1;
 }
#endif

 struct addrinfo hints, *sinf;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_UNSPEC; //support both IPv4/6
 hints.ai_socktype = (udp == 0) ? SOCK_STREAM : SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE;

 if (serve != 0 || addr == NULL || addr[0] == '\0') addr = NULL;
 if (getaddrinfo(addr, port, &hints, &sinf) != 0) return -2;

 int s = socket(sinf->ai_family, sinf->ai_socktype, sinf->ai_protocol);
 if (s == -1) {
  freeaddrinfo(sinf);
  return -3;
 }

 if (serve == 0) { //client
  if (connect(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
   freeaddrinfo(sinf);
   closesocket(s);
   return -5;
  }
 } else { //server
  if (bind(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
   freeaddrinfo(sinf);
   closesocket(s);
   return -4;
  }
  if (udp == 0) { //server tcp
   if (listen(s, 5) == SOCKET_ERROR) {
    freeaddrinfo(sinf);
    closesocket(s);
    return -5;
   }
  } else if (0) { //server udp
   if (connect(s, sinf->ai_addr, sinf->ai_addrlen) == SOCKET_ERROR) {
    freeaddrinfo(sinf);
    closesocket(s);
    return -6;
   }
  }
 }

 freeaddrinfo(sinf);
 return s;
}

int mplay_init_tcp(char *addr, char *port, char serve) {
 return mplay_init(addr,port,serve,0);
}

int mplay_init_udp(char *localport, char serve) {
 return mplay_init(NULL,localport,serve,1);
}

int mplay_accept(int s) {
 return accept(s, NULL, NULL);
}

#define BUFSIZE 512
char buf[BUFSIZE];

char *mplay_receive(int s) {
 int r;
 if ((r = recv(s,buf,BUFSIZE,0)) == SOCKET_ERROR) return NULL;
 if (r == BUFSIZE) return buf;
 buf[r] = '\0';
 return buf;
}

int mplay_bounce(int s) {
 struct sockaddr_storage whom;
 u_int len = sizeof(whom);
 int n = recvfrom(s,buf,BUFSIZE,0,(struct sockaddr *)&whom,&len);
 if (n == 0) return 1;
 if (n == SOCKET_ERROR) return -1;
 printf("Bouncing: %s\n",buf);
 n = sendto(s,buf,n,0,(struct sockaddr *)&whom,sizeof(whom));
 if (n == 0) return 2;
 if (n == SOCKET_ERROR) return -2;
 return 0;
}

#define mplay_send(s,msg,len) send(s,msg,len,0)
/*int mplay_send(int s, char *msg, int len) {
 return send(s,msg,len,0);
}*/

int mplay_get_port(int s) {
 struct sockaddr_in sa;
 u_int sas = sizeof(sa);
 if (getsockname(s, (struct sockaddr*) &sa, &sas) == SOCKET_ERROR) {
  closesocket(s);
  return -1;
 }
 return ntohs(sa.sin_port);
}

#include <netinet/in.h>
#include <arpa/inet.h>

void getIp() {
// WSADATA wsa_Data;
// int wsa_ReturnCode = WSAStartup(0x101,&wsa_Data);

 // Get the local hostname
 int i;
 char hname[80];
 struct hostent *he;
 struct in_addr **addr_list;

 gethostname(hname, 80);
 if ((he = gethostbyname(hname)) == NULL) {
  herror("gethostbyname");
  return;
 }

 printf("Name: %s\n", he->h_name);
 addr_list = (struct in_addr **) he->h_addr_list;
 for (i = 0; addr_list[i] != NULL; i++) {
  printf("%s\n",inet_ntoa(*addr_list[i]));
 }

// WSACleanup();
}

int mplay_blocking(int s, char block) {
#ifdef WIN32
 u_long iMode = (block == 0) ? 1 : 0;
 return ioctlsocket(s,FIONBIO,&iMode);
#else
 int flags, r;
 if ((flags = fcntl(s, F_GETFL, 0)) < 0) return -1;
 if (block == 0) flags |= O_NONBLOCK;
 else flags &= ~O_NONBLOCK;
 if (fcntl(s, F_SETFL, flags) < 0) return -2;
 return 0;
#endif
}

