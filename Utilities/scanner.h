/****************************************************************
 * Header for the 'Scanner' class.
 *
 * Author/copyright:  Duncan Buell
 * Date: 8 May 2016
 *
**/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "scanline.h"
#include "utils.h"

#define NDEBUG
#include <cassert>

typedef int64_t LONG;

class Scanner {
 public:
  /****************************************************************
   * Constructors and destructors for the class.
  **/
  Scanner();
  virtual ~Scanner();

  /****************************************************************
   * Variables.
  **/
  std::ifstream local_stream_;

  /****************************************************************
   * General functions.
  **/
  void Close();
  bool HasNext();
  double NextDouble();
  std::string Next();
  std::string NextLine();
  void OpenFile(std::string filename);
  int NextInt();
  LONG NextLONG();

 private:
  const std::string kTag = "SCANNER: ";

  ScanLine scanline_;
};

#endif  // SCANNER_H_
