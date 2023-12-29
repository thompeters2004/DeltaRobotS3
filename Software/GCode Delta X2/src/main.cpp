#include "main.h"

Coordinates off;

void setup()
{
  Serial.begin(115200);

  delay(500);

  while (InitializeEtherCAT() == false)
  {
    // error state
    Serial.println("Initialized fail");
    // probably wrong chipselect
  }

  bool NotEnabled = 0;

  while (!NotEnabled) // Wait for enable
  {
    NotEnabled = EtherCATGetBIT(DeltaPickerEnableByte, DeltaPickerEnableBit);
    Serial.print("NotEnabled");

    UpdateEtherCAT_Deltapicker(off, false); // 0,0,0    Speed 0     Not Enabled
  }

  Serial.println("IsDelta");
  delay(1000);

  sendGcode("G28"); // homing
  sendGcode("G01 F200"); //Speed select

  
}

void loop()
{
  Coordinates CurrentCoordinates = off; // after calibration we can assume that the deltapicker is on the default position
  Coordinates DesiredCoordinates = off; // no data from ethercat yet so we shouldn't move

      DesiredCoordinates.x = 0;
      DesiredCoordinates.y = 0;
      DesiredCoordinates.z = -300;
      DesiredCoordinates.w = 45;
      DesiredCoordinates.GripperStatus = 1;
      DesiredCoordinates.ObjectType = 1;

  while (1)
  {
    EtherCATCycleUpdate(); // Get new EtherCAT data

    bool DeltaPickerEnabled = EtherCATGetBIT(DeltaPickerEnableByte, DeltaPickerEnableBit);
    if (DeltaPickerEnabled) // If system is enabled
    {
      if(DesiredCoordinates.z == -300){ 
        DesiredCoordinates.z = -320;
      } else {
        DesiredCoordinates.GripperStatus = !DesiredCoordinates.GripperStatus;
        DesiredCoordinates.z = -300;
      }
      if(DesiredCoordinates.ObjectType == 1){
         DesiredCoordinates.ObjectType = 2;
      }
      if(DesiredCoordinates.ObjectType == 2){
        DesiredCoordinates.ObjectType = 1;
      }
      // Movement
      DeltaPickerGetDesiredCoordinates(DesiredCoordinates); // get desired coordinates from deltapicker
     DesiredCoordinates.x++;
     if(DesiredCoordinates.z < 0) DesiredCoordinates.z++;

      //Test values

      easy_convert(DesiredCoordinates, CurrentCoordinates);
    
    }
    delay(500);
   // CurrentCoordinates = readGCode();
    UpdateEtherCAT_Deltapicker(CurrentCoordinates, DeltaPickerEnabled);
    // UpdateGripper
  }
}
