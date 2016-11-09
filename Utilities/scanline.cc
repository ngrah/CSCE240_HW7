#include "scanline.h"

using namespace std;

// static const string TAG = "ScanLine: ";

// ifstream Utils::inStream; // deprecated
// ofstream Utils::outStream; // deprecated
//ofstream ScanLine::zorklogStream;
//ostringstream ScanLine::zorkoss;
//stringstream ScanLine::zorkss;

//stringstream ScanLine::myss;

/****************************************************************
 * Constructor.
**/
ScanLine::ScanLine() {
}
/****************************************************************
 * Destructor.
**/
ScanLine::~ScanLine() {
}

/****************************************************************
 * General functions.
**/
/****************************************************************
 * Function 'HasMoreData'.
 *
 * Returns:
 *   true if there are ANY more characters in the input 'string'
**/
bool ScanLine::HasMoreData() {
  bool return_value = true;
#ifdef EBUGS
  Utils::logStream << TAG << "enter HasMoreData" << std::endl;
  Utils::logStream.flush();
#endif
  
  if(scanline_ss_.eof())
  {
    return_value = false;
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave HasMoreData '" << return_value << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return return_value;
} 

/****************************************************************
 * Function 'HasNext'.
 *
 * Returns:
 *   true if there are ANY more characters in the input 'string'
**/
bool ScanLine::HasNext() {
  bool return_value = true;
#ifdef EBUGS
  Utils::logStream << TAG << "enter HasNext" << std::endl;
  Utils::logStream.flush();
#endif
  
  if(scanline_ss_.eof())
  {
    return_value = false;
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave HasNext '" << return_value << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return return_value;
} 

/****************************************************************
 * Initialization. This because I can't make constructors work.
**/
void ScanLine::OpenString(std::string line) {
  std::string token;

#ifdef EBUGS
  Utils::logStream << TAG << "enter OpenString '" << line << "'" << std::endl;
  Utils::logStream.flush();
#endif
  
  scanline_ss_.clear();
  scanline_ss_.str(line);

#ifdef EBUGS
  Utils::logStream << TAG << "leave OpenString" << std::endl;
  Utils::logStream.flush();
#endif
} 

/****************************************************************
 * Function 'Next' to return the next string.
 *
 * The definition of a 'token' is anything other than whitespace.
 * In this implementation, whitespace includes newline, carriage
 * return, and tab.
 *
 * Returns:
 *   the 'string' versin of the next token
**/
string ScanLine::Next() {
  std::string token;

#ifdef EBUGS
  Utils::logStream << TAG << "enter next" << std::endl;
  Utils::logStream.flush();
#endif
  
  token = "";
  if(!scanline_ss_.eof())
  {
    scanline_ss_ >> token;
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave next '" << token << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return token;
} 

/****************************************************************
 * Function 'NextDouble' to return the next integer.
 *
 * This function does not really trap erorrs. Errors inconverting
 * from 'string' to 'int' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as an 'int'
**/
double ScanLine::NextDouble() {
  double local_double = 0.0;

#ifdef EBUGS
  Utils::logStream << TAG << "enter NextDouble" << std::endl;
  Utils::logStream.flush();
#endif
  
  if(!scanline_ss_.eof())
  {
    scanline_ss_ >> local_double;
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave NextDouble '" << nextValue << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return local_double;
} 

/****************************************************************
 * Function 'NextInt' to return the next integer.
 *
 * This function does not really trap erorrs. Errors inconverting
 * from 'string' to 'int' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as an 'int'
**/
int ScanLine::NextInt() {
  int next_value = 0;
  std::string token = "";

#ifdef EBUGS
  Utils::logStream << TAG << "enter NextInt" << std::endl;
  Utils::logStream.flush();
#endif
  
  if(!scanline_ss_.eof())
  {
    scanline_ss_ >> token;
    next_value = Utils::StringToInteger(token); 
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave NextInt '" << NextValue << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return next_value;
} 

/****************************************************************
 * Function 'NextLine' to return the rest of the line.
 *
 * This is just a wrapper for the 'getline' function.
 * Note that this does not trim whitespace at the beginning
 * or at the end.
 *
 * CAVEAT: We have put a max of 1024 characters that can be
 *         parsed with this function.
 *
 * Returns:
 *   the 'string' version of the rest of the line
**/
string ScanLine::NextLine() {
  char xx[1025];
  std::string token;

#ifdef EBUGS
  Utils::logStream << TAG << "enter NextLine" << std::endl;
  Utils::logStream.flush();
#endif
  
  if (scanline_ss_.eof())
  {
    token = "";
  }
  else
  {
    scanline_ss_.getline(xx,1024);
    token = string(xx);
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave NextLine '" << token << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return token;
} 

/****************************************************************
 * Function 'nextLONG' to return the next LONG.
 *
 * This function does not really trap errors. Errors in converting
 * from 'string' to 'LONG' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as an 'LONG'
**/
LONG ScanLine::NextLONG() {
  LONG next_value = 0;
  std::string token = "";

#ifdef EBUGS
  Utils::logStream << TAG << "enter NextLONG" << std::endl;
  Utils::logStream.flush();
#endif
  
  if(!scanline_ss_.eof())
  {
    scanline_ss_ >> token;
    next_value = Utils::StringToLONG(token); 
  }

#ifdef EBUGS
  Utils::logStream << TAG << "leave NextLONG '" << next_value << "'" << std::endl;
  Utils::logStream.flush();
#endif

  return next_value;
} 

/****************************************************************
 * Test function to read.
void ScanLine::zork()
{
  string token;

#ifdef EBUGS
  Utils::logStream << TAG << "enter zork" << endl;
  Utils::logStream.flush();
#endif
  
// while(!scanLineSS.eof())
  while(this->hasNext())
  {
    token = next();

    Utils::logStream << TAG << "mytoken '" << token << "'" << endl;
    Utils::logStream.flush();
  }
  scanLineSS.clear();

#ifdef EBUGS
  Utils::logStream << TAG << "leave zork" << endl;
  Utils::logStream.flush();
#endif
} 
**/

