/****************************************************************
 * Main program for simulation.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used/modified: Group 7
 * Date: 2016-11-29
 *
 * This is the entry point for the voting simulation program.
 * It first ensures that the correct arguments were passed in.
 *
 * Next, Utilities opens a log file. From this point forward,
 * primary log data shall be written to |Utils::log_stream|
 * instead of std::cout, std::cerr, etc.
 *
 * Finally, execution is passed off to the Simulation class.
 *
 * When execution returns from the Simulation class,
 * the streams are closed as needed and the program returns
 * exit success.
 *
**/
#include "main.h"

static const string kTag = "MAIN: ";

int main(int argc, char *argv[]) {
  string config_filename = "XX";
  string pct_filename = "XX";
  string data_filename = "XX";
  string log_filename = "XX";
  string out_filename = "XX";
  string outstring = "XX";
  string timecall_output = "XX";

  ofstream out_stream;

  Scanner config_stream;
  Scanner pct_stream;
  Scanner data_stream;

  Configuration config;
  Simulation simulation;

  MyRandom random;

  cout << kTag << "Beginning execution" << endl;

  Utils::CheckArgs(
      5, argc, argv,
      "configfilename pctfilename datafilename outfilename logfilename");
  
  config_filename = static_cast<string>(argv[1]);
  pct_filename = static_cast<string>(argv[2]);
  data_filename = static_cast<string>(argv[3]);
  out_filename = static_cast<string>(argv[4]);
  log_filename = static_cast<string>(argv[5]);

  Utils::FileOpen(out_stream, out_filename);
  Utils::LogFileOpen(log_filename);

  outstring = kTag + "Beginning execution\n";
  outstring += kTag + Utils::TimeCall("beginning");
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  outstring = kTag + "outfile '" + out_filename + "'" + "\n";
  outstring += kTag + "logfile '" + log_filename + "'" + "\n";
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  ////////////////////////////////////////////////////////////////////
  // config has RN seed, station count spread, election day length
  //   and mean and dev voting time
  config_stream.OpenFile(config_filename);
  data_stream.OpenFile(data_filename);
  config.ReadConfiguration(config_stream, data_stream);
  config_stream.Close();
  data_stream.Close();

  outstring = kTag + config.ToString() + "\n";
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  random = MyRandom(config.seed_);

  ////////////////////////////////////////////////////////////////////
  // now read the precinct data
  pct_stream.OpenFile(pct_filename);
  simulation.ReadPrecincts(pct_stream);
  pct_stream.Close();

  ////////////////////////////////////////////////////////////////////
  // here is the real work
  simulation.RunSimulation(config, random, out_stream);

  ////////////////////////////////////////////////////////////////////
  // close up and go home
  outstring = kTag + "Ending execution" + "\n";
  outstring += kTag + Utils::TimeCall("ending");
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  Utils::FileClose(out_stream);
  Utils::FileClose(Utils::log_stream);

  cout << kTag << "Ending execution" << endl;

  return 0;
}
