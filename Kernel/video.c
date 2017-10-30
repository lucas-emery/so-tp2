#include <video.h>
#include <terminal.h>


static video_row *video = (video_row*) 0xB8000;
static uint8_t cursorX = 0;
static uint8_t cursorY = 0;


void writeScreen(uint8_t x, uint8_t y, char ch, char style) {
	if(ch == 0)
		video[y][x].ch = ' ';
	else
		video[y][x].ch = ch;

	video[y][x].style = style;
}

void writeAtCursor(char ch, char style) {
	if(ch == 0)
		video[cursorY][cursorX].ch = ' ';
	else
		video[cursorY][cursorX].ch = ch;

	video[cursorY][cursorX].style = style;
}

void updateCursor(uint8_t x, uint8_t y) {
	cursorX = x;
	cursorY = y;
}

void writeStyle(uint8_t x, uint8_t y, char style) {
	video[y][x].style = style;
}

void setScreen(video_row* buffer) {
	memcpy(video[0], buffer[0], CELLSIZE*WIDTH*HEIGHT);
}
