#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

#define WIDTH 80
#define HEIGHT 25
#define BUFFER_SIZE (WIDTH*HEIGHT)+1000

#ifndef TRUE
#define TRUE 1
#endif

#ifndef  FALSE
#define FALSE 0
#endif

void printc(char ch);
void printcWithStyle(char ch, char style);
void print(char *s);
void println(char *s);
void printBase(uint64_t val, uint32_t base);
void setStyle(char style);
void incrementCursor();
void newLine();
void backspace();
void cursorUp();
void cursorDown();
void cursorLeft();
void cursorRight();
void invertStyle(uint8_t x, uint8_t y);
void shiftScreen();
void clearScreen();
void updateMouse(uint8_t x, uint8_t y);
uint8_t getCharAt(uint8_t x, uint8_t y);
void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void updateMouse(uint8_t x, uint8_t y);
void selectTo(uint8_t x, uint8_t y);
void blinkCursor();
void writeBuffer(char ch);
char readBuffer();
void keyboardLeft();
void keyboardRight();
void setEcho(uint8_t boolean);
int getFocusedPID();
void setFocusedPID(int pid);

#endif
