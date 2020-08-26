#include "RoboClaw.h"

#include <ros/ros.h>

using namespace std;

#define ADDRESS 0x80
// #define Kp 0x00010000
// #define Ki 0x00008000
// #define KD 0x00004000
// #define qpps 44000
#define KP 0x00009000
#define KI 0x00000250
#define KD 0x00001000
#define QPPS 300000
// #define qpps 24000

void basic() {
  ros::NodeHandle nh;
  int32_t val, prev_val = 0, diff = 0;
  uint8_t status;
  uint8_t mode1, mode2;
  uint16_t voltage;
  bool valid;
  string port("/dev/ttyACM0");
  USBSerial serial;
  uint32_t enc1, enc2;

  //no baud rate needed as the Linux driver will always run at the fastest rate possible
  
  cout << "Opening serial port..." << endl;
  serial.Open(port.c_str());

  RoboClaw roboclaw(&serial);

  string version;
  valid = roboclaw.ReadVersion(ADDRESS, &version);
  cout << "valid: " << valid << endl;
  cout << "Version: " << version << endl;

  roboclaw.ResetEncoders(ADDRESS);

  //enc1= roboclaw.ReadEncM1(ADDRESS, &status, &valid);
  //cout << "valid: " << valid << endl;
  //cout << "Enc M1: " << enc1 << endl;

  //enc1= roboclaw.ReadEncM1(ADDRESS, &status, &valid);
  //cout << "Enc M1: " << enc1 << endl;
  //exit(0);

  //voltage = roboclaw.ReadMainBatteryVoltage(ADDRESS, &valid);
  voltage = roboclaw.ReadLogicBattVoltage(ADDRESS, &valid);
  cout << "valid: " << valid << endl;
  cout << "voltage: " << hex << voltage << endl;

  uint32_t ispeed;
  ispeed = roboclaw.ReadISpeedM1(ADDRESS, &status, &valid);
  cout << "valid: " << valid << endl;
  cout << "ispeed: " << ispeed << endl;
  //exit(0);

  //duty cycle range is 0-32767
  uint16_t m1duty = 6000;
  uint16_t m2duty = 6000;
  //valid = roboclaw.DutyM1M2(ADDRESS, m1duty, m2duty);
  //cout << "valid: " << valid << endl;

  //ros::Duration(3).sleep();

  //valid = roboclaw.DutyM1M2(ADDRESS, 1, 1);
  


  //roboclaw.ReadEncoderModes(ADDRESS, mode1, mode2);
  //cout << "valid: " << valid << endl;
  //cout << "modes: " << mode1 << ", " << mode2 << endl;
  

  //roboclaw.SetPWM(ADDRESS, 0);
  //roboclaw.SetM1Constants(ADDRESS,KD,KP,KI,QPPS);
  //roboclaw.SetM2Constants(ADDRESS,KD,KP,KI,QPPS);
   //roboclaw.ReadEncM1(ADDRESS, &status, &valid);

  uint32_t desired = QPPS / 2.0;
  // roboclaw.ForwardM2(ADDRESS, 127);
  // roboclaw.ReadError(ADDRESS, &valid);
  //roboclaw.SpeedAccelM1(ADDRESS, desired / 4.0, desired);
  //roboclaw.SpeedAccelM2(ADDRESS, desired / 4.0, desired);
  // roboclaw.ForwardM2(ADDRESS, 10);
  ros::Time start = ros::Time::now();
  //if (valid) {
  //  printf("%u\n", val);
  //}

  while (ros::ok()) {
    ros::Duration(0.1).sleep();

    enc1 = roboclaw.ReadEncM1(ADDRESS, &status, &valid);
    enc2 = roboclaw.ReadEncM2(ADDRESS, &status, &valid);

    ispeed = roboclaw.ReadISpeedM1(ADDRESS, &status, &valid);
    //cout << "valid: " << valid << endl;
    cout << "ispeed: " << ispeed << endl;

    //cout << "Enc L: " << enc2 << " Enc R: " << enc1 << endl;

    diff = val - prev_val;
    prev_val = val;

    if (valid) {
      //printf("Encoder count: %u", val);
      if (status & 1) {
        printf(" UNDERFLOW");
      }
      if ((status & 2) >> 1) {
        printf(" BACKWARDS");
      }
      if ((status & 4) >> 2) {
        printf(" OVERFLOW");
      }
      printf("\n");
    } else {
      //printf("INVALID! - %i\n", valid);
    }

    // Stop after 1 revolution
    // if (val > 2.7 * 1.5 * 16 * 4 * 500) {
    //   break;
    // }

    //val = roboclaw.ReadSpeedM2(ADDRESS, &status, &valid);
    //if(valid){
    //  printf("Speed : %i (status = %x) %i\n", val, status, diff);
    //}

    // if (abs(val  - desired) / (double)desired < 0.05) {
    //   break;
    // }
  }
  ros::Duration total = ros::Time::now() - start;
  cout << total << endl;
  //roboclaw.ForwardM1(ADDRESS, 0);
  //roboclaw.ForwardM2(ADDRESS, 0);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "enc_demo");
  ros::Time::init();

  basic();
}
