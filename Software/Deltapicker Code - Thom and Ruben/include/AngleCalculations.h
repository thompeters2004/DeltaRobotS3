/**
 * @file AngleCalculations.h
 * @author Vincent de Man   (V.deMan@student.han.nl)
 *         Jordi Pluk       (Jordi.pluk@han.nl & JPW.Pluk@student.han.nl)
 *         Thom Peters      (T.Peters13@student.han.nl)
 *         Ruben Konings    (RL.Konings@student.han.nl)
 * @brief
 * @version 4.0
 * @date 15-11-2023
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ANGLE_CALCS_H
#define ANGLE_CALCS_H
#include <Arduino.h>
#include <math.h>
#include <stdint.h>

//Typedef for returning local angles

/**
 * @brief Struct with resulting Theta calculations
 * 
 */
typedef struct ThetaResults
{
    double ThetaX = 0;
    double ThetaY = 0;
    double ThetaZ = 0;
} ThetaResults;

typedef struct GripperValues
{
    int GripperDesiredState = 0;
    int GripperObject = 0;
} GripperValues;
/**
 * @brief Coordinate class to easily use x,y,z coordinates
 * 
 */
class Coordinates
{
    public:
    Coordinates(); 
    Coordinates(int _X, int _Y, int _Z);
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;

    // Coordinates& operator=(const Coordinates & rhs)
    // {
    //     if (this == &rhs)
    //     {
    //     return *this;
    //     } 
    //     x = rhs.x;
    //     y = rhs.y;
    //     x = rhs.x; 
        
    //     return *this;  
    // }

    void operator=(const Coordinates& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z; 
    }
    
    private:
    friend bool operator==(const Coordinates & lhs, const Coordinates & rhs)
    {
        return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z));
    }

     friend bool operator!=(const Coordinates & lhs, const Coordinates & rhs)
    {
        return ((lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z));
    }


    
};// Coordinates;


/**
 * @brief Function to calculate the Theta between the current and desired coordintates.
 * 
 * @param _CurrentCoordinates The current coordinates of the deltapicker
 * @param _CoordinateGoal The goal coordinates
 * @return ThetaResults The resulting Theta
 * 
 * @details This function automatically calculates the theta needed to move from 
 * the current coordinates to the desired coordinates.
 */
ThetaResults Calculate(Coordinates _CurrentCoordinates, Coordinates _CoordinateGoal);

/**
 * @brief Function to calculate the delta of the current angle
 * 
 * @param X0 X coordinate
 * @param Y0 Y coordinate
 * @param Z0 Z coordinate
 * @return ThetaResults Structure containing the theta's delta of the motors
 * 
 * @details This function calculates the theta delta of the current angles
 * Then it will return a structure containing the theta values for every motor.
 */
ThetaResults CoordinatesToDelta(double X0, double Y0, double Z0);

/**
 * @brief Function to calculate the motor angle
 * 
 * @param X00 X coordinate
 * @param Y00 Y coordinate
 * @param Z00 Z coordinate
 * 
 * @return double The theta of the motor
 */
double CalculateAngles(double X00, double Y00, double Z00);

//constant values for sin120 and cos120 used in calculations
const double sin120 = 0.8660254;
const double cos120 = -0.5;

#endif