#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int wlan_register(int, int, int);
int wlan_deregister(int);
int wlan_move(int, int, int);
int wlan_send(int,char *);
int wlan_recv(int, char *);

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

struct node *nlhead;
struct sockaddr_in myskt;
int count, s;

int main() {
  uint8_t type, *data;
  char *buf;
  in_port_t myport;
  struct in_addr ipaddr;
  socklen_t sktlen;
  sktlen = sizeof(myskt);
  myport = atoi("20000");
  nlhead = NULL;

  if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }
  
  if(bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
    perror("bind");
    exit(1);
  }

  buf = (char *)malloc(sizeof(struct packet));

  while(1) {
    if((count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    wlan_recv(sizeof(buf), buf);
  printf("a\n");
  }
  return 0;
}

int wlan_register(int id, int x, int y) {
  struct node *nd, *nd_new;
  nd = nlhead;

  while(1) {
    if(nd->id == 0) {
      nd->id = id;
      nd->x = x;
      nd->y = y;
      return 0;
    }
      if(nd->id == id) return -1;
      if(nd->fp == NULL) {
        nd_new = (struct node *)malloc(sizeof(struct node));
        nd_new->id = id;
        nd_new->x = x;
        nd_new->y = y;
        nd->fp = nd_new;
        nd_new->bp = nd;
        return 0;
      }
      nd = nd->fp;
  }
}

int wlan_deregister(int id) {
  return 0;
}

int wlan_move(int id, int x, int y) {
  return 0;
}

int wlan_send(int size,char *buf) {
  char * data;
  uint8_t type;
  uint16_t length;
  uint32_t status;
  struct packet *recv_header, *send_header;
  struct node *nd;
  
  if((count = sendto(s, buf, size, 0, (struct sockaddr *)&myskt, sizeof(myskt))) < 0) {
    perror("sendto");
    return -1;
  }
  switch(type) {
      case 0x01:
        printf("regsiter_request sent!\n");
        wlan_register(nd->id, nd->x, nd->y);
        data = (char *) send_header;
        wlan_send(sizeof(data), data);
        break;
      case 0x02:
        printf("register_response sent!\n");
        break;
      case 0x03:
        printf("deregister_request sent!\n");
        wlan_deregister(nd->id);
        break;
      case 0x04:
        printf("deregister_response sent!\n");
        break;
      case 0x05:
        printf("move_request received!\n");
        wlan_move(nd->id, nd->x, nd->y);
        break;
      case 0x06:
        printf("move_response received!\n");
        break;
      case 0x07:
        printf("wlan_send_request received!\n");
        break;
      case 0x08:
        printf("wlan_send_response received!\n");
        break;
      case 0x09:
        printf("wlan_recv_request received!\n");
        break;
      case 0x0a:
        printf("wlan_recv_response received!\n");
        break;
      default:
        break;
  }
  return 0;
}

int wlan_recv(int size, char *buf) {
  char *data;
  uint8_t type;
  uint16_t length;
  uint32_t status;
  struct packet *recv_header, *send_header;
  struct node *nd;

  printf("1\n");
  recv_header = (struct packet *)malloc(sizeof(struct packet));
  recv_header = (struct packet *) buf;
  type = recv_header->type;
  printf("type: %04x\n", type);
  length = recv_header->length;
  printf("lenth: %04x\n", length);
  status = recv_header->status;
  nd = recv_header->data;

  switch(type) {
      case 0x01:
        printf("regsiter_request received!\n");
        wlan_register(nd->id, nd->x, nd->y);

        send_header->type = 0x02;
        send_header->length = 0;
       send_header->status = 0;
        data = (char *) send_header;
        wlan_send(sizeof(data), data);
        break;
      case 0x02:
        printf("register_response received!\n");
        break;
      case 0x03:
        printf("deregister_request recieved!\n");
        wlan_deregister(nd->id);
        break;
      case 0x04:
        printf("deregister_response recieved!\n");
        break;
      case 0x05:
        printf("move_request received!\n");
        wlan_move(nd->id, nd->x, nd->y);
        break;
      case 0x06:
        printf("move_response received!\n");
        break;
      case 0x07:
        printf("wlan_send_request received!\n");
        break;
      case 0x08:
        printf("wlan_send_response received!\n");
        break;
      case 0x09:
        printf("wlan_recv_request received!\n");
        break;
      case 0x0a:
        printf("wlan_recv_response received!\n");
        break;
      default:
        break;
  }
  return 0;
}
