#ifndef PCBTABLEADT_H
#define PCBTABLEADT_H

 #define RUNNING 0;
 #define BLOCKED 1;
 #define READY 2;
 #define TRUE 1;
 #define FALSE 0;
 #define NULL 0;


 typedef struct pcbCDT * pcbADT;

 static int idCount = 0;
 static pcbADT pcbTable[];//= direccion donde quieran ponerlo
 static pcbADT last = NULL;
 static MAX_CHILDREN = 10;
 static int tableSize = 0;

 int addPCB(int privilege);
 void removePCB(int id);
 void changeState(int id, int state);
 void addChild(int fatherId);
 void createChild(pcbADT father);
 pcbADT remove(pcbADT current, int id);
 
 #endif PCBTABLEADT_H
