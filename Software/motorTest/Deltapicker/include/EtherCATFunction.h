/**
 * @file EtherCATFunctions.h
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
#ifndef EASYCATFUNCTION_H
#define EASYCATFUNCTION_H


#include <Arduino.h>
#include "main.h"

/**
 * @brief Function to intialize the EtherCAT board
 * 
 * @return true 
 * @return false 
 */
bool InitializeEtherCAT();

/**
 * @brief Function to get new EtherCAT data from the shield
 * 
 * @details The EtherCAT shield handles the EtherCAT communication
 * To get the in/output buffers from the shield this function should be called.
 */
void EtherCATCycleUpdate();

/**
 * @brief Function to get a Byte from EtherCAT
 * 
 * @param Index The Index of the byte
 * @return uint8_t The value of the byte at the index
 * 
 * @details This function should be used to get the value of a byte.
 * The function reads from BufferOut. 
 */
uint8_t EtherCATGetByte(int Index);

/**
 * @brief Function to set a EtherCAT byte
 * 
 * @param Index The Index of the byte
 * @param Value The value of the byte
 * 
 * @details Function to set a EtherCAT byte in the buffer.
 * The byte will be sent cyclicly when EtherCATCycleUpdate() has been called. 
 */
void EtherCATSetByte(int Index, uint8_t Value);

/**
 * @brief Function to get the value of a EtherCAT bit
 * 
 * @param Index The index of the Byte
 * @param bitnr The bitnrm counted from LSB
 * @return true Bit is a logic one
 * @return false Bit is a logic zero
 * 
 * @details This function takes in the index of the byte and the bitnumber.
 * The bitnumber is the number of the bit counted from the LSB.
 * The function returns a bool that indicates a logic one or zero. 
 */
bool EtherCATGetBIT(int Index, uint8_t bitnr);

/**
 * @brief Function to set a specific EtherCAT bit
 * 
 * @param Index The index of the byte that will be set
 * @param bitnr The bitnumber of the bit that will be set counted from LSB
 * @param Value The new bit value. Logic zero or one
 * 
 * @details Function to set a secific EtherCAT bit. 
 * The function takes in the byte index and the bitnumber.
 * Note that the value will only be sent cyclicly over EtherCAT 
 * after EtherCATCycleUpdate() has been called.
 */
void EtherCATSetBIT(int Index, uint8_t bitnr, bool Value);

/**
 * @brief Function to update the deltapicker EtherCAT variables
 * 
 * @param CurrentCoordinates The current coordinates of the deltapicker
 * @param CurrentSpeed The current speed the deltapicker is running at
 * @param DeltaPickerEnabled Bit if the deltapicker is enabled
 * 
 * @details This function automatically sets all the EtherCat variables that are associated with the deltapicker
 * Call this function to set the variables then call EtherCATCyleUpdate() to cyclicly send the data.
 */
void UpdateEtherCAT_Deltapicker(Coordinates CurrentCoordinates, uint8_t CurrentSpeed, bool DeltaPickerEnabled);

/**
 * @brief Function to update the EtherCAT variables associated with the gripper
 * 
 * @param GripperEnabled Bool if the gripper is enabled
 * @param MaxGripperSpeed The maximum speed of the gripper
 * @param PickerState The state of the picker
 * @param GripperAttached Bit if the gripper is attached
 * @param GripperCalibrationDone Bit if the gripper is done calibrated
 * @param GripperInstructionsDone Bit if the gripper instructions are done
 * 
 * @details Function to update the EtherCAT variables associated with the gripper. 
 * This function updates the gripper variables. Call EtherCATCyleUpdate()
 * to cylcicly send the variables.
 */
void UpdateEtherCAT_Gripper(bool GripperEnabled, uint8_t MaxGripperSpeed, uint8_t PickerState, bool GripperAttached, bool GripperCalibrationDone, bool GripperInstructionsDone);

#endif