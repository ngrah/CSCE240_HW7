#include "utils.h"

static const std::string kTag = "UTILS: ";
static const std::string WHITESPACE = " \n\t\r";

std::ofstream Utils::log_stream;
std::ostringstream Utils::oss;
std::stringstream Utils::ss;

/****************************************************************
 * Constructor.
**/
Utils::Utils() {
}
/****************************************************************
 * Destructor.
**/
Utils::~Utils() {
}

/****************************************************************
 * General functions.
**/
/****************************************************************
 * Check for the correct number of arguments.
 *
 * Parameters:
 *   howMany - the expected number of arguments
 *   argc - the usual 'argc' of command line information
 *   argv - the usual 'argv' of command line information
 *   usage - the 'usage' message to display on error
 * Return: none
**/
void Utils::CheckArgs(const int howMany, const int argc,
                      char *argv[], const std::string usage) {
  if(argc != howMany + 1)
  {
//    std::cout << kTag << "incorrect argument count\n";
    std::cout << kTag << "usage: " << argv[0] << " " << usage << std::endl;
    exit(1);
  }
}

/****************************************************************
 * Close an input stream.
 *
 * Parameters:
 *   in_stream - the 'ifstream' input stream by reference
 * Return: none
**/
void Utils::FileClose(std::ifstream& in_stream) {
  std::cout << kTag << "close the input file\n";
  in_stream.close();
  std::cout << kTag << "the input file was closed\n";
}

/****************************************************************
 * Close an output stream.
 *
 * Parameters:
 *   out_stream - the 'ofstream' output stream by reference
 * Return: none
**/
void Utils::FileClose(std::ofstream& out_stream) {
  std::cout << kTag << "close the output file\n";
  out_stream.close();
  std::cout << kTag << "the output file was closed\n";
}

/****************************************************************
 * Check to see if a file does exist.
 * This is done by attempting to open the file.
 *
 * Parameters:
 *   filename - the name of the file to be opened
 * Return: none
**/
bool Utils::FileDoesExist(std::string filename) {
  bool return_value = false; // default is that the file does not exist
  std::ifstream in_stream;

  std::cout << kTag << "test existence of input file '" << filename << "'\n";
  in_stream.open(filename.c_str());
  if(in_stream.fail())
  {
    std::cout << kTag << "open failed for '" << filename << "'\n";
    return_value = false;
    in_stream.close();
  }
  else
  {
    std::cout << kTag << "open succeeded for '" << filename << "'\n";
    return_value = true;
    in_stream.close();
  }

  return return_value;
}

/****************************************************************
 * Check to see if a file does not exist.
 * This is done by attempting to open the file.
 *
 * Parameters:
 *   filename - the name of the file to be opened
 * Return: none
**/
bool Utils::FileDoesNotExist(std::string filename) {
  bool return_value = true; // default is that the file does not exist
  std::ifstream in_stream;

  std::cout << kTag << "test nonexistence of input file '" << filename << "'\n";
  in_stream.open(filename.c_str());
  if(in_stream.fail())
  {
    std::cout << kTag << "file '" << filename << "' does not exist\n";
    return_value = true;
    in_stream.close();
  }
  else
  {
    std::cout << kTag << "file '" << filename << "' does indeed exist\n";
    return_value = false;
    in_stream.close();
  }

  return return_value;
}

/****************************************************************
 * Open an input stream.
 *
 * Parameters:
 *   in_stream - the 'ifstream' input stream by reference
 *   filename - the name of the file to be opened
 * Return: none
**/
void Utils::FileOpen(std::ifstream& in_stream, std::string filename) {
  std::cout << kTag << "open the input file '" << filename << "'\n";
  in_stream.open(filename.c_str());
  if(in_stream.fail())
  {
    std::cout << kTag << "open failed for '" << filename << "'\n";
    exit(0);
  }
  std::cout << kTag << "open succeeded for '" << filename << "'\n";
}

/****************************************************************
 * Open an output stream.
 *
 * Parameters:
 *   out_stream - the 'ofstream' output stream by reference
 *   filename - the name of the file to be opened
 * Return: none
**/
void Utils::FileOpen(std::ofstream& out_stream, std::string filename) {
  std::cout << kTag << "open the output file '" << filename << "'\n";
  out_stream.open(filename.c_str());
  if(out_stream.fail())
  {
    std::cout << kTag << "open failed for '" << filename << "'\n";
    exit(0);
  }
  std::cout << kTag << "open succeeded for '" << filename << "'\n";
}

/****************************************************************
 * Open the static logfile stream.
 *
 * Parameters:
 *   filename - the name of the file to associate with
 *              the 'Utils::log_stream'
 * Return: none
**/
void Utils::LogFileOpen(std::string filename) {
  std::cout << kTag << "open the logfile '" << filename << "'\n";
  log_stream.open(filename.c_str());
  if(log_stream.fail())
  {
    std::cout << kTag << "open failed for '" << filename << "'\n";
    exit(0);
  }
  std::cout << kTag << "open succeeded for '" << filename << "'\n";
}

/****************************************************************
 * These are the overloaded formatting functions that all return
 * a 'string' value after having formatted the first argument.
 * 
 * We have the following formatting:
 *   'char*' to a 'string' 
 *   'char*' to a 'string' of 'width' (default right) 
 *   'char*' to a 'string' of 'width' (left or right, default right) 
 *
 *   'string' to a 'string' 
 *   'string' to a 'string' of 'width' (default right) 
 *   'string' to a 'string' of 'width' (left or right, default right) 
 *
 *   'short'     to a 'string'
 *   'short'     to a 'string' of 'width'
 *   'int'       to a 'string'
 *   'int'       to a 'string' of 'width'
 *   'UINT'      to a 'string'
 *   'UINT'      to a 'string' of 'width'
 *   'LONG'      to a 'string'
 *   'LONG'      to a 'string' of 'width'
 *
 *   'double' to a 'string'
 *   'double' to a 'string' of 'width'
 *   'double' to a 'string' of 'width' and 'precision'
 *
**/
/****************************************************************
 * String-ify a 'char*' array, without width formatting.
 *
 * Parameters:
 *   value - the 'char*' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const char* value) {
  Utils::oss.str("");
  Utils::oss << std::string(value);
  return oss.str();
}

/****************************************************************
 * String-ify a 'char*' array, with width formatting.  This
 * uses the default justification on the left.
 *
 * Parameters:
 *   value - the 'char*' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const char* value, const int width) {
  Utils::oss.str("");
  Utils::oss << std::setw(width) << std::string(value);
  return oss.str();
}

/****************************************************************
 * String-ify a 'char*' array, with width formatting and with
 * specified justification.
 *
 * Parameters:
 *   value - the 'char*' variable to be converted and formatted.
 *   width - the width of the output field.
 *   justify - desired justification, either 'right' or 'left'
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const char* value, const int width,
                          const std::string justify) {
  Utils::oss.str("");

  if("left" == justify)
  {
    Utils::oss.setf(std::ios::left, std::ios::adjustfield);
    Utils::oss << std::setw(width) << std::string(value);
  }
  else if("right" == justify)
  {
    Utils::oss.setf(std::ios::right, std::ios::adjustfield);
    Utils::oss << std::setw(width) << std::string(value);
  }
  else
  {
    Utils::oss << std::setw(width) << std::string(value);
  }
  return oss.str();
}

/****************************************************************
 * String-ify a string, without width formatting and without
 * specified justification.  This is just a pass-through.
 *
 * Parameters:
 *   value - the 'string' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const std::string value) {
  return value;
}

/****************************************************************
 * String-ify a string, with width formatting but without
 * specified justification.
 *
 * Parameters:
 *   value - the 'string' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const std::string value, const int width) {
  Utils::oss.str("");
  Utils::oss << std::setw(width) << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'string', with width formatting and with
 * specified justification.
 *
 * Parameters:
 *   value - the 'string' variable to be converted and formatted.
 *   width - the width of the output field.
 *   justify - desired justification, either 'right' or 'left'
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const std::string value, const int width,
                          const std::string justify) {
  Utils::oss.str("");
  if("left" == justify)
  {
    Utils::oss.setf(std::ios::left, std::ios::adjustfield);
    Utils::oss << std::setw(width) << value;
  }
  else if("right" == justify)
  {
    Utils::oss.setf(std::ios::right, std::ios::adjustfield);
    Utils::oss << std::setw(width) << value;
  }
  else
  {
    Utils::oss << std::setw(width) << value;
  }
  return oss.str();
}

/****************************************************************
 * String-ify a 'short', without width formatting and without
 * specified justification.
 *
 * Parameters:
 *   value - the 'short' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const short value) {
  Utils::oss.str("");
  Utils::oss << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'short', with width formatting and with
 * default justification on the right.
 *
 * Parameters:
 *   value - the 'short' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const short value, const int width) {
  Utils::oss.str("");
  Utils::oss.setf(std::ios::right, std::ios::adjustfield);
  Utils::oss << std::setw(width) << value;
  return oss.str();
}

/****************************************************************
 * String-ify an 'int', without width formatting and without
 * specified justification.
 *
 * Parameters:
 *   value - the 'int' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const int value) {
  Utils::oss.str("");
  Utils::oss << value;
  return oss.str();
}

/****************************************************************
 * String-ify an 'int', with width formatting and with
 * default justification on the right.
 *
 * Parameters:
 *   value - the 'int' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const int value, const int width) {
  Utils::oss.str("");
  Utils::oss.setf(std::ios::right, std::ios::adjustfield);
  Utils::oss << std::setw(width) << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'UINT', without width formatting and without
 * specified justification.
 *
 * Parameters:
 *   value - the 'UINT' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const UINT value) {
  Utils::oss.str("");
  Utils::oss << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'UINT', with width formatting and with
 * default justification on the right.
 *
 * Parameters:
 *   value - the 'UINT' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const UINT value, const int width) {
  Utils::oss.str("");
  Utils::oss.setf(std::ios::right, std::ios::adjustfield);
  Utils::oss << std::setw(width) << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'LONG', without width formatting and without
 * specified justification.
 *
 * Parameters:
 *   value - the 'LONG' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const LONG value) {
  Utils::oss.str("");
  Utils::oss << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'LONG', with width formatting and with
 * default justification on the right.
 *
 * Parameters:
 *   value - the 'LONG' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const LONG value, const int width) {
  int localWidth = static_cast<int>(width);
  Utils::oss.str("");
  Utils::oss.setf(std::ios::right, std::ios::adjustfield);
  Utils::oss << std::setw(localWidth) << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'double', without width formatting and without
 * specified precision.
 *
 * Parameters:
 *   value - the 'double' variable to be converted and formatted.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const double value) {
  Utils::oss.str("");
  Utils::oss << value;
  return oss.str();
}

/****************************************************************
 * String-ify a 'double', with width formatting but without
 * specified precision.
 *
 * This will by default be justified on the left. It is not clear
 * that any choice of justification is better than any other for
 * this kind of formatting.
 *
 * Parameters:
 *   value - the 'double' variable to be converted and formatted.
 *   width - the width of the output field.
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const double value, const int width) {
  Utils::oss.str("");
  Utils::oss << std::setw(width) << value;

  std::string returnString = Utils::Format(oss.str(), width);
  return returnString;
}

/****************************************************************
 * String-ify a 'double', with width formatting and with
 * specified precision.
 *
 * With precision specified, we have a fixed location for the
 * decimal point and thus we should justify on the right. 
 *
 * Parameters:
 *   value - the 'double' variable to be converted and formatted.
 *   width - the width of the output field.
 *   precision - desired precision to the right of the point
 * Return: the string-ified version of 'value'
**/
std::string Utils::Format(const double value, const int width,
                          const int precision) {
  Utils::oss.str("");
  Utils::oss << std::fixed << std::setprecision(precision) << std::setw(width)
             << value;

  std::string returnString = Utils::Format(oss.str(), width, "right");
  return returnString;
}

/****************************************************************
 * Replace blanks with another character.
 * This is to allow using an underscore to make a string one string
 * instead of having embedded blanks. 
 *
 * Parameters:
 *   what - the input 'string' to trim blanks from
 * Returns:
 *   the 'what' string less any leading or trailing blanks
**/
std::string Utils::ReplaceBlanks(std::string input, char c) {
  std::string return_value;

  return_value = input;

  std::string::size_type pos = return_value.find(" ");
  while(pos != std::string::npos)
  {
    return_value[pos] = c;
    pos = return_value.find(" ");
  }

  return return_value;
}

/****************************************************************
 * Convert a string to an integer.
 *
 * Parameters:
 *   input - the input 'string' to convert from
 * Returns:
 *   the 'int' value of 'input'
**/
int Utils::StringToInteger(std::string input) {
  bool is_negative;
  int return_value = 0;
  const std::string digits = "0123456789";

//  Utils::log_stream << kTag << "string '" << input << "'\n";
//  Utils::log_stream.flush();
  is_negative = false;
  if("-" == input.substr(0,1))
  { 
//    Utils::log_stream << kTag << "negative '" << input.substr(0,1) << "'\n";
//    Utils::log_stream.flush();
    is_negative = true;
    input = input.substr(1);
  }
//  Utils::log_stream << kTag << "now pos '" << input << "'\n";
//  Utils::log_stream.flush();

  for(std::string::iterator iter = input.begin(); iter != input.end(); ++iter)
  {
    int digit = digits.find(*iter);
    if((0 > digit) || (9 < digit))
    {
      Utils::log_stream << kTag << "ERROR: string '" << input
                       << "' not a number\n";
      Utils::log_stream.flush();
      exit(0);
    }
    return_value = 10*return_value + digit;
  }

  if(is_negative) return_value = -return_value;

  return return_value;
} // int Utils::StringToInteger(string input)

/****************************************************************
 * Convert a string to a LONG.
 *
 * Parameters:
 *   input - the input 'string' to convert from
 * Returns:
 *   the 'int' value of 'input'
**/
LONG Utils::StringToLONG(std::string input) {
  LONG return_value = 0L;
  const std::string digits = "0123456789";

  for(std::string::iterator iter = input.begin(); iter != input.end(); ++iter)
  {
    int digit = digits.find(*iter);
    if((0 > digit) || (9 < digit))
    {
      Utils::log_stream << kTag << "ERROR: string '" << input
                       << "' not a number\n";
      Utils::log_stream.flush();
      exit(0);
    }
    return_value = 10*return_value + digit;
  }

  return return_value;
} // LONG Utils::StringToLONG(std::string input)

/****************************************************************
 * Call the timing function
 *
 * Parameters:
 *   timestring - the label to be put into the time log
 * Returns: the 'string' version of the timing log
**/
std::string Utils::TimeCall(const std::string time_string) {
  static double time_new = 0.0;
  return Utils::TimeCall(time_string, time_new);
}

/****************************************************************
 * Call the timing function
 *
 * Parameters:
 *   timestring - the label to be put into the time log
 * Returns: the 'string' version of the timing log
**/
std::string Utils::TimeCall(const std::string timestring, double& timeNew) {
  char s[160];
  std::string return_value;
  static bool firsttime = true;
  static double usercurrent = 0.0,userone = 0.0,usertwo = 0.0;
  static double systemcurrent = 0.0,systemone = 0.0,systemtwo = 0.0;
  static double cpupctone,cpupcttwo;
  static double TIMEsystemtotal,TIMEusertotal;
  static struct rusage rusage;
  static time_t TIMEtcurrent,TIMEtone,TIMEttotal = 0.0,TIMEttwo;

  if(firsttime)
  {
    firsttime = false;
    TIMEusertotal = 0.0;
    TIMEsystemtotal = 0.0;
    TIMEtone = time(0);
    TIMEttwo = time(0);
  }

  getrusage(RUSAGE_SELF,&rusage);

  usertwo = (double) rusage.ru_utime.tv_sec;
  usertwo += (double) rusage.ru_utime.tv_usec/1000000.0;
  systemtwo = (double) rusage.ru_stime.tv_sec;
  systemtwo += (double) rusage.ru_stime.tv_usec/1000000.0;

  usercurrent = usertwo - userone;
  systemcurrent = systemtwo - systemone;

  TIMEusertotal += usercurrent;
  TIMEsystemtotal += systemcurrent;

  TIMEttwo = time(0);
  TIMEtcurrent = TIMEttwo - TIMEtone;
  TIMEttotal += TIMEtcurrent;
  if(TIMEtcurrent != 0)
  {
    cpupctone = 100.0 * ((double)usercurrent+systemcurrent) /
                           ((double)(TIMEtcurrent));
    if(cpupctone > 100.0) cpupctone = 100.0;
  }
  else
  {
    cpupctone = 0.0;
  }

  if(TIMEttotal != 0)
  {
    cpupcttwo = 100.0 * ((double)TIMEusertotal+TIMEsystemtotal) /
                           ((double)(TIMEttotal));
    if(cpupcttwo > 100.0) cpupcttwo = 100.0;
  }
  else
  {
    cpupcttwo = 0.0;
  }

  return_value = "";
  snprintf(s,80,"\nTIME***********************************************************************\n");
  return_value += std::string(s);

  snprintf(s,80,"TIME CPU percent  %6.2f %6.2f                    %s",
             cpupctone,cpupcttwo,ctime(&TIMEttwo));
  return_value += std::string(s);

  snprintf(s,80,"TIME %-15s %10.2f u   %10.2f s   Res:%12ld\n",
                 timestring.c_str(),usercurrent,systemcurrent, rusage.ru_maxrss);
  return_value += std::string(s);

  snprintf(s,80,"TIME %-15s %10.2f u_t %10.2f s_t\n",
                 timestring.c_str(),TIMEusertotal,TIMEsystemtotal);
  return_value += std::string(s);

  snprintf(s,80,"TIME***********************************************************************\n");
  return_value += std::string(s);

  return_value += "\n\n";

  userone = usertwo;
  systemone = systemtwo;
  TIMEtone = TIMEttwo;

  timeNew = TIMEusertotal;

  return return_value;
} // std::string Utils::timecall(const std::string timestring)

/****************************************************************
 * Convert a string to all lowercase.
 *
 * Parameters:
 *   to - the input 'string' to convert 
 *   from - the output converted 'string'
 * Returns: none
**/
void Utils::ToLower(std::string& to, const std::string from) {
  static char c[1024];

  snprintf(c, 1024, "%s", from.c_str());
  for(UINT i = 0; i < from.length(); ++i)
  {
    if(isupper(c[i])) c[i] = tolower(c[i]);
  }
  to = std::string(c);
} // void Utils::toLower(std::string& to, const std::string from)

/****************************************************************
 * Trim away leading and trailing blanks.
 *
 * Parameters:
 *   what - the input 'string' to trim blanks from
 * Returns:
 *   the 'what' string less any leading or trailing blanks
**/
std::string Utils::TrimBlanks(std::string what) {
  int length;
  std::string return_value;

  return_value = what;

  // trim away leading blanks
  while(" " == return_value.substr(0,1))
  {
    length = return_value.length();
    return_value = return_value.substr(1,length);
  }

  // trim away trailing blanks
  if(!return_value.empty())
  {
    length = return_value.length();
    while(" " == return_value.substr(length-1,1))
    {
      return_value = return_value.substr(0,length-1);
      length = return_value.length();
    }
  }

  return return_value;
} // std::string Utils::trimBlanks(std::string what)

/****************************************************************
 * General function for trimming whitespace from begin and end.
 *
 * Parameters:
 *   s - the input 'string' to trim blanks from
 * Returns:
 *   the 's' string less any leading or trailing whitespace
**/
std::string Utils::Trim(std::string s) {
  size_t foundPos = std::string::npos;
  std::string return_string = s;

#ifdef EBUG3
  Utils::log_stream << kTag << "orig string: '" << return_string << "'\n";
  Utils::log_stream.flush();
#endif

  foundPos = return_string.find_first_not_of(WHITESPACE);
#ifdef EBUG3
  Utils::log_stream << kTag << "found first: " << foundPos << std::endl;
  Utils::log_stream.flush();
#endif
  if(foundPos != std::string::npos)
  {
    return_string = s.substr(foundPos);
  }
#ifdef EBUG3
  Utils::log_stream << kTag << "new string:  '" << return_string << "'\n";
  Utils::log_stream.flush();
#endif

  foundPos = return_string.find_last_not_of(WHITESPACE);
#ifdef EBUG3
  Utils::log_stream << kTag << "found last:  " << foundPos << std::endl;
  Utils::log_stream.flush();
#endif
  if(foundPos != std::string::npos)
  {
    return_string = return_string.substr(0,foundPos+1);
  }
#ifdef EBUG3
  Utils::log_stream << kTag << "new string:  '" << return_string << "'\n";
  Utils::log_stream.flush();
#endif

  return return_string;
} // std::string Utils::Trim(std::string s)

