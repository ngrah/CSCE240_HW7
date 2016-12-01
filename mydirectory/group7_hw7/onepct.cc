stall #include "onepct.h"
/****************************************************************
* Implementation for the 'OnePct' class.
*
* Author/copyright:  Duncan Buell. All rights reserved.
* Used/Modified: Group 7
* Date: 30 November 2016
*
* This class represents a single precint in the voting simulation program.
* The ReadPrecincts function in the Simulation class uses creates multiple 
* instances of this class, populates them with data from the input file, 
* and stores them in the pcts_ map for use in the simulation.
*
* The RunSimulation function in the Simulation class iterates through the the pcts_ map
* of precincts.  Using the GetExpectedVoters function contained within this
* class, the simulation determines if a precinct contains an appropriate number
* of voters to be used in the simulation.  This number is specified in Config.
*
* If a precinct has an appropriate number of expected voters for the simulation,
* the Simulation class then uses the RunSimulationPct function for that Precinct,
* and repeats the process for the next precinct in the pcts_ map.
*
**/

static const string kTag = "OnePct: ";

/****************************************************************
* Constructor.
**/
OnePct::OnePct() {}
/****************************************************************
* Constructor.
* This constuctor uses the ReadData function contained within this class
* to populate a new instance of OnePct with data from the input file.
**/
OnePct::OnePct(Scanner& infile) { this->ReadData(infile); }

/****************************************************************
* Destructor.
**/
OnePct::~OnePct() {}

/****************************************************************
* Accessors and mutators.
**/

/****************************************************************
 * Function 'GetExpectedVoters'
 * This function returns the number of expected voters for a precinct.
**/
int OnePct::GetExpectedVoters() const { return pct_expected_voters_; }

/****************************************************************
 * Function 'GetPctNumber'
 * This function returns the number assigned to a prectinct by ReadData.
**/
int OnePct::GetPctNumber() const { return pct_number_; }

/****************************************************************
* General functions.
**/
/******************************************************************************
 * Function 'ComputeMeanAndDev'
 * This function is called by RunSimulationPct to calculate mean and standard
 * deviation of simulated voting times. The voting times are taken from the 
 * instances of OneVoter in the voters_done_voting_ map. 
**/
void OnePct::ComputeMeanAndDev() {
  int sum_of_wait_times_seconds = 0;
  double sum_of_adjusted_times_seconds = 0.0;
  sum_of_wait_times_seconds = 0;
  multimap<int, OneVoter>::iterator iter_multimap;
  for (iter_multimap = voters_done_voting_.begin();
       iter_multimap != voters_done_voting_.end(); ++iter_multimap) {
    OneVoter voter = iter_multimap->second;
    sum_of_wait_times_seconds += voter.GetTimeWaiting();
  }
  wait_mean_seconds_ = static_cast<double>(sum_of_wait_times_seconds) /
                       static_cast<double>(pct_expected_voters_);

  sum_of_adjusted_times_seconds = 0.0;
  for (iter_multimap = voters_done_voting_.begin();
       iter_multimap != voters_done_voting_.end(); ++iter_multimap) {
    OneVoter voter = iter_multimap->second;
    double this_addin =
        static_cast<double>(voter.GetTimeWaiting()) - wait_mean_seconds_;

    sum_of_adjusted_times_seconds += (this_addin) * (this_addin);
  }
  wait_dev_seconds_ = sqrt(sum_of_adjusted_times_seconds /
                           static_cast<double>(pct_expected_voters_));
}

/****************************************************************
 * Function 'CreateVoters'
 * This function is called by the RunSimulationPct function.
 * This function uses the information in the configuration file and the random
 * number generator to create instances of the OneVoter class, and insert the
 * instances of OneVoter into the voters_backup_ map.
 *
 * This task is performed with two sections.  The first section generates and
 * stores voters that were at the polling station at the time it opened.
 * The second section generates and stores voters who arrive throughout the day.
**/
void OnePct::CreateVoters(const Configuration& config, MyRandom& random,
                          ofstream& out_stream) {
  int duration = 0;
  int arrival = 0;
  int sequence = 0;
  double percent = 0.0;
  string outstring = "XX";

  voters_backup_.clear();
  sequence = 0;
  
  ////////////////////////////////////////////////////////////////////////////
  // This creates voters who are in line when the precinct opens.
  percent = config.arrival_zero_;
  int voters_at_zero = round((percent / 100.0) * pct_expected_voters_);
  arrival = 0;
  for (int voter = 0; voter < voters_at_zero; ++voter) {
    int durationsub =
        random.RandomUniformInt(0, config.GetMaxServiceSubscript());
    duration = config.actual_service_times_.at(durationsub);
    OneVoter one_voter(sequence, arrival, duration);
    voters_backup_.insert(std::pair<int, OneVoter>(arrival, one_voter));
    ++sequence;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // This creates voters who arrive at the precinct throughout the day.
  for (int hour = 0; hour < config.election_day_length_hours_; ++hour) {
    percent = config.arrival_fractions_.at(hour);
    int voters_this_hour = round((percent / 100.0) * pct_expected_voters_);
    if (0 == hour % 2) ++voters_this_hour;

    int arrival = hour * 3600;
    for (int voter = 0; voter < voters_this_hour; ++voter) {
      double lambda = static_cast<double>(voters_this_hour / 3600.0);
      int interarrival = random.RandomExponentialInt(lambda);
      arrival += interarrival;
      int durationsub =
          random.RandomUniformInt(0, config.GetMaxServiceSubscript());
      duration = config.actual_service_times_.at(durationsub);

      OneVoter one_voter(sequence, arrival, duration);
      voters_backup_.insert(std::pair<int, OneVoter>(arrival, one_voter));
      ++sequence;
    }
  }
}

/******************************************************************************
 * Function 'DoStatistics'
 * This function is called by the RunSimulationPct function after the simulation
 * has completed and data has been collected, and returns the number of voters 
 * who waited too long.
 *
 * This function creates a map to store wait times of voters done voting. 
 * Then, this function iterates through the voters_done_voting_ map and stores
 * the wait time for each voter in the wait_time_minutes_map.
 * Next, this function iterates through the wait_time_minutes_map and counts how
 * many voters waited for too long. The ComputeMeanAndDev function is then 
 * called to compute the mean and standard deviation of the wait times in
 * the wait_time_minutes_map. The calculated data is stored to a string 
 * outstring passed out with the Utils Output function.
 *
**/

int OnePct::DoStatistics(int iteration, const Configuration& config,
                         int station_count, map<int, int>& map_for_histo,
                         ofstream& out_stream) {
  string outstring = "\n";
  map<int, int> wait_time_minutes_map;

  /////////////////////////////////////////////////////////////////////////////
  // This iterates through voters_done_voting_ and stores wait times in
  // wait_times_minutes.
  
  multimap<int, OneVoter>::iterator iter_multimap;
  for (iter_multimap = this->voters_done_voting_.begin();
       iter_multimap != this->voters_done_voting_.end(); ++iter_multimap) {
    OneVoter voter = iter_multimap->second;
    int wait_time_minutes = voter.GetTimeWaiting() / 60;  // secs to mins

    ++(wait_time_minutes_map[wait_time_minutes]);
    ++(map_for_histo[wait_time_minutes]);
  }

  /////////////////////////////////////////////////////////////////////////////
  // This iterates through wait_times_minutes and determines the number of
  // voters who waited too long
  
  int toolongcount = 0;
  int toolongcountplus10 = 0;
  int toolongcountplus20 = 0;
  for (auto iter = wait_time_minutes_map.rbegin();
       iter != wait_time_minutes_map.rend(); ++iter) {
    int waittime = iter->first;
    int waitcount = iter->second;
    if (waittime > config.wait_time_minutes_that_is_too_long_)
      toolongcount += waitcount;
    if (waittime > config.wait_time_minutes_that_is_too_long_ + 10)
      toolongcountplus10 += waitcount;
    if (waittime > config.wait_time_minutes_that_is_too_long_ + 20)
      toolongcountplus20 += waitcount;
  }

  /////////////////////////////////////////////////////////////////////////////
  // This uses ComputeMeanAndDev to calculate the mean and standard
  // deviation of voter wait times and passes a string containing the info to
  // Utils Output.
  
  ComputeMeanAndDev();
  outstring = "";
  outstring +=
      kTag + Utils::Format(iteration, 3) + " " + Utils::Format(pct_number_, 4) +
      " " + Utils::Format(pct_name_, 25, "left") +
      Utils::Format(pct_expected_voters_, 6) + Utils::Format(station_count, 4) +
      " stations, mean/dev wait (mins) " +
      Utils::Format(wait_mean_seconds_ / 60.0, 8, 2) + " " +
      Utils::Format(wait_dev_seconds_ / 60.0, 8, 2) + " toolong " +
      Utils::Format(toolongcount, 6) + " " +
      Utils::Format(100.0 * toolongcount / (double)pct_expected_voters_, 6, 2) +
      Utils::Format(toolongcountplus10, 6) + " " +
      Utils::Format(100.0 * toolongcountplus10 / (double)pct_expected_voters_,
                    6, 2) +
      Utils::Format(toolongcountplus20, 6) + " " +
      Utils::Format(100.0 * toolongcountplus20 / (double)pct_expected_voters_,
                    6, 2) +
      "\n";

  Utils::Output(outstring, out_stream, Utils::log_stream);

  wait_time_minutes_map.clear();

  return toolongcount;
}

/****************************************************************
 * Function 'ReadData'
 * This function reads from the input file and uses the data to provide values 
 * for the private variables in an instance of OnePct.
 *
**/
void OnePct::ReadData(Scanner& infile) {
  if (infile.HasNext()) {
    pct_number_ = infile.NextInt();
    pct_name_ = infile.Next();
    pct_turnout_ = infile.NextDouble();
    pct_num_voters_ = infile.NextInt();
    pct_expected_voters_ = infile.NextInt();
    pct_expected_per_hour_ = infile.NextInt();
    pct_stations_ = infile.NextInt();
    pct_minority_ = infile.NextDouble();

    int stat1 = infile.NextInt();
    int stat2 = infile.NextInt();
    int stat3 = infile.NextInt();
    stations_to_histo_.insert(stat1);
    stations_to_histo_.insert(stat2);
    stations_to_histo_.insert(stat3);
  }
}  // void OnePct::ReadData(Scanner& infile)

/****************************************************************
 * Function 'RunSimulationPct'
 * This function is called by the Simulation class for each instance of OnePct.
 *
 * This function begins by calculation the max station count and min station 
 * count for the precinct based on expected voters.  Then, CreateVoters is 
 * called for this instance of OnePct. Next, RunSimulationPct2 is called for 
 * this instance of OnePct to simulate the line management and voting station 
 * usage associated with the voting process and store the generated data. 
 * Lastly, the histogram of voting data is stored to a string and passed out 
 * with Utils Output.
**/
void OnePct::RunSimulationPct(const Configuration& config, MyRandom& random,
                              ofstream& out_stream) {
  string outstring = "XX";

  int min_station_count =
      pct_expected_voters_ * config.time_to_vote_mean_seconds_;
  min_station_count =
      min_station_count / (config.election_day_length_hours_ * 3600);
  if (min_station_count <= 0) min_station_count = 1;
  int max_station_count = min_station_count + config.election_day_length_hours_;

  bool done_with_this_count = false;
  for (int stations_count = min_station_count;
       stations_count <= max_station_count; ++stations_count) {
    if (done_with_this_count) break;
    done_with_this_count = true;

    map<int, int> map_for_histo;

    outstring = kTag + this->ToString() + "\n";
    Utils::Output(outstring, out_stream, Utils::log_stream);
    for (int iteration = 0; iteration < config.number_of_iterations_;
         ++iteration) {
      this->CreateVoters(config, random, out_stream);

      voters_pending_ = voters_backup_;
      voters_voting_.clear();
      voters_done_voting_.clear();

      this->RunSimulationPct2(stations_count);
      int number_too_long = DoStatistics(iteration, config, stations_count,
                                         map_for_histo, out_stream);
      if (number_too_long > 0) {
        done_with_this_count = false;
      }
    }

    voters_voting_.clear();
    voters_done_voting_.clear();

    outstring = kTag + "toolong space filler\n";
    Utils::Output(outstring, out_stream, Utils::log_stream);

    if (stations_to_histo_.count(stations_count) > 0) {
      outstring = "\n" + kTag + "HISTO " + this->ToString() + "\n";
      outstring +=
          kTag + "HISTO STATIONS " + Utils::Format(stations_count, 4) + "\n";
      Utils::Output(outstring, out_stream, Utils::log_stream);

      int time_lower = (map_for_histo.begin())->first;
      int time_upper = (map_for_histo.rbegin())->first;

      int voters_per_star = 1;
      if (map_for_histo[time_lower] > 50) {
        voters_per_star =
            map_for_histo[time_lower] / (50 * config.number_of_iterations_);
        if (voters_per_star <= 0) voters_per_star = 1;
      }

      for (int time = time_lower; time <= time_upper; ++time) {
        int count = map_for_histo[time];

        double count_double = static_cast<double>(count) /
                              static_cast<double>(config.number_of_iterations_);

        int count_divided_ceiling =
            static_cast<int>(ceil(count_double / voters_per_star));
        string stars = string(count_divided_ceiling, '*');

        outstring = kTag + "HISTO " + Utils::Format(time, 6) + ": " +
                    Utils::Format(count_double, 7, 2) + ": ";
        outstring += stars + "\n";
        Utils::Output(outstring, out_stream, Utils::log_stream);
      }
      outstring = "HISTO\n\n";
      Utils::Output(outstring, out_stream, Utils::log_stream);
    }
  }
}

/****************************************************************
* Function 'RunSimulationPct2'
* This function is called by RunSimulationPct, and is used to simulate the 
* actual process of moving lines of generated voters through the voting 
* stations and storing their wait times.
*
**/
void OnePct::RunSimulationPct2(int stations_count) {
  free_stations_.clear();
  for (int i = 0; i < stations_count; ++i) {
    free_stations_.push_back(i);
  }

  voters_voting_.clear();
  voters_done_voting_.clear();

  int second = 0;
  bool done = false;
  while (!done) {
    for (auto iter = voters_voting_.begin(); iter != voters_voting_.end();
         ++iter) {
      if (second == iter->first) {
        OneVoter one_voter = iter->second;

        int which_station = one_voter.GetStationNumber();
        free_stations_.push_back(which_station);
        voters_done_voting_.insert(std::pair<int, OneVoter>(second, one_voter));
      }
    }
    voters_voting_.erase(second);

    vector<map<int, OneVoter>::iterator> voters_pending_to_erase_by_iterator;
    for (auto iter = voters_pending_.begin(); iter != voters_pending_.end();
         ++iter) {
      if (second >= iter->first) {        // if they have already arrived
        if (free_stations_.size() > 0) {  // and there are free stations
          OneVoter next_voter = iter->second;
          if (next_voter.GetTimeArrival() <= second) {
            int which_station = free_stations_.at(0);
            free_stations_.erase(free_stations_.begin());
            next_voter.AssignStation(which_station, second);
            int leave_time = next_voter.GetTimeDoneVoting();
            voters_voting_.insert(
                std::pair<int, OneVoter>(leave_time, next_voter));
            voters_pending_to_erase_by_iterator.push_back(iter);

            // This was commented out 6 October 2016
            //            Utils::log_stream << kTag << "ASSIGNED    "
            //                              << Utils::Format(second, 5) << ": "
            //                              << next_voter.ToString() << "\n";

            /*
            Utils::log_stream << kTag << "PENDING, VOTING, DONE    "
            << Utils::Format((int)voters_pending_.size(), 5) << ": "
            << Utils::Format((int)voters_voting_.size(), 5) << ": "
            << Utils::Format((int)voters_done_voting_.size(), 5) << endl;
            */
          }     // if (next_voter.GetTimeArrival() <= second) {
        }       // if (free_stations_.size() > 0) {
      } else {  // if (second == iter->first) {
        break;  // we have walked in time past current time to arrivals in the
                // future
      }
    }  // for (auto iter = voters_pending_.begin(); iter !=
       // voters_pending_.end(); ++iter) {

    for (auto iter = voters_pending_to_erase_by_iterator.begin();
         iter != voters_pending_to_erase_by_iterator.end(); ++iter) {
      voters_pending_.erase(*iter);
    }
    ++second;
    //    if (second > 500) break;
    done = true;
    if ((voters_pending_.size() > 0) || (voters_voting_.size() > 0)) {
      done = false;
    }
  }  // while (!done) {

}  // void Simulation::RunSimulationPct2()

/****************************************************************
 * Function 'ToString' 
 * This is a standard ToString function.  
 * This function formats simulated and expected voter turnout, voters per hour,
 * number of polling stations and minority data to a string s. This function then
 * iterates through the stations_to_histo_ map and formats the data contained to
 * string s.  Lastly, this function returns string s.
 *
**/
string OnePct::ToString() {
  string s = "";

  s += Utils::Format(pct_number_, 4);
  s += " " + Utils::Format(pct_name_, 25, "left");
  s += Utils::Format(pct_turnout_, 8, 2);
  s += Utils::Format(pct_num_voters_, 8);
  s += Utils::Format(pct_expected_voters_, 8);
  s += Utils::Format(pct_expected_per_hour_, 8);
  s += Utils::Format(pct_stations_, 3);
  s += Utils::Format(pct_minority_, 8, 2);

  s += " HH ";
  for (auto iter = stations_to_histo_.begin(); iter != stations_to_histo_.end();
       ++iter) {
    s += Utils::Format(*iter, 4);
  }
  s += " HH";

  return s;
}  // string OnePct::ToString()

/****************************************************************
 * Function 'ToStringVoterMap'
 * This function takes in a multimap of instances of OneVoter.  This function
 * then iterates through the map of voters, calling ToString on each instance of
 * OneVoter and storing the output in string s.  Lastly, this function returns
 * string s.
**/
string OnePct::ToStringVoterMap(string label, multimap<int, OneVoter> themap) {
  string s = "";

  s += "\n" + label + " WITH " + Utils::Format((int)themap.size(), 6) +
       " ENTRIES\n";
  s += OneVoter::ToStringHeader() + "\n";
  for (auto iter = themap.begin(); iter != themap.end(); ++iter) {
    s += (iter->second).ToString() + "\n";
  }

  return s;
}  // string OnePct::ToString()
