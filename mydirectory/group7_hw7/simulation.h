/****************************************************************
 * Header for the 'Simulation' class.
 *
 * Author/copyright:  Duncan Buell
 * Date: 19 July 2016
 *
**/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>

#include "../Utilities/utils.h"
#include "../Utilities/scanner.h"
#include "../Utilities/scanline.h"

using namespace std;

#include "configuration.h"
#include "onepct.h"

class Simulation
{
public:
/****************************************************************
 * Constructors and destructors for the class. 
**/
  Simulation();
  virtual ~Simulation();

/****************************************************************
 * General functions.
**/
  void ReadPrecincts(Scanner& infile);
  void RunSimulation(const Configuration& config,
                     MyRandom& random, ofstream& out_stream);
  string ToString();
  string ToStringPcts();

private:
/****************************************************************
 * Variables.
**/
  map<int, OnePct> pcts_;

/****************************************************************
 * Private functions.
**/
};

#endif // SIMULATION_H
