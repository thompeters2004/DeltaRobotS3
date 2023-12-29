#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

class Coordinates
{
    public:
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;
    int16_t w = 0;
    bool GripperStatus = 0;
    int8_t ObjectType = 0;

    void operator=(const Coordinates& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z; 
        w = rhs.w;
        GripperStatus = rhs.GripperStatus;
        ObjectType = rhs.ObjectType;
    }
    
    private:
    friend bool operator==(const Coordinates & lhs, const Coordinates & rhs)
    {
        return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w));
    }

     friend bool operator!=(const Coordinates & lhs, const Coordinates & rhs)
    {
        return ((lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z) || (lhs.w != rhs.w));
    }
};

#include "Gcode.h"
#include "EtherCATConfig.h"
#include "EtherCATFunction.h"


#endif