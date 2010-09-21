/*
gcc ip.c -o ip.exe -Iinclude -Llib -DCURL_STATICLIB -lcurl -lssh2 C:/MinGW/bin/libeay32.dll C:/MinGW/bin/libssl32.dll -lwldap32 -lws2_32 -lz
*/
#include <curl/curl.h>
#define IPLEN 24

char ip[IPLEN];
int p = 0;

int handle_data(void *ptr, int size, int nmemb, void *stream) {
 int i, numbytes = size * nmemb;
 char *str = (char *)ptr;
 for (i = 0; i < numbytes && p < IPLEN - 1; i++)
  ip[p++] = str[i];
 ip[p] = '\0';
}

int main() {
 char *site = "http://www.whatismyip.com/automation/n09230945.asp";
 CURL *curl = curl_easy_init();
 if (!curl) { return 0; }
 curl_easy_setopt(curl, CURLOPT_URL, site);
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_data);
 curl_easy_perform(curl);
 curl_easy_cleanup(curl);
 printf("%s\n",ip);
 return 0;
}

