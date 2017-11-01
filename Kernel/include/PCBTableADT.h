#ifndef PCBTABLEADT_H
#define PCBTABLEADT_H

 #define RUNNING 0;
 #define BLOCKED 1;
 #define READY 2;

 static int idCount = 0;
 static pcbADT pcbTable;//= direccion donde quieran ponerlo

 typedef struct pcbCDT;
 typedef struct pcbCDT * pcbADT;
 typedef pcbADT last;
 void addPCB(int privilege);
 void removePCB(int id);
 void changeState(int id, int state);
 pcbADT remove(pcbADT current, int id);
 
 #endif PCBTABLEADT_H
