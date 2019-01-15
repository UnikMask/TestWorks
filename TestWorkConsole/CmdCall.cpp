#include "CmdCall.h"
#include <iostream>
#include "learning.h"

Learn renatools; //The object of the learn class --> keeps the data imported & the parameters.

using namespace std;

string Command::kbcmd(int ls,char exclch) //To seperate command into key words.
///Finally works.
{
	char cmdchar[1024]; //The memory allocation size has to be a constant, so a high number is better against shady users, but a lower number favors memory use.
	int i = ls;
	strncpy_s(cmdchar, cmdline.c_str(), sizeof(cmdchar));
	string kwstr = "";
	while (cmdchar[i] != '\0' && cmdchar[i] != exclch)
	{
		kwstr += cmdchar[i];
		++i;
	}
	return kwstr;
	
}

void Command::rdstr() //The command reading step.
{
	std::string nwstr;
	nwstr = kbcmd(0);
	if (nwstr == "rena") //Key word that calls the machine learning algorithm commands (in developpment, completed when I'll have the necessary data).
	{
		call_rena(5);
	}
	else if (nwstr == "prompt") //Key word that calls the prompt commands (yet to be made).
	{
		call_twc(7); //Not yet developped. See CmdCall.h
	}
	else if (nwstr == "exit") //In case you want to close the prompt. Exits through looping out.
	{
		shut = true;
	}
	else // In case the written expression is not valid.
	{
		cout << nwstr << " not a valid expression. Please enter a valid expression (e.g. rena, prompt, or exit)."; //A message is wiser than silence.
	}
}

void Command::call_twc(int li) //Function for commands related to the prompt.
{
	if (kbcmd(li) == "cmdf" && kbcmd(li+5) == "=" && kbcmd(li+6, '\0') != "")
	{
		cmdf = kbcmd(li + 7, '\0');
	}
}

void Command::call_rena(int li) //Function for commands related to the R.E.N.A. (Redemption Eleven Newsoft Alpha or Real Estate Neural Architecture)
{
	if (kbcmd(li) == "learn")
	{
		renatools.grdesc();
	}
	else if (kbcmd(li) == "predict")
	{
		renatools.hypoth();
	}
	else if (kbcmd(li) == "load")
	{
		if (kbcmd(li + 5) == "")
		{
			renatools.dataload();
		}
		else
		{
			renatools.dataload(kbcmd(li+5));
		}
		
	}
	else if (kbcmd(li) == "square")
	{
		renatools.square_lr();
	}
	else
	{
		cout << "Wrong keyword or typos present. Please enter a valid command. ";
	}
}

