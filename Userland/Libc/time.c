#include <stdint.h>

extern void int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

typedef struct {
  int hours;
  int minutes;
  int seconds;
} time;

typedef struct {
  int day;
  int month;
  int year;
} date;

static time currentTime;
static date currentDate;

void updateTime() {
  int80(4, (uint64_t) &currentTime.hours, (uint64_t) &currentTime.minutes, (uint64_t) &currentTime.seconds);
}

void updateDate() {
  int80(5, (uint64_t) &currentDate.day, (uint64_t) &currentDate.month, (uint64_t) &currentDate.year);
}

int getHour() {
  updateTime();
  return currentTime.hours;
}

int getMinutes() {
  updateTime();
  return currentTime.minutes;
}

int getSeconds() {
  updateTime();
  return currentTime.seconds;
}

int getDay() {
  updateDate();
  return currentDate.day;
}

int getMonth() {
  updateDate();
  return currentDate.month;
}

int getYear() {
  updateDate();
  return currentDate.year;
}

void setTimeZone(int tz) {
  int80(3, (uint64_t) tz,0,0);
}
