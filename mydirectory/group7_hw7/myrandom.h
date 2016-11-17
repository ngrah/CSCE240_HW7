/****************************************************************
 * Header file for the random number generator.
 *
 * Author/copyright:  Duncan Buell
 * Date: 23 May 2016
 *
**/

#ifndef MYRANDOM_H
#define MYRANDOM_H

#include <cassert>
#include <iostream>
#include <random>
using namespace std;

#include "../../Utilities/scanline.h"
#include "../../Utilities/scanner.h"

#undef NDEBUG

class MyRandom {
 public:
  MyRandom();
  MyRandom(unsigned seed);
  virtual ~MyRandom();

  int RandomExponentialInt(double mean);
  double RandomNormal(double mean, double dev);
  double RandomUniformDouble(double lower, double upper);
  int RandomUniformInt(int lower, int upper);

 private:
  unsigned int seed_;

  std::mt19937 generator_;
};

#endif
