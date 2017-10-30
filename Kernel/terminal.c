#include <terminal.h>
#include <lib.h>
#include <video.h>


static video_row videoBuffer[HEIGHT];
static char kbBuffer[BUFFER_SIZE];

static uint16_t startIndex = 0;
static uint16_t endIndex = 0;
static uint16_t writeIndex = 0;
static uint16_t readIndex = 0;
static uint16_t size = 0;

static uint8_t echo = TRUE;
static uint8_t cursorVisible = FALSE;

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;
static char defaultStyle = 0x07;

static uint8_t fromX = 0;
static uint8_t fromY = 0;
static uint8_t toX = 0;
static uint8_t toY = 0;

static char buffer[64];

//==============================================================================
//  VIDEO
//==============================================================================

void updateScreen();
void toggleCursors();

void printChar(char ch, char style) {
	int x = cursorX;
	int y = cursorY;
	int prevX = -1;
	int prevY;
	while(videoBuffer[y][x].ch != 0) {
		x++;
		if(x == WIDTH) {
			x = 0;
			if(y == HEIGHT-1) {
				if(cursorY == 0)
					return;
				shiftScreen();
				cursorY--;
			}
			else
				y++;
		}
	}
	while(x != cursorX || y != cursorY) {
		prevX = x-1;
		prevY = y;
		if(prevX < 0) {
			prevX = WIDTH-1;
			prevY--;
		}
		videoBuffer[y][x] = videoBuffer[prevY][prevX];
		x = prevX;
		y = prevY;
	}
	videoBuffer[cursorY][cursorX].style = style;
	videoBuffer[cursorY][cursorX].ch = ch;
	if(prevX == -1)
		writeAtCursor(ch, style);
	else
		updateScreen();
	incrementCursor();
}

void printc(char ch) {
	printcWithStyle(ch, defaultStyle);
}

void printcWithStyle(char ch, char style) {
	switch(ch) {
		case 8:
			backspace();
			break;
		case 10:
			newLine();
			break;
		default:
			toggleCursors();
			printChar(ch, style);
			toggleCursors();
			break;
	}
}

void print(char *s) {
	toggleCursors();
	int i = 0;
	while(s[i] != 0) {
		printChar(s[i], defaultStyle);
		i++;
	}
	toggleCursors();
}

void println(char *s) {
	print(s);
	newLine();
}

void setStyle(char style) {
	defaultStyle = style;
}

void incrementCursor() {
	if(cursorX == WIDTH-1) {
		cursorX = 0;

		if(cursorY == HEIGHT-1)
			shiftScreen();
		else
			cursorY++;
	}
	else
		cursorX++;

	updateCursor(cursorX, cursorY);
}

void newLine() {
	toggleCursors();
	while (videoBuffer[cursorY][cursorX].ch != 0) {
		if(cursorX == WIDTH-1) {
			cursorX = 0;
			if(cursorY == HEIGHT-1)
				shiftScreen();
			else
				cursorY++;
		}
		else
			cursorX++;

	}

	cursorX = 0;

	if(cursorY == HEIGHT-1)
		shiftScreen();
	else
		cursorY++;

	updateCursor(cursorX, cursorY);
	updateScreen();
	toggleCursors();
}

void backspace() {
	if(cursorY > 0 || cursorX > 0) {
		toggleCursors();
		int x = cursorX;
		int y = cursorY;
		int prevX;
		int prevY;
		if(x == 0) {
			prevX = WIDTH-1;
			prevY = y-1;
		}
		else {
			prevX = x-1;
			prevY = y;
		}
		int offset = 0;
		cursorX = prevX;
		cursorY = prevY;
		while(prevY >= 0 && videoBuffer[prevY][prevX].ch == 0) {
			offset++;
			cursorX = prevX;
			cursorY = prevY;
			prevX--;
			if(prevX < 0) {
				prevX = WIDTH-1;
				prevY--;
			}
		}
		if(offset == 0)
			offset = 1;

		do {
			prevX = x - offset;
			prevY = y;
			while (prevX < 0) {
				prevX += WIDTH;
				prevY--;
			}

			videoBuffer[prevY][prevX] = videoBuffer[y][x];
			videoBuffer[y][x].ch = 0;

			if(x == WIDTH-1) {
				x = 0;
				y++;
			}
			else
				x++;

		} while(y < HEIGHT && videoBuffer[prevY][prevX].ch != 0);

		updateCursor(cursorX, cursorY);
		updateScreen();
		toggleCursors();
	}
}

void cursorUp() {
	toggleCursors();
	if(cursorY > 0)
		cursorY--;
	while (cursorX > 0 && videoBuffer[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorDown() {
	toggleCursors();
	if(cursorY < HEIGHT-1)
		cursorY++;
	while (cursorX > 0 && videoBuffer[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorLeft() {
	toggleCursors();
	if(cursorX > 0)
		cursorX--;
	else if(cursorY > 0) {
		cursorY--;
		while(cursorX < WIDTH-1 && videoBuffer[cursorY][cursorX].ch != 0)
			cursorX++;
	}
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorRight() {
	toggleCursors();
	if(cursorX == WIDTH-1 || videoBuffer[cursorY][cursorX].ch == 0) {
		if(cursorY < HEIGHT-1) {
			cursorY++;
			cursorX = 0;
		}
	}
	else
		cursorX++;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void updateScreen() {
	setScreen(videoBuffer);
}

void shiftScreen() {
	memcpy((uint8_t*) videoBuffer[0], (uint8_t*) videoBuffer[1], CELLSIZE*WIDTH*(HEIGHT-1));
	for(uint8_t x = 0; x < WIDTH; x++) {
		videoBuffer[HEIGHT-1][x].ch = 0;
		videoBuffer[HEIGHT-1][x].style = defaultStyle;
	}
	updateScreen();
}

void clearScreen() {
	toggleCursors();
	for(uint8_t y = 0; y < HEIGHT; y++) {
		for(uint8_t x = 0; x < WIDTH; x++) {
			videoBuffer[y][x].ch = 0;
			videoBuffer[y][x].style = defaultStyle;
		}
	}
	cursorX = 0;
	cursorY = 0;
	updateCursor(0, 0);
	updateScreen();
	toggleCursors();
}

uint8_t getCharAt(uint8_t x, uint8_t y) {
    return videoBuffer[y][x].ch;
}

void printBase(uint64_t value, uint32_t base) {
	uintToBase(value, buffer, base);
	print(buffer);
}

void printDec(uint64_t value) {
	printBase(value, 10);
}

void printHex(uint64_t value) {
	printBase(value, 16);
}

void printBin(uint64_t value) {
	printBase(value, 2);
}

void toggleCursors() {
	if(cursorVisible) {
		videoBuffer[cursorY][cursorX].style = 0x77 ^ videoBuffer[cursorY][cursorX].style;
		writeStyle(cursorX, cursorY, videoBuffer[cursorY][cursorX].style);
	}

	int xOff = toX - fromX;
	int yOff = toY - fromY;
	int xStep = (xOff > 0)? 1 : -1;
	int yStep = (yOff > 0)? 1 : -1;
	xOff += xStep;
	yOff += yStep;
	do {
		yOff -= yStep;
		do {
			xOff -= xStep;
			videoBuffer[fromY+yOff][fromX+xOff].style = 0x77 ^ videoBuffer[fromY+yOff][fromX+xOff].style;
			writeStyle(fromX+xOff, fromY+yOff, videoBuffer[fromY+yOff][fromX+xOff].style);
		}while(xOff != 0);
		xOff = toX - fromX + xStep;
	}while(yOff != 0);
}

void updateMouse(uint8_t x, uint8_t y) {
	toggleCursors();
	fromX = toX = x;
	fromY = toY = y;
	toggleCursors();
}

void selectTo(uint8_t x, uint8_t y) {
	toggleCursors();
	toX = x;
	toY = y;
	toggleCursors();
}

void blinkCursor() {
	videoBuffer[cursorY][cursorX].style = 0x77 ^ videoBuffer[cursorY][cursorX].style;
	writeStyle(cursorX, cursorY, videoBuffer[cursorY][cursorX].style);
	if(cursorVisible)
		cursorVisible = FALSE;
	else
		cursorVisible = TRUE;
}

//==============================================================================
//  KEYBOARD
//==============================================================================

void shiftLeft() {
	if(writeIndex != startIndex) {
		uint16_t from = writeIndex-1;
		if(from < 0)
			from = BUFFER_SIZE;
		uint16_t to;
		while(from != endIndex) {
			to = from;
			from++;
			if(from == BUFFER_SIZE)
				from = 0;
			kbBuffer[to] = kbBuffer[from];
		}
	}
}

void shiftRight() {
	if(writeIndex != endIndex) {
		uint16_t from = endIndex;
		uint16_t to;
		do {
			to = from;
			from--;
			if(from < 0)
				from = BUFFER_SIZE;
			kbBuffer[to] = kbBuffer[from];
		}	while(from != writeIndex);
	}
}

void writeBuffer(char ch) {
	switch (ch) {
		case '\b':
			if(writeIndex != startIndex) {
				shiftLeft();
				writeIndex--;
				if(writeIndex < 0)
					writeIndex = BUFFER_SIZE;
				endIndex--;
				if(endIndex < 0)
					endIndex = BUFFER_SIZE;
				size--;
				if(echo)
					printc(ch);
			}
			break;
		case '\n':
			kbBuffer[endIndex] = ch;
			endIndex++;
			size++;
			if(endIndex == BUFFER_SIZE)
				endIndex = 0;
			writeIndex = startIndex = endIndex;
			if(echo)
				printc(ch);
			break;
		default:
			if(size < BUFFER_SIZE-1) {					//Dejar un espacio para \n
				shiftRight();
				kbBuffer[writeIndex] = ch;
				writeIndex++;
				if(writeIndex == BUFFER_SIZE)
					writeIndex = 0;
				endIndex++;
				if(endIndex == BUFFER_SIZE)
					endIndex = 0;
				size++;
			}
			if(echo)
				printc(ch);
			break;
	}
}

char readBuffer() {
	char ch = 0;
	if(readIndex < startIndex) {
		ch = kbBuffer[readIndex];
		readIndex++;
		if(readIndex == BUFFER_SIZE)
			readIndex = 0;
		size--;
	}
	return ch;
}

void keyboardLeft() {
	if(writeIndex != startIndex) {
		writeIndex--;
		if(writeIndex < 0)
			writeIndex = BUFFER_SIZE;
		if(echo)
			cursorLeft();
	}
}

void keyboardRight() {
	if(writeIndex != endIndex) {
		writeIndex++;
		if(writeIndex == BUFFER_SIZE)
			writeIndex = 0;
		if(echo)
			cursorRight();
	}
}

void setEcho(uint8_t boolean) {
	echo = boolean;
}
