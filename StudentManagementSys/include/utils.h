#ifndef UTILS_H
#define UTILS_H
#include "major.h"

void clearInputBuffer();
void pauseAndClear();

bool fileExist(const char *path); 
bool createDirectory(const char *path);
Date getCurrentDate();
#endif // UTILS_H
