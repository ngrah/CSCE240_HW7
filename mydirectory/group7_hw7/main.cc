/****************************************************************
 * Main program for simulation.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Date: 6 October 2016
 *
**/
#include "main.h"

static const string kTag = "MAIN: ";

int main(int argc, char *argv[])
{
  string config_filename;
  string pct_filename = "XX";
  string log_filename = "XX";
  string out_filename = "XX";
  string outstring = "XX";
  string timecall_output = "XX";

  ofstream out_stream;

  Scanner config_stream;
  Scanner pct_stream;

  Configuration config;
  Simulation simulation;

  MyRandom random;

  cout<< kTag << "Beginning execution" << endl;

  Utils::CheckArgs(4, argc, argv, "configfilename pctfilename outfilename logfilename");
  config_filename = static_cast<string>(argv[1]);
  pct_filename = static_cast<string>(argv[2]);
  out_filename = static_cast<string>(argv[3]);
  log_filename = static_cast<string>(argv[4]);

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
  config.ReadConfiguration(config_stream);
  config_stream.Close();

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

  cout<< kTag << "Ending execution" << endl;

  return 0;
}
