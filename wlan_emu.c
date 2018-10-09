#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "wlan_emu.h"

int wlan_register(int, int, int);
int wlan_deregister(int);
int wlan_move(int, int, int);
int wlan_send(int,char);
int wlan_recv(int, char, int);
void wlan_sock_init(char *, in_port_t, struct sockaddr_in);

struct node {
  struct node *fp;
  struct node *bp;
  int_port_t port;
  uint16_t recvflag;
  int32_t id;
  int x;
  int y;
}

struct node nlhead;
int s, count;
in_port_t myport;

void sockaddr_init (char *address, in_port_t port, struct sockaddr_in *skt) {
    skt.sin_family = AF_INET;

    if (inet_aton(address, &skt.sin_addr) == 0) {
        if (strcmp(address, "") == 0 ) {
            skt.sin_addr.s_addr = INADDR_ANY;
        } else {
            fprintf(stderr, "Invalid IP Address.\n");
            exit(-1);
        }
    }

    if (port == 0) {
        fprintf(stderr, "invalid port number.\n");
        exit(-1);
    }

    skt.sin_port = myport;
}

int wlan_register(int32_t id, int x, int y) {
  struct node *nd, *nd_new;
  nd = nlhead;

  while(true) {
      if(nd->id == id) return -1;
      if(nd->fp == NULL) {
        nd_new = (struct node)malloc(sizeof(struct node));
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
  struct node *nd;

  nd = nlhead;
  while(true) {
    if(nd->id == id) {
      nd->bp->fp = nd->fp;      //削除処理
      nd->fp->bp = nd->bp;
        return 0;
    }
    if(nd->fp == NULL) return -1;
    nd = nd->fp;
  }
}

int wlan_move(uint32_t id, int x, int y) {
  struct node *nd;
  nd = nlhead;

  while(true) {
    if(nd->id == id) {
      nd->x = x;
      nd->y = y;
        return 0;
    }
    if(nd->fp == NULL) {
      printf("No such id.\n");
      return -1;
    }
    nd = nd->fp;
  }
}

int wlan_send(int size,char *buf) {
  if((count = sendto(s, buf, size, 0, (struct sockadr *)&skt, sizeof skt)) < 0) {
    perror(1);
  }
}

int wlan_recv(int size, char *buf) {
  if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  if(bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
    perror("bind");
    exit(1);
  }

  if((count = recvfrom(s, buf, size, 0, (struct sockadr *)&skt, sizeof skt)) < 0) {
    perror(1);
  }
}

