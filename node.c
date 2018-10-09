#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

struct node {
  struct node *fp;
  struct node *bp;
  in_port_t port;
  uint16_t recvflag;
  int32_t id;
  int x;
  int y;
};

struct packet {
  uint8_t type;
  uint16_t length;
  struct node *data;
  uint32_t status;
};

int s, count, datalen;
char *buf, *ip;
in_port_t port;
struct sockaddr_in skt;
struct in_addr ipaddr;

int main() {
  struct packet *send_header;
  struct node *nd;
  int32_t id;
  int x;
  int y;
  uint32_t hostlong;
  uint16_t hostshort;
  ip = (char *)malloc(sizeof(char) * 100);
  port = atoi("20000");                                                 //socket
  strcpy(ip, "0.0.0.0");
  if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
  }

  memset(&skt, 0, sizeof(skt));
  skt.sin_family = AF_INET;
  skt.sin_port = htons(port);
  skt.sin_addr.s_addr = INADDR_ANY; 

  id = 2;
  x = 3;
  y = 1;

  nd = (struct node *)malloc(sizeof(struct node));
  nd->id = id;
  nd->x = x;
  nd->x = y;

  send_header->type = 0x02;
  send_header->length = 0;
  send_header->status = 0;
  send_header->data = nd;

  buf = (char *)send_header;
  //bind
  printf("%s\n", buf);

  if((count = sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&skt, sizeof skt)) < 0) {
    perror("sendto");
    exit(1);
  }

  if(close(s) < 0) {
    perror("close");
    exit(1);
  }
}
