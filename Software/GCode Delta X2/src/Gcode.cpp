#include <Gcode.h>
#include <string.h>

void easy_convert(Coordinates &NewValues, Coordinates &CompareValues)
{
    static char buf[200];
                         //0bXYZW what needs to be updated
    uint16_t SelectPrint = 0b0000;

    if(NewValues.x != CompareValues.x) SelectPrint |= 0b1000;
    if(NewValues.y != CompareValues.y) SelectPrint |= 0b0100;
    if(NewValues.z != CompareValues.z) SelectPrint |= 0b0010;
    if(NewValues.w != CompareValues.w) SelectPrint |= 0b0001;

    int skip = 0;

    switch (SelectPrint){
        case 0b0001:
            sprintf(buf,"G01 W%d",NewValues.w);    
            break;
        case 0b0010:
            sprintf(buf,"G01 Z%d",NewValues.z);    
            break;
        case 0b0011:
            sprintf(buf,"G01 Z%d W%d",NewValues.z, NewValues.w);    
            break;
        case 0b0100:
            sprintf(buf,"G01 Y%d",NewValues.y);    
            break;
        case 0b0101:
            sprintf(buf,"G01 Y%d W%d",NewValues.y, NewValues.w);    
            break;
        case 0b0110:
            sprintf(buf,"G01 Y%d Z%d",NewValues.y, NewValues.z);    
            break;
        case 0b0111:
            sprintf(buf,"G01 Y%d Z%d W%d",NewValues.y, NewValues.z, NewValues.w);    
            break;
        case 0b1000:
            sprintf(buf,"G01 X%d",NewValues.x);    
            break;
        case 0b1001:
            sprintf(buf,"G01 X%d W%d",NewValues.x, NewValues.w);    
            break;
        case 0b1010:
            sprintf(buf,"G01 X%d Z%d",NewValues.x, NewValues.z);   
            break;
        case 0b1011:
            sprintf(buf,"G01 X%d Z%d W%d",NewValues.x, NewValues.z, NewValues.w); 
            break;
        case 0b1100:
            sprintf(buf,"G01 X%d Y%d",NewValues.x, NewValues.y);  
            break;            
        case 0b1101:
            sprintf(buf,"G01 X%d Y%d W%d",NewValues.x, NewValues.y, NewValues.w);  
            break;
        case 0b1110:
            sprintf(buf,"G01 X%d Y%d Z%d",NewValues.x, NewValues.y, NewValues.z); 
            break;
        case 0b1111:
            sprintf(buf,"G01 X%d Y%d Z%d W%d",NewValues.x, NewValues.y, NewValues.z, NewValues.w);  
            break;            
        default:
            skip = 1;
            break;
    }

    //send string
    if(!skip){
        sendGcode(buf);
    }

    Serial.flush();

    CompareValues = NewValues;
}

void sendGcode(String Command)
{
    Serial.println(Command);
}
/*
Coordinates readGCode()
{ // Check positions and Gripper status
    Coordinates Returndata;
    sendGcode("Position");
    delay(500);
    while (Serial.available() > 0)
    {
        Serial.read();                                        // Remove X
        Returndata.x = (Serial.readStringUntil(' ').toInt()); // writes in the string all the inputs till a space
        Serial.read();
        Serial.read();
        Returndata.y = (Serial.readStringUntil(' ').toInt()); // writes in the string all the inputs till a space
        Serial.read();
        Serial.read();
        Returndata.z = (Serial.readStringUntil(' ').toInt()); // writes in the string all the inputs till a space
        Serial.read();
        Serial.read();
        Returndata.w = (Serial.readStringUntil('\n').toInt()); // writes in the string all the inputs till the end of line character
    }

    return Returndata;
}

*/