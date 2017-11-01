#ifndef PCBTABLEADT_H
#define PCBTABLEADT_H

 #define RUNNING 0;
 #define BLOCKED 1;
 #define READY 2;
 #define TRUE 1;
 #define FALSE 0;
 

 static int idCount = 0;
 static pcbADT pcbTable;//= direccion donde quieran ponerlo
 static pcbADT last = NULL;

 typedef struct pcbCDT;
 typedef struct pcbCDT * pcbADT;
 void addPCB(int privilege);
 void removePCB(int id);
 void changeState(int id, int state);
 pcbADT remove(pcbADT current, int id);
 
 #endif PCBTABLEADT_H
