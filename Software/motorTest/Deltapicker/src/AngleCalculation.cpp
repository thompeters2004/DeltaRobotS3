/**
 * @file AngleCalculation.cpp
 * @author Vincent de Man (V.deMan@student.han.nl)  &   Jordi Pluk (Jordi.pluk@han.nl & JPW.Pluk@student.han.nl)
 * @brief 
 * @version 3.0
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "AngleCalculations.h"


// Measurement
double Rb = 125; // Radius Base
double Rh = 58;  // Radius hand
double B = 152;  // Bicep Length
double F = 323;  // Forearm leng th

ThetaResults Calculate(Coordinates _CurrentCoordinates, Coordinates _CoordinateGoal)
{
  ThetaResults Results;
  ThetaResults CurrentMotorAngle;

  Results = CoordinatesToDelta((double)_CurrentCoordinates.x, (double)_CurrentCoordinates.y, (double)_CurrentCoordinates.z); // What is the current position of the motors

    Serial.println("Current1");
    Serial.println(_CurrentCoordinates.x);
    Serial.println(_CurrentCoordinates.y);
    Serial.println(_CurrentCoordinates.z);

    Serial.println("Desired");
    Serial.println(_CoordinateGoal.x);
    Serial.println(_CoordinateGoal.y);
    Serial.println(_CoordinateGoal.z);

    Serial.println("Theta");
    Serial.println(Results.ThetaX);
    Serial.println(Results.ThetaY);
    Serial.println(Results.ThetaZ);

  CurrentMotorAngle.ThetaX = Results.ThetaX; // Set current angles
  CurrentMotorAngle.ThetaY = Results.ThetaY;
  CurrentMotorAngle.ThetaZ = Results.ThetaZ;

  Results = CoordinatesToDelta((double)_CoordinateGoal.x, (double)_CoordinateGoal.y, (double)_CoordinateGoal.z); // What is the wanted position for the motors

    Serial.println("Current2");
    Serial.println(_CurrentCoordinates.x);
    Serial.println(_CurrentCoordinates.y);
    Serial.println(_CurrentCoordinates.z);

    Serial.println("Desired");
    Serial.println(_CoordinateGoal.x);
    Serial.println(_CoordinateGoal.y);
    Serial.println(_CoordinateGoal.z);

    Serial.println("Theta");
    Serial.println(Results.ThetaX);
    Serial.println(Results.ThetaY);
    Serial.println(Results.ThetaZ);

  Results.ThetaX = CurrentMotorAngle.ThetaX - Results.ThetaX;
  Results.ThetaY = CurrentMotorAngle.ThetaY - Results.ThetaY;
  Results.ThetaZ = CurrentMotorAngle.ThetaZ - Results.ThetaZ;
    Serial.println("Current3");
    Serial.println(_CurrentCoordinates.x);
    Serial.println(_CurrentCoordinates.y);
    Serial.println(_CurrentCoordinates.z);

    Serial.println("Desired");
    Serial.println(_CoordinateGoal.x);
    Serial.println(_CoordinateGoal.y);
    Serial.println(_CoordinateGoal.z);

    Serial.println("Theta");
    Serial.println(Results.ThetaX);
    Serial.println(Results.ThetaY);
    Serial.println(Results.ThetaZ);
  return Results; // Difference in angles
}

ThetaResults CoordinatesToDelta(double X0, double Y0, double Z0)
{
  ThetaResults Results;

  Results.ThetaX = CalculateAngles(X0, Y0, Z0);
  //                        Rotate coordinates +120 degrees when looking top down for Y motor
  Results.ThetaY = CalculateAngles((X0 * cos120 + Y0 * sin120), (Y0 * cos120 - X0 * sin120), Z0);
  //                        Rotate coordinates -120 degrees when looking top down for Z motor
  Results.ThetaZ = CalculateAngles((X0 * cos120 - Y0 * sin120), (Y0 * cos120 + X0 * sin120), Z0);
  
  return Results;
}

double CalculateAngles(double X00, double Y00, double Z00)
{
  double Y1 = -0.5 * 0.57735 * Rb; // f/2 * tan(30 deg)
  Y00 -= 0.5 * 0.57735 * Rh;       // shift center to edge

  double a = (pow(X00, 2) + pow(Y00, 2) + pow(Z00, 2) + pow(B, 2) - pow(F, 2) - pow(Y1, 2)) / (2.0 * Z00); // z = a + b*y
  double b = (Y1 - Y00) / Z00;

  double d = -(a + b * Y1) * (a + b * Y1) + B * (b * b * B + B); // discriminant

  double Yj = (Y1 - a * b - sqrt(d)) / (b * b + 1); // choosing outer povar
  double Zj = a + b * Yj;

  double Angle = atan(-Zj / (Y1 - Yj)) * 180 / 3.14159 + ((Yj > Y1) ? 180 : 0);
  double ReverseAngle = 90.000 - Angle;

  return ReverseAngle;
}


Coordinates::Coordinates(int _X, int _Y, int _Z) : x(_X), y(_Y), z(_Z)
{
      
}
Coordinates::Coordinates()
{
  x = 0; 
  y = 0; 
  z = 0; 
}



