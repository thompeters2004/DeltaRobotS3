/**
 * @file EtherCATConfig.h
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
#ifndef ETHERCATCONFIG_H
#define ETHERCATCONFIG_H


// Deltapicker control byte/bits
#define DeltaPickerEnableByte 0
#define DeltaPickerEnableBit 7

#define DeltaPickerSpeedByte 0

#define XcoordMSB 1
#define XcoordLSB 2
#define YcoordMSB 3
#define YcoordLSB 4
#define ZcoordMSB 5
#define ZcoordLSB 6

#define PickerAttachedByte 7
#define PickerAttachedBit 7

#define PickerStateByte 7
#define PickerStateBitMSB 6
#define PickerStateBitLSB 5

#define ObjectSelectByte 7
#define ObjectSelect1 4
#define ObjectSelect2 3
#define ObjectSelect3 2

#define CalibrationByte 7
#define CalibrationBit 2
#define CalibrationMSB 1
#define CalibrartionLSB 0 

#define EnablePickerMotorByte 8
#define EnablePickerMotorBit 7

#define MaxSpeedPickerByte 8
#define MaxSpeedStartBit 6
#define MaxSpeedStopBit 0





// Deltapicker control byte/bits
#define DeltaPickerEnabledByte 0
#define DeltaPickerEnabledBit 7

#define DeltaPickerReturnSpeedByte 0

#define XcurrentCoordMSB 1
#define XcurrentCoordLSB 2
#define YcurrentCoordMSB 3
#define YcurrentCoordLSB 4
#define ZcurrentCoordMSB 5
#define ZcurrentCoordLSB 6

#define PickerReturnAttachedByte 7
#define PickerReturnAttachedBit 7

#define PickerReturnStateByte 7
#define PickerReturnStateBitMSB 6
#define PickerReturnStateBitLSB 5

#define ObjectSelectByte 7
#define ObjectSelect1 4
#define ObjectSelect2 3
#define ObjectSelect3 2

#define CalibrationDoneByte 7
#define CalibrationDoneBit 4

#define InstructionsDoneByte 7
#define InstructionsDoneBit 3

#define EnabledPickerMotorByte 8
#define EnabledPickerMotorBit 7

#define ReturnSpeedPickerByte 8
#define ReturnSpeedStartBit 6
#define ReturnSpeedStopBit 0


#endif //ETHERCATCONFIG_H