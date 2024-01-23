// Main file, where the main function is. If something's wrong, always check
// there first and get deeper into the code from there. Do touch and tinker
// stuff if you'd like to.
#include "CmdCall.h"
#include "learning.h"
#include <iostream>

void consoleintro();
std::string cmdf, commandio(std::string cmda), mystr;
Command command; // The object of the command class.

int main() // The main action.
{
  command.cmdline = "load re_nysn.txt";
  command.call_rena(
      0); // Initialize all parameters since the beginning to avoid user
          // mistakes by tapping learn or predict before load.

  consoleintro();
  while (command.shut == false) {

    command.cmdline = commandio(command.cmdf); // Write the input.
    command.rdstr(); // Read the input and respond to the input.
  }
  return 0;
}

inline void consoleintro() // A small introduction to the batch.
{
  std::string space = "--------------------------------------------------------"
                      "----------------";
  std::cout << "\r" << space << "\n"
            << "Welcome to Unik's RENA."
            << "\n";
  std::cout << "@2018. No Rights Reserved. See the cmdoc.txt file for list of "
               "commands."
            << "\n"
            << space; /// The cmdoc file is yet to be made.
}

inline std::string
commandio(std::string cmda) // All the steps of the input face.
{
  std::string thestr;
  std::cout << "\n" << cmda;
  std::getline(std::cin, thestr);
  return thestr;
}
