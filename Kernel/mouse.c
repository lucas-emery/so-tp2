#include <mouse.h>
#include <lib.h>
#include <keyboard.h>
#include <terminal.h>
#include <video.h>

#define DELAY 15

typedef struct {
	uint8_t x;
	uint8_t y;
} position;

typedef struct {
	position start;
	position end;
} selection;

static signed char info[3];
static uint8_t byte = 0;

static char clipboard[WIDTH*HEIGHT + HEIGHT];
static int clipboardIndex = 0;

static uint8_t left = FALSE;
static uint8_t right = FALSE;

static position cursor = {0,0};
static selection selected;

static int counterX = 0;
static int counterY = 0;

/*From http://houbysoft.com/download/ps2mouse.html*/
uint8_t mouseRead() {
	mouseWait(0);
	return readPort(0x60);
}

void mouseWrite(uint8_t write) {
	mouseWait(1);
	writePort(0x64, 0xD4);
	mouseWait(1);
	writePort(0x60, write);
}

void initializeMouse() {
	uint8_t status_byte;

	mouseWait(1);
	writePort(0x64,0xA8);
	mouseWait(1);
	writePort(0x64,0x20);
	mouseWait(0);
	status_byte = (readPort(0x60) | 2);
	mouseWait(1);
	writePort(0x64, 0x60);
	mouseWait(1);
	writePort(0x60, status_byte);
	mouseWrite(0xF6);
	mouseRead();
	mouseWrite(0xF4);
	mouseRead();
}

void mouseWait(uint8_t type)
{
  unsigned int timeOut=100000;
  if(type==0)
  {
    while(timeOut--) //Data
    {
      if((readPort(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(timeOut--) //Signal
    {
      if((readPort(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouseHandler() {
	uint8_t hasMoved = FALSE;
	if(readPort(0x64) & 0x20)
		info[byte++] = mouseRead();

	if(byte == 3) {
		byte = 0;

		if(info[0] & 0x80 || info[0] & 0x40) {
			return;
		}

		if((info[0] & 0x1) && !left)  {
			selected.start.x = cursor.x;
			selected.start.y = cursor.y;
			left = TRUE;
		}
		if(!(info[0] & 0x1) && left) {
      		selected.end.x = cursor.x;
			selected.end.y = cursor.y;
			updateMouse(cursor.x, cursor.y);
			clipboardIndex = 0;
      		copySelection();
			left = FALSE;
		}

		if((info[0] & 0x2) && !right)  {
			for(int i = 0; i<clipboardIndex; i++)
				writeBuffer(clipboard[i]);
			mouseWait(0);
			right = TRUE;
		}
		if(!(info[0] & 0x2) && right)
			right = FALSE;

		if (info[1] != 0 || info[2] != 0) {

			counterX += info[1];
			counterY += info[2];

			if(counterX >= DELAY || counterX <= -DELAY) {
				counterX/=DELAY;
				if(cursor.x + counterX <= WIDTH-1 && cursor.x + counterX >= 0) {
	            	cursor.x += counterX;
	            	hasMoved = TRUE;
				}
				counterX = 0;
			}

			if(counterY >= DELAY || counterY <= -DELAY) {
				counterY/=DELAY;
	            if(cursor.y - counterY<= HEIGHT-1 && cursor.y - counterY>= 0) {
	            	cursor.y -= counterY;
	            	hasMoved = TRUE;
	            }
	            counterY = 0;
	        }
            if(hasMoved) {
            	if(!left)
            		updateMouse(cursor.x, cursor.y);
            	else
            		selectTo(cursor.x, cursor.y);
            	mouseWait(0);
			}
		}
	}
}

void copySelection() {
	selection copy;
    int x, y;

    if(selected.start.x > selected.end.x) {
        copy.start.x = selected.end.x;
        copy.end.x = selected.start.x;
    }
    else {
        copy.start.x = selected.start.x;
        copy.end.x = selected.end.x;
    }

    if(selected.start.y > selected.end.y) {
        copy.start.y = selected.end.y;
        copy.end.y = selected.start.y;
    }
    else {
        copy.start.y = selected.start.y;
        copy.end.y = selected.end.y;
    }

    x = copy.start.x;
    y = copy.start.y;

    while(y <= copy.end.y) {
        while(x <= copy.end.x) {
            clipboard[clipboardIndex++] = getCharAt(x, y);
        	x++;
        }
        y++;
        x = copy.start.x;
       	clipboard[clipboardIndex++] = '\n';
    }
    mouseWait(0);
}
