#pragma once
#include <string>


class Command
{
public:
	bool shut = false; //The authorisation for the prompt to shut.
	std::string cmdline, cmdf = ">>"; //the command line.

	void call_rena(int li); //To be developped, to call functions related to the machine learning algorithms.

	void call_twc(int li); //To be developped, to call functions related to the interface.

	std::string kbcmd(int ls,char exclch = ' '); //Function to seperate words or things in the keyboard input.

	void rdstr();
};
