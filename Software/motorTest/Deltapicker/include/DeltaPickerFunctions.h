/**
 * @file DeltaPickerFunctions.h
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

#ifndef DELTAPICKER_FUNCTIONS_H
#define DELTAPICKER_FUNCTIONS_H

#include "main.h"
#include <Arduino.h>
#include "AccelStepper.h"

#define LIMIT_SWITCH_PIN_X A3 //5
#define LIMIT_SWITCH_PIN_Y 8
#define LIMIT_SWITCH_PIN_Z 2 //A3

#define LimitSwitchGripperPin A2 //A4
#define STEP_PIN_GRIPPER A0 //4 
#define DIRECTION_PIN_GRIPPER A1 //A5

#define STEP_PIN_X 4 //2
#define STEP_PIN_Y 6
#define STEP_PIN_Z A5 //9


#define DIRECTION_PIN_X 3
#define DIRECTION_PIN_Y 7
#define DIRECTION_PIN_Z A4 //A2


#define ENABLE_DELTAPICKERMOTORS_PIN 0

const double DEGREES_PER_STEP = 0.05625;

/**
 * @brief Function to update the max speed of the deltapicker
 * 
 * @param NewSpeed The new speed 
 * 
 * @details This function will set the new speed of the deltapicker
 */
void DeltaPickerUpdateMaxSpeed(uint8_t NewSpeed);

/**
 * @brief Function to get the desired deltapicker coordinates from EtherCAT
 * 
 * @return Coordinates The goal coordinates from EtherCAT
 * 
 * @details This function returns the desired coordinates from EtherCAT.
 * Note that the function will only return new coordinates after EtherCATCycleUpdate() 
 * has been called. 
 */
void DeltaPickerGetDesiredCoordinates(Coordinates &ReturnCoordinates);

/**
 * @brief Function to start the calibration of the deltapicker
 * 
 * 
 * @details This function will calibrate the deltapicker using the limit switches
 * The function will move the arms up until it hits the limit switches. Then
 * we have a known position and can move the deltapicker to the default position.
 */
void DeltaPickerCalibrate(void);
void SECONDDeltaPickerCalibrate(void);
/**
 * @brief Function to move the deltapicker to a coordinate
 * 
 * @param _CurrentCoordinates The current coordinate of the deltapicker
 * @param _CoordinateGoal The desired coordinate where the deltapicker will move to
 * 
 * @details This function will move the deltapicker to a new coordinate. 
 */
void MoveToCoordinates(Coordinates _CurrentCoordinates, Coordinates _CoordinateGoal);

/**
 * @brief Function to check if the desired coordinates are in scope
 * 
 * @param _DesiredCoordinates The coordinates to check
 * @return true Coordinates are in range of the deltapicker
 * @return false Coordinates are not in range of the deltapicker
 */
bool DesiredCoordinateInScope(Coordinates _DesiredCoordinates);

/**
 * @brief Get the desired values from EtherCAT
 * 
 * @param _Grippervalue Structure with gripper data that will be filled
 *
 * @details This function will get the desired gripper values from etherCAT
 */
void GripperGetDesiredValues(GripperValues &_Grippervalue);

/**
 * @brief Function to adjust the gripper with the new values
 * 
 * @details Call this function to move the gripper to the desired 
 * Values received from EtherCAT. 
 */
void AdjustGripper();
/**
 * @brief Function to test motor slip 
 * 
 * @details Call this function to move the motors the same time to see if they slip
 */
void TestMotors();

#endif // DELTAPICKER_FUNCTIONS_H
