/**
 * @file EtherCATFunction.cpp
 * @author Vincent de Man (V.deMan@student.han.nl)  &   Jordi Pluk (Jordi.pluk@han.nl & JPW.Pluk@student.han.nl)
 * @brief 
 * @version 3.0
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "EtherCATFunction.h"
#include <EasyCAT.hpp>


static EasyCAT EtherCatController(A5);

void DeltaPickerGetDesiredCoordinates(Coordinates &ReturnCoordinates)
{
  ReturnCoordinates.x = (((int16_t)(EtherCATGetByte(XcoordMSB)) << 8) | (EtherCATGetByte(XcoordLSB)));
  ReturnCoordinates.y = (((int16_t)(EtherCATGetByte(YcoordMSB)) << 8) | (EtherCATGetByte(YcoordLSB)));
  ReturnCoordinates.z = (((int16_t)(EtherCATGetByte(ZcoordMSB)) << 8) | (EtherCATGetByte(ZcoordLSB)));
  ReturnCoordinates.w = (((int16_t)(EtherCATGetByte(WcoordMSB)) << 8) | (EtherCATGetByte(WcoordLSB)));
  ReturnCoordinates.GripperStatus = (int8_t)((EtherCATGetByte(7) & 0x60) >> 5);
  ReturnCoordinates.ObjectType = (int8_t)((EtherCATGetByte(7) & 0x1C) >> 2);
}

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

void UpdateEtherCAT_Deltapicker(Coordinates CurrentCoordinates, bool DeltaPickerEnabled)
{
    //Set the Deltapicker speed
    //EtherCATSetByte(DeltaPickerReturnSpeedByte, CurrentSpeed); 
    EtherCATSetBIT(DeltaPickerEnabledByte,DeltaPickerEnabledBit,DeltaPickerEnabled);
    EtherCATSetByte(XcurrentCoordMSB,CurrentCoordinates.x >> 8);
    EtherCATSetByte(XcurrentCoordLSB,CurrentCoordinates.x);
    
    EtherCATSetByte(YcurrentCoordMSB,CurrentCoordinates.y >> 8);
    EtherCATSetByte(YcurrentCoordLSB,CurrentCoordinates.y);
    
    EtherCATSetByte(ZcurrentCoordMSB,CurrentCoordinates.z >> 8);
    EtherCATSetByte(ZcurrentCoordLSB,CurrentCoordinates.z);

    EtherCATSetByte(WcurrentCoordMSB,CurrentCoordinates.w >> 8);
    EtherCATSetByte(WcurrentCoordLSB,CurrentCoordinates.w);
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

