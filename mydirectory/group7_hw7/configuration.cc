#include "configuration.h"
/****************************************************************
 * Implementation for the 'Configuration' class.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used with permission and modified by: Group 7
 * Date: 6 October 2016
**/

static const string kTag = "CONFIG: ";

/****************************************************************
 * Constructor.
**/
Configuration::Configuration() {}

/****************************************************************
 * Destructor.
**/
Configuration::~Configuration() {}

/****************************************************************
 * Accessors and mutators.
**/
/****************************************************************
 * Function 'GetMaxServiceSubscript'
 * This function returns the total number of service times.
**/
int Configuration::GetMaxServiceSubscript() const {
  return static_cast<int>(actual_service_times_.size()) - 1;
}

/****************************************************************
 * General functions.
**/
/****************************************************************
 * Function 'ReadConfiguration'
 * This function reads two lines from the scanner containing
 * election data.  The scanner is a parameter of the function.
 *
 * The first line has seven different integer values that
 * have information regarding what is known and/or expected
 * on election day.  The second line has the first number as
 * the percentage of people that voted in advance, and the
 * rest of the numbers in the line are the corresponding
 * hour-by-hour percentages of voters that arrive on the
 * election day.  The actual service times are read from a
 * file and read into a public vector which are used in
 * other functions.
 * NOTE: These public variables can be modified so extra
 *       care should be taken not to modify them in other
 *       areas of the code.
 *
 * Parameters:
 * Scanner& instream - A scanner containing the two lines
 *                     of data needed for the program. 
**/
void Configuration::ReadConfiguration(Scanner& instream) {
  /*
  */
  string line;
  ScanLine scanline;

  line = instream.NextLine();
  scanline.OpenString(line);
  seed_ = scanline.NextInt();
  election_day_length_hours_ = scanline.NextInt();
  election_day_length_seconds_ = election_day_length_hours_ * 3600;
  time_to_vote_mean_seconds_ = scanline.NextInt();
  min_expected_to_simulate_ = scanline.NextInt();
  max_expected_to_simulate_ = scanline.NextInt();
  wait_time_minutes_that_is_too_long_ = scanline.NextInt();
  number_of_iterations_ = scanline.NextInt();

  line = instream.NextLine();
  scanline.OpenString(line);
  arrival_zero_ = scanline.NextDouble();
  for (int sub = 0; sub < election_day_length_hours_; ++sub) {
    double input = scanline.NextDouble();
    arrival_fractions_.push_back(input);
  }

  Scanner service_times_file;
  // TODO(hxtk): document or remove hard-coded filename
  service_times_file.OpenFile("../../dataallsorted.txt");
  while (service_times_file.HasNext()) {
    int thetime = service_times_file.NextInt();
    actual_service_times_.push_back(thetime);
  }
}

/****************************************************************
 * Function 'ToString'
 *
 * This function returns a string with labels for the data read
 * in the 'ReadConfiguration' function
**/
string Configuration::ToString() {
  string s = "\n";
  s += kTag;
  s += "RN seed:              ";
  s += Utils::Format(seed_, 8) + "\n";
  s += kTag;
  s += "Election Day length:  ";
  s += Utils::Format(election_day_length_seconds_, 8) + " =";
  s += Utils::Format(election_day_length_seconds_ / 3600.0, 8, 2) + " (";
  s += Utils::Format(election_day_length_hours_, 8, 2) + ") hours\n";
  s += kTag;
  s += "Time to vote mean:    ";
  s += Utils::Format(time_to_vote_mean_seconds_, 8) + " =";
  s += Utils::Format(time_to_vote_mean_seconds_ / 60.0, 8, 2) + " minutes\n";
  s += kTag;
  s += "Min and max expected voters for this simulation:     ";
  s += Utils::Format(min_expected_to_simulate_, 8);
  s += Utils::Format(max_expected_to_simulate_, 8) + "\n";
  s += "Wait time (minutes) that is 'too long': ";
  s += Utils::Format(wait_time_minutes_that_is_too_long_, 8) + "\n";
  s += "Number of iterations to perform: ";
  s += Utils::Format(number_of_iterations_, 4) + "\n";
  s += "Max service time subscript: ";
  s += Utils::Format(GetMaxServiceSubscript(), 6) + "\n";
  int offset = 6;
  s += kTag;
  s += Utils::Format(0, 2) + "-" + Utils::Format(0, 2);
  s += " : " + Utils::Format(arrival_zero_, 7, 2) + "\n";
  for (UINT sub = 0; sub < arrival_fractions_.size(); ++sub) {
    s += kTag;
    s += Utils::Format(offset + sub, 2) + "-" +
         Utils::Format(offset + sub + 1, 2);
    s += " : " + Utils::Format(arrival_fractions_.at(sub), 7, 2) + "\n";
  }
  s += "\n";
  return s;
}
