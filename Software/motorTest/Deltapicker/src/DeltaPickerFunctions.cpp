/**
 * @file DeltaPickerFunctions.cpp
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
#include "DeltaPickerFunctions.h"

static AccelStepper MotorX(AccelStepper::DRIVER, STEP_PIN_X, DIRECTION_PIN_X);
static AccelStepper MotorY(AccelStepper::DRIVER, STEP_PIN_Y, DIRECTION_PIN_Y);
static AccelStepper MotorZ(AccelStepper::DRIVER, STEP_PIN_Z, DIRECTION_PIN_Z);
static AccelStepper GripperMotor(AccelStepper::DRIVER, STEP_PIN_GRIPPER, DIRECTION_PIN_GRIPPER);
//Mag later weg (grippermotor), schrijft nou nog mijn pinmode goed

uint32_t GripperPositionCounter = 0;

void DeltaPickerUpdateMaxSpeed(uint8_t NewSpeed) // Unconverted Value 0-100
{                                                // 0-100    ->     MAX 3500 for accel driver library     500 - 3000
  float ConvertedSpeed = (500.0f + (25.0f * (float)NewSpeed));

  MotorX.setMaxSpeed(ConvertedSpeed); // NewSpeed is StepsPerSecond
  MotorY.setMaxSpeed(ConvertedSpeed);
  MotorZ.setMaxSpeed(ConvertedSpeed);
  // GripperMotor.setMaxSpeed(ConvertedSpeed);
  MotorX.setAcceleration(3000);
  MotorY.setAcceleration(3000);
  MotorZ.setAcceleration(3000);
  // GripperMotor.setAcceleration(1500);
}

void DeltaPickerGetDesiredCoordinates(Coordinates &ReturnCoordinates)
{
  ReturnCoordinates.x = (((int16_t)(EtherCATGetByte(XcoordMSB)) << 8) | (EtherCATGetByte(XcoordLSB)));
  ReturnCoordinates.y = (((int16_t)(EtherCATGetByte(YcoordMSB)) << 8) | (EtherCATGetByte(YcoordLSB)));
  ReturnCoordinates.z = (((int16_t)(EtherCATGetByte(ZcoordMSB)) << 8) | (EtherCATGetByte(ZcoordLSB)));
}

bool DesiredCoordinateInScope(Coordinates _DesiredCoordinates)
{
  bool check = 0;

  if (_DesiredCoordinates.z > -400)
  {
    if (_DesiredCoordinates.z < -190)
    {
      if (_DesiredCoordinates.x <= 250)
      {
        if (_DesiredCoordinates.x >= -250)
        {
          if (_DesiredCoordinates.y <= 250)
          {
            if (_DesiredCoordinates.y >= -250)
            {
              check = 1;
            }
          }
        }
      }
    }
  }
  if (_DesiredCoordinates.z < -200) check = 1; //Test disable bounds
  return check;
}

void SECONDDeltaPickerCalibrate(void)
{

  uint8_t SetSpeed = 0;
  DeltaPickerUpdateMaxSpeed(SetSpeed);

  int SWXEnabled = 0;
  int SWYEnabled = 0;
  int SWZEnabled = 0;

  bool LimitSwitchX = false, LimitSwitchY = false, LimitSwitchZ = false;
  while (!(SWXEnabled && SWYEnabled && SWZEnabled))
  {
    // Read Limit switches
    LimitSwitchX |= digitalRead(LIMIT_SWITCH_PIN_X); // X Motor
    LimitSwitchY |= digitalRead(LIMIT_SWITCH_PIN_Y); // Y Motor
    LimitSwitchZ |= digitalRead(LIMIT_SWITCH_PIN_Z); // Z Motor

    if (LimitSwitchX)
    {
      SWXEnabled = 1;
    }
    if (LimitSwitchY)
    {
      SWYEnabled = 1;
    }
    if (LimitSwitchZ)
    {
      SWZEnabled = 1;
    }
    Serial.println("Moving");
    MotorX.move(-50);
    MotorY.move(-50);
    MotorZ.move(-50);
    if (!SWXEnabled)
      MotorX.run();
    if (!SWYEnabled)
      MotorY.run();
    if (!SWZEnabled)
      MotorZ.run();

      SetSpeed = EtherCATGetByte(DeltaPickerSpeedByte) & 0x7F;
      DeltaPickerUpdateMaxSpeed(SetSpeed);                  // Without enable 0-100 speed
  }

  // put motors on 90 degrees        Exactly 420 steps! DONT CHANGE
  MotorX.move(420);
  MotorY.move(420);
  MotorZ.move(420);
  while (MotorX.distanceToGo() && MotorY.distanceToGo() && MotorZ.distanceToGo())
  {
    MotorX.run();
    MotorY.run();
    MotorZ.run();
  }

  // save the current 90 degrees position as the 'zero' position of motors
  MotorX.setCurrentPosition(0);
  MotorY.setCurrentPosition(0);
  MotorZ.setCurrentPosition(0);
}

void DeltaPickerCalibrate(void)
{
  int SWXEnabled = 0;
  int SWYEnabled = 0;
  int SWZEnabled = 0;

  MotorX.setPinsInverted(true, true, true);
  MotorY.setPinsInverted(true, true, true);
  MotorZ.setPinsInverted(true, true, true);

  bool LimitSwitchX = false, LimitSwitchY = false, LimitSwitchZ = false;
  while (!(SWXEnabled && SWYEnabled && SWZEnabled))
  {
    // Read Limit switches
    LimitSwitchX |= digitalRead(LIMIT_SWITCH_PIN_X); // X Motor
    LimitSwitchY |= digitalRead(LIMIT_SWITCH_PIN_Y); // Y Motor
    LimitSwitchZ |= digitalRead(LIMIT_SWITCH_PIN_Z); // Z Motor

    if (LimitSwitchX)
    {
      SWXEnabled = 1;
    }
    if (LimitSwitchY)
    {
      SWYEnabled = 1;
    }
    if (LimitSwitchZ)
    {
      SWZEnabled = 1;
    }
    Serial.println("Moving");
    MotorX.move(-500);
    MotorY.move(-500);
    MotorZ.move(-500);
    if (!SWXEnabled)
      MotorX.run();
    if (!SWYEnabled)
      MotorY.run();
    if (!SWZEnabled)
      MotorZ.run();
  }

  // put motors on 90 degrees        Exactly 420 steps! DONT CHANGE
  MotorX.move(420);
  MotorY.move(420);
  MotorZ.move(420);
  while (MotorX.distanceToGo() && MotorY.distanceToGo() && MotorZ.distanceToGo())
  {

    MotorX.run();
    MotorY.run();
    MotorZ.run();
  }

  // save the current 90 degrees position as the 'zero' position of motors
  MotorX.setCurrentPosition(0);
  MotorY.setCurrentPosition(0);
  MotorZ.setCurrentPosition(0);
  /*
for (int i = 0; i < 2; i++){
  // gripper initialization
  Serial.println("Moving in");
  while (!digitalRead(LimitSwitchGripperPin))
  {
    digitalWrite(DIRECTION_PIN_GRIPPER, LOW); // Inwards = low
    digitalWrite(STEP_PIN_GRIPPER, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP_PIN_GRIPPER, LOW);
    delayMicroseconds(50);
  }
  Serial.println("Moving out");
  for (uint16_t i = 0; i < 50000; i++)
  {
    digitalWrite(DIRECTION_PIN_GRIPPER, HIGH); // Move to a 0 position
    digitalWrite(STEP_PIN_GRIPPER, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP_PIN_GRIPPER, LOW);
    delayMicroseconds(50);
  }
  Serial.println("Done Moving out");

  GripperPositionCounter = 0; // Set This as the new 0 position.

}
*/
}

void MoveToCoordinates(Coordinates _CurrentCoordinates, Coordinates _CoordinateGoal)
{
  // Calculate the movement
  ThetaResults AngleDifference;
  AngleDifference = Calculate(_CurrentCoordinates, _CoordinateGoal);
  
  static ThetaResults StepsCarry;
  
  // Move motors
  double StepsXNotRounded = ((double)AngleDifference.ThetaX / DEGREES_PER_STEP) + StepsCarry.ThetaX;
  double StepsYNotRounded = ((double)AngleDifference.ThetaY / DEGREES_PER_STEP) + StepsCarry.ThetaY;
  double StepsZNotRounded = ((double)AngleDifference.ThetaZ / DEGREES_PER_STEP) + StepsCarry.ThetaZ;

  int StepsX, StepsY,StepsZ;

  StepsX = (int)StepsXNotRounded;
  StepsY = (int)StepsYNotRounded;
  StepsZ = (int)StepsZNotRounded;

  StepsCarry.ThetaX = StepsXNotRounded - (double)StepsX;
  StepsCarry.ThetaY = StepsYNotRounded - (double)StepsY;
  StepsCarry.ThetaZ = StepsZNotRounded - (double)StepsZ;

  MotorX.move(StepsX);
  MotorY.move(StepsY);
  MotorZ.move(StepsZ);

  Serial.print("Motor X Steps : ");
  Serial.println(MotorX.distanceToGo());

  Serial.print("Motor Y Steps : ");
  Serial.println(MotorY.distanceToGo());

  Serial.print("Motor Z Steps : ");
  Serial.println(MotorZ.distanceToGo());

  while (MotorX.distanceToGo() || MotorY.distanceToGo() || MotorZ.distanceToGo())
  {
    MotorX.run();
    MotorY.run();
    MotorZ.run();
  }
}
void TestMotors()
{
  delay(2000);
  for (size_t i = 0; i < 100; i++)
  {
    MotorX.move(1000);
    MotorY.move(1000);
    MotorZ.move(1000);
    while (MotorX.distanceToGo() && MotorY.distanceToGo() && MotorZ.distanceToGo())
    {

      MotorX.run();
      MotorY.run();
      MotorZ.run();
    }
    
    MotorX.move(-1000);
    MotorY.move(-1000);
    MotorZ.move(-1000);
    while (MotorX.distanceToGo() && MotorY.distanceToGo() && MotorZ.distanceToGo())
    {

      MotorX.run();
      MotorY.run();
      MotorZ.run();
    }
  }
  

}

// void GripperGetDesiredValues(GripperValues &_Grippervalue)
// {
//   // From EtherCAT inputs, Byte7 get bits 5,6 and place them in the struct
//   _Grippervalue.GripperDesiredState = (int8_t)((EtherCATGetByte(7) & 0x60) >> 5);

//   // From EtherCAT inputs, Byte 7 get bits 2,3,4 and place them in the struct
//   _Grippervalue.GripperObject = (int8_t)((EtherCATGetByte(7) & 0x1C) >> 2);
// }

// void AdjustGripper()
// {
//   GripperValues _Grippervalue;
//   uint16_t GripperPositionPeg = 45000;
//   uint16_t GripperPositionRing = 26000;
//   GripperGetDesiredValues(_Grippervalue);
//   if (_Grippervalue.GripperDesiredState)
//   { // Close grippper
//     if (_Grippervalue.GripperObject == 1)
//     {
//       if (GripperPositionCounter != GripperPositionPeg && GripperPositionCounter != GripperPositionRing) 
//       {                                    // Move to pick up PEG
//         for (uint16_t i = 0; i < (GripperPositionPeg - GripperPositionCounter); i++)
//         {                                           
//           digitalWrite(DIRECTION_PIN_GRIPPER, LOW); 
//           digitalWrite(STEP_PIN_GRIPPER, HIGH);
//           delayMicroseconds(5);
//           digitalWrite(STEP_PIN_GRIPPER, LOW);
//           delayMicroseconds(5);
//         }
//         GripperPositionCounter = GripperPositionPeg;
//       }
//     }
//     if (_Grippervalue.GripperObject == 2)
//     {
//       if (GripperPositionCounter != GripperPositionPeg && GripperPositionCounter != GripperPositionRing)
//       {                                                          
//         for (uint16_t i = 0; i < (GripperPositionRing - GripperPositionCounter); i++)
//         {                                           
//           digitalWrite(DIRECTION_PIN_GRIPPER, LOW); 
//           digitalWrite(STEP_PIN_GRIPPER, HIGH);
//           delayMicroseconds(5);
//           digitalWrite(STEP_PIN_GRIPPER, LOW);
//           delayMicroseconds(5);
//         }
//         GripperPositionCounter = GripperPositionRing;
//       }
//     }
//   }
//   else
//   { // Open grippper Return to 0 position
//      for (uint16_t i = 0; i < (GripperPositionCounter); i++)
//         {                                           
//           digitalWrite(DIRECTION_PIN_GRIPPER, HIGH); 
//           digitalWrite(STEP_PIN_GRIPPER, HIGH);
//           delayMicroseconds(5);
//           digitalWrite(STEP_PIN_GRIPPER, LOW);
//           delayMicroseconds(5);
//         }
//         GripperPositionCounter = 0;
//   }
//   Serial.println(GripperPositionCounter);
// }
