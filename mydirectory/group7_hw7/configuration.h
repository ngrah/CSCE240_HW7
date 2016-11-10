/****************************************************************
 * Header for the 'Configuration' class
 *
 * Author/copyright:  Duncan Buell
 * Date: 19 July 2016
 *
**/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <set>
#include <vector>

#include "../Utilities/utils.h"
#include "../Utilities/scanner.h"
#include "../Utilities/scanline.h"

#include "myrandom.h"

using namespace std;

static const int kDefaultSeed = 19;
static const int kDummyConfigInt = -111;
static const double kDummyConfigDouble = -22.22;

class Configuration
{
public:
/****************************************************************
 * Constructors and destructors for the class. 
**/
 Configuration();
 virtual ~Configuration();

/****************************************************************
 * Accessors and Mutators.
**/
/****************************************************************
 * public variables
**/
 int seed_ = kDefaultSeed;
 int election_day_length_hours_ = kDummyConfigInt;
 int election_day_length_seconds_ = kDummyConfigInt;
 int time_to_vote_mean_seconds_ = kDummyConfigInt;
 int max_expected_to_simulate_ = kDummyConfigInt;
 int min_expected_to_simulate_ = kDummyConfigInt;
 int wait_time_minutes_that_is_too_long_ = kDummyConfigInt;
 int number_of_iterations_ = kDummyConfigInt;
 vector<int> actual_service_times_;
 double arrival_zero_ = kDummyConfigDouble;
 vector<double> arrival_fractions_;

/****************************************************************
 * General functions.
**/

 int GetMaxServiceSubscript() const;
 void ReadConfiguration(Scanner& instream);
 string ToString();

private:

};

#endif // ONEVOTER_H
