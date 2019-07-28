#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <wiringPi.h>

#include "HcSr04.hpp"

using namespace std;

#define TRIG_PIN	4 // BCM 23
#define ECHO_PIN	5 // BCM 24

#define DURING	5
#define CATCH_DISTANCE	5

namespace raspserver {

HcSr04::HcSr04(unsigned int pinTrig, unsigned int pinEcho):pinTrig(pinTrig),pinEcho(pinEcho){
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  digitalWrite(pinTrig, LOW);
  delay(1000);
}

HcSr04::~HcSr04(){
}

void HcSr04::setUp()
{
  if ( wiringPiSetup() == -1 )
  {
  }
}

double HcSr04::mesure()
{
  digitalWrite(pinTrig, HIGH);
  delay(1);
  digitalWrite(pinTrig, LOW);
  struct timespec sigoff, sigon;
  while(digitalRead(pinEcho) == LOW){
    clock_gettime(CLOCK_REALTIME, &sigoff);
  }
  while(digitalRead(pinEcho) == 1){
    clock_gettime(CLOCK_REALTIME, &sigon);
  }
  long sec, nsec = 0;
  double diff = 0;
  sec = sigon.tv_sec - sigoff.tv_sec;
  nsec = sigon.tv_nsec - sigoff.tv_nsec;
  diff = ((sec * 1000000000) + nsec);
  diff = diff * 34 / 2000000;
  cout << sec << ":" << nsec << ":" << diff << endl; 
//  printf("sec: %ld\n", sec);
//  printf("nsec: %ld\n", nsec);
//  printf("diff: %f\n", diff);
  return diff;
}

}
