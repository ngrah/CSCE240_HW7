/****************************************************************
 * Header for the 'ScanLine' class for utility programs.
 *
 * Author/copyright:  Duncan Buell
 * Date: 16 July 2013
 *
 * This code performs the utility function of being a 'Scanner'
 * for a string, analogous to what a 'Scanner' does on a file.
**/

#ifndef SCANLINE_H
#define SCANLINE_H

#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../Utilities/utils.h"
// #include "../Utilities/Scanner.h"

#define NDEBUG
#include <cassert>

typedef unsigned int UINT;
typedef int64_t LONG;

class ScanLine {
 public:
  //  static const string WHITESPACE;
  /****************************************************************
   * Input, output, and log streams.
  **/
  //  static ofstream zorklogStream;

  //  static stringstream zorkss;
  //  static ostringstream zorkoss;

  std::stringstream scanline_ss_;

  /****************************************************************
   * Constructors and destructors for the class.
  **/
  ScanLine();
  virtual ~ScanLine();

  /****************************************************************
   * General functions.
  **/
  bool HasMoreData();
  bool HasNext();
  void OpenString(std::string line);
  std::string Next();
  double NextDouble();
  int NextInt();
  LONG NextLONG();
  std::string NextLine();

 private:
};

#endif  // SCANLINE_H
