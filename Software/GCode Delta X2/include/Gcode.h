#ifndef GCODE_H
#define GCODE_H

#include <main.h>

void easy_convert (Coordinates &NewValues, Coordinates &CompareValues);
void sendGcode(String Command);
/*
String ConversetoGCode(Coordinates Values);



Coordinates readGCode();
*/
#endif