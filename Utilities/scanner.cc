#include "scanner.h"

/****************************************************************
 * Constructor.
**/
Scanner::Scanner() {
  scanline_.OpenString("");

  std::string the_next = scanline_.Next();
}

/****************************************************************
 * Destructor.
**/
Scanner::~Scanner() {
}

/****************************************************************
 * Accessors and mutators.
**/

/****************************************************************
 * General functions.
**/
/****************************************************************
 * Function to close the stream.
**/
void Scanner::Close() {
  Utils::FileClose(local_stream_);
}

/****************************************************************
 * Function for testing for more data in the file.
 *
 * Returns:
 *   true if there is a 'next' of anything in the file. 
**/
bool Scanner::HasNext() {
  bool return_value = true;

//  std::cout << TAG << "enter HasNext" << std::endl;

  if (scanline_.HasNext()) {
//    std::cout << TAG << "scanLine HasNext true" << std::endl;
//    std::cout << TAG << "leave HasNext" << std::endl;
    return true;
  }
  else {
    bool got_answer = false;
    while (!got_answer) {
//      std::cout << TAG << "scanLine HasNext false" << std::endl;
      std::string next_line = this->NextLine();
//      std::cout << TAG << "next line is " << NextLine << std::endl;
      next_line = Utils::TrimBlanks(next_line);
      if (0 == next_line.length()) {
//        std::cout << TAG << "next line exists and is blank " << NextLine << std::endl;
//        std::cout << TAG << "leave HasNext" << std::endl;
//        return false;
        if (local_stream_.eof()) return false;
      }
      else {
//        std::cout << TAG << "next line exists and is not blank " << NextLine << std::endl;
        scanline_.OpenString(next_line);
//        std::cout << TAG << "leave HasNext" << std::endl;
        got_answer = true;
        return true;
      }
    } // while (!got_answer)

  }

//  std::cout << TAG << "leave HasNext" << std::endl;

  return return_value;
} // bool Scanner::HasNext()

/****************************************************************
 * Function for returning a next token as a string.
 *
 * The definition of a 'token' is anything other than whitespace.
 * In this implementation, whitespace includes newline, carriage
 * return, and tab.
 *
 * We have two booleans to keep track of three states:
 *   have not yet found NWS (token beginning)
 *   have not yet found WS after finding NWS (token end)
 *
 * We read a character.
 * If there is no next character to be read
 *    Then we throw an error
 * While we are not at end of file:
 *    Test the char for NWS
 *    If NWS
 *       Set the bool that we have found NWS
 *       If we have found WS after NWS, 
 *          put back the NWS--it's the start of the next token
 *          break
 *       Else append the NWS to the token
 *    Else (i.e., if WS)
 *       If we have ever found NWS
 *          Set the bool that we have found WS after NWS
 *    Read the next character.
 *
 * Returns:
 *   the 'string' version of the next token.
**/
std::string Scanner::Next() {

  std::string return_value = scanline_.Next();

  return return_value;
} // string Scanner::Next()

/****************************************************************
 * Function for returning a next character.
 *
 * Returns:
 *   the next character in the file
char Scanner::nextChar()
{
  char c = this->localStream.get();
  return c;
} // char Scanner::nextChar()
**/

/****************************************************************
 * Function for returning a next 'double'.
 *
 * This function does not trap errors. Errors in converting from
 * 'string' to 'double' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as a 'double'
**/
double Scanner::NextDouble() {
  double return_value;

  std::string next_token = this->Next();
  return_value = atof(next_token.c_str());

  return return_value;
} // double Scanner::NextDouble()

/****************************************************************
 * Function for returning the next 'int' value.
 *
 * This function does not really trap errors. Errors in converting
 * from 'string' to 'int' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as an 'int'
**/
int Scanner::NextInt() {
  int return_value;
  std::string next_token;

  return_value = 0;
  next_token = this->Next();

  return_value = Utils::StringToInteger(next_token);

  return return_value;
} // int Scanner::NextInt()

/****************************************************************
 * Function for returning the rest of the line as a string.
 *
 * This is just a wrapper for the 'getline' function.
 * Note that this does not trim whitespace at the beginning
 * or at the end.
 *
 * Returns:
 *   the 'string' version of the rest of the line
**/
std::string Scanner::NextLine() {
  std::string return_value;

//  std::cout << TAG << "enter NextLine" << std::endl;

  if (scanline_.HasNext())
  {
    return_value = scanline_.NextLine();
//    std::cout << TAG << "scanLine hasNext " << return_value << std::endl;
  }
  else
  {
    getline(local_stream_, return_value);
//    std::cout << TAG << "scanLine needed new line " << return_value << std::endl;
  }

//  std::cout << TAG << "leave NextLine" << std::endl;
  return return_value;
} // string Scanner::NextLine()

/****************************************************************
 * Function for returning the next 'LONG' value.
 *
 * This function does not really trap errors. Errors in converting
 * from 'string' to 'LONG' simply cause crashing.
 *
 * Returns:
 *   the next token in the file, parsed as an 'LONG'
**/
LONG Scanner::NextLONG() {
  LONG return_value;
  std::string next_token;

  return_value = 0;
  next_token = this->Next();

  return_value = Utils::StringToLONG(next_token);

  return return_value;
} // LONG Scanner::NextLONG()

/****************************************************************
 * Function to open a file as a 'Scanner'.
**/
void Scanner::OpenFile(std::string filename) {
  Utils::FileOpen(local_stream_, filename);
}

