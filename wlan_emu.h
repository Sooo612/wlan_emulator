#ifndef __WLAN_EMU_H__
#define __WLAN_EMU_H__

int wlan_register(int, int, int);
int wlan_deregister(int);
int wlan_move(int, int, int);
int wlan_send(int,char *);
int wlan_recv(int, char *);

#endif
