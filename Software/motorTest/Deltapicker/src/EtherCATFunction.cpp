/**
 * @file EtherCATFunction.cpp
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
#include "EtherCATFunction.h"
#include <EasyCAT.hpp>

static EasyCAT EtherCatController(10); //A5

bool InitializeEtherCAT()
{ 
    return EtherCatController.Init() ;
}

void EtherCATCycleUpdate()
{
    EtherCatController.MainTask(); // execute the EtherCatController task
}

uint8_t EtherCATGetByte(int Index)
{
    return EtherCatController.BufferOut.Byte[Index];
}

void EtherCATSetByte(int Index, uint8_t Value)
{
    EtherCatController.BufferIn.Byte[Index] = Value;
}


bool EtherCATGetBIT(int Index, uint8_t bitnr)  //From LSB
{
    uint8_t byte = EtherCATGetByte(Index);
    return (byte >> bitnr) & 1U;
}

void EtherCATSetBIT(int Index, uint8_t bitnr, bool Value)
{
    uint8_t Byte = EtherCATGetByte(Index);
    Byte = (Byte & ~(1U << bitnr)) | (Value << bitnr); // zoek het uit, DIT WERKT, NIET TWIJFELEN, ANDERS VRAAG NAAR JORDI PLUK
    EtherCATSetByte(Index, Byte);
}

void UpdateEtherCAT_Deltapicker(Coordinates CurrentCoordinates, uint8_t CurrentSpeed, bool DeltaPickerEnabled)
{
    //Set the Deltapicker speed
    EtherCATSetByte(DeltaPickerReturnSpeedByte, CurrentSpeed); 
    EtherCATSetBIT(DeltaPickerEnabledByte,DeltaPickerEnabledBit,DeltaPickerEnabled);
    EtherCATSetByte(XcurrentCoordMSB,CurrentCoordinates.x >> 8);
    EtherCATSetByte(XcurrentCoordLSB,CurrentCoordinates.x);
    
    EtherCATSetByte(YcurrentCoordMSB,CurrentCoordinates.y >> 8);
    EtherCATSetByte(YcurrentCoordLSB,CurrentCoordinates.y);
    
    EtherCATSetByte(ZcurrentCoordMSB,CurrentCoordinates.z >> 8);
    EtherCATSetByte(ZcurrentCoordLSB,CurrentCoordinates.z);
}

void UpdateEtherCAT_Gripper(bool GripperEnabled, uint8_t MaxGripperSpeed, uint8_t PickerState, bool GripperAttached, bool GripperCalibrationDone, bool GripperInstructionsDone)
{
    EtherCATSetByte(ReturnSpeedPickerByte, MaxGripperSpeed);
    EtherCATSetBIT(EnabledPickerMotorByte, EnabledPickerMotorBit, GripperEnabled);

    EtherCATSetByte(PickerReturnStateByte, PickerState<<PickerReturnStateBitLSB);
    EtherCATSetBIT(PickerReturnAttachedByte, PickerReturnAttachedBit, GripperAttached);

    EtherCATSetBIT(CalibrationDoneByte, CalibrationDoneBit, GripperCalibrationDone);

    EtherCATSetBIT(InstructionsDoneByte, InstructionsDoneBit, GripperInstructionsDone);
}

