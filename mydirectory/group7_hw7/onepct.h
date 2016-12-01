/****************************************************************
 * Header for the 'OnePct' class
 *
 * Author/copyright:  Duncan Buell
 * Date: 6 October 2016
 *
**/

#ifndef ONEPCT_H
#define ONEPCT_H

#include <cmath>
#include <map>
#include <set>
#include <vector>

#include "../../Utilities/scanline.h"
#include "../../Utilities/scanner.h"
#include "../../Utilities/utils.h"

using namespace std;

#include "configuration.h"
#include "myrandom.h"
#include "onevoter.h"

static const double kDummyDouble = -88.88;
static const int kDummyInt = -999;
static const string kDummyString = "dummystring";

class OnePct {
 public:
  /****************************************************************
   * Constructors and destructors for the class.
  **/
  OnePct();
  OnePct(Scanner& infile);
  virtual ~OnePct();

  /****************************************************************
   * Accessors and Mutators.
  **/
  int GetExpectedVoters() const;
  int GetPctNumber() const;

  /****************************************************************
   * General functions.
  **/
  // ReadData takes input and sets variable values for a single precinct.
  void ReadData(Scanner& infile);
  // RunSimulationPct simulates the voting process for a single precinct.
  void RunSimulationPct(const Configuration& config, MyRandom& random,
                        ofstream& out_stream);
  // ToString Is a standard ToString function for data related to a single 
  //   precinct.
  string ToString();
  // ToStringVoterMap is a standard ToString function for voter data collected
  //   during the simulation.
  string ToStringVoterMap(string label, multimap<int, OneVoter> themap);

  /****************************************************************
   * Private variables
  **/
 private:
  int pct_expected_voters_ = kDummyInt;
  int pct_expected_per_hour_ = kDummyInt;
  double pct_minority_ = kDummyDouble;
  string pct_name_ = kDummyString;
  int pct_number_ = kDummyInt;
  double pct_turnout_ = kDummyDouble;
  int pct_stations_ = kDummyInt;
  int pct_num_voters_ = kDummyInt;
  double wait_dev_seconds_;
  double wait_mean_seconds_;
  set<int> stations_to_histo_;
  vector<int> free_stations_;
  multimap<int, OneVoter> voters_backup_;
  multimap<int, OneVoter> voters_done_voting_;
  multimap<int, OneVoter> voters_pending_;
  multimap<int, OneVoter> voters_voting_;

  /****************************************************************
   * General private functions.
  **/
  // CreateVoters generates instances of OneVoter to be used in the simulation.
  void CreateVoters(const Configuration& config, MyRandom& random,
                    ofstream& out_stream);
  // DoStatistics determines the number of voters who waited too long, and uses
  //  ComputeMeanAndDev to compute the mean and standard deviation of wait 
  //  times.  This function also formats this information into a string and 
  //  passes it out with Utils Output, and returns the number of voters who 
  //  waited too long.
  int DoStatistics(int iteration, const Configuration& config,
                   int station_count, map<int, int>& map_for_histo,
                   ofstream& out_stream);
  // ComputeMeanAndDev computes the mean and standard deviation of voter wait
  //  times.
  void ComputeMeanAndDev();
  // RunSimulationPct2 is called by RunSimulationPct.
  // This function simulates moving lines of voters through polling stations 
  //  for a single precinct. 
  void RunSimulationPct2(int stations);
};

#endif  // ONEPCT_H
