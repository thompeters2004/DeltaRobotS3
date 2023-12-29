/**
 * @file main.cpp
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
#include "main.h"

void setup()
{
  Serial.begin(9600);

  while (InitializeEtherCAT() == false)
  {
    // Error state
    Serial.println("Initialized fail");
    // probably wrong chipselect
  }
  // Wait for enable bit
  EtherCATCycleUpdate();
  bool NotEnabled = 0;
  pinMode(LimitSwitchGripperPin, INPUT);
  Coordinates off(0, 0, 0);

  while (!NotEnabled)
  {
    NotEnabled = EtherCATGetBIT(DeltaPickerEnableByte, DeltaPickerEnableBit);
    Serial.println("Not Enabled");

    UpdateEtherCAT_Deltapicker(off, 0, false); // 0,0,0    Speed 0     Not Enabled
    EtherCATCycleUpdate();
  }

  UpdateEtherCAT_Deltapicker(off, 0, true); // 0,0,0    Speed 0     Enabled
  Serial.println("Setup done");
}

void loop()
{
  // Calibrate the deltapicker. The deltapicker will move to the default postion
  uint8_t SetSpeed = 0;

  DeltaPickerUpdateMaxSpeed(SetSpeed);
  DeltaPickerCalibrate();
  Coordinates CurrentCoordinates = DefaultPosition; // after calibration we can assume that the deltapicker is on the default position
  Coordinates DesiredCoordinates = DefaultPosition; // no data from ethercat yet so we shouldn't move
  GripperValues Grippervalue;

  while (1)
  {
    EtherCATCycleUpdate(); // Get new EtherCAT data
    bool DeltaPickerEnabled = EtherCATGetBIT(DeltaPickerEnableByte, DeltaPickerEnableBit);

    if (DeltaPickerEnabled == false)
    {
      SECONDDeltaPickerCalibrate();
      Serial.println("Recalibration done");
      EtherCATCycleUpdate(); // Get new EtherCAT data
      delay(2000);
    }

    if (DeltaPickerEnabled == true) // If system is enabled
    {
      SetSpeed = EtherCATGetByte(DeltaPickerSpeedByte) & 0x7F;
      DeltaPickerUpdateMaxSpeed(SetSpeed);                  // Without enable 0-100 speed
      DeltaPickerGetDesiredCoordinates(DesiredCoordinates); // get desired coordinates from deltapicker

      if (DesiredCoordinates != CurrentCoordinates && DesiredCoordinateInScope(DesiredCoordinates))
      {
        // Move accel stepper
        MoveToCoordinates(CurrentCoordinates, DesiredCoordinates);
        CurrentCoordinates.x = DesiredCoordinates.x;
        CurrentCoordinates.y = DesiredCoordinates.y;
        CurrentCoordinates.z = DesiredCoordinates.z;
      }

      else
      {
        delay(10); // Wait 10 mili second before rechecking
      }
      // AdjustGripper();
    }

    else
    {
      // No enable
      delay(20); // Wait a second before rechecking if enable is out
    }
    UpdateEtherCAT_Deltapicker(CurrentCoordinates, SetSpeed, DeltaPickerEnabled);
  }
}
