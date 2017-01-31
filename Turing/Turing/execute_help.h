#ifndef TURING_CLI_EXECUTE_HELP_H
#define TURING_CLI_EXECUTE_HELP_H

#include "all_interface.h"

class ExecuteHelp : public CommandOperation {
public:
	ExecuteHelp(TuringInterface * interface,
				MachineSimulator * machine_simulator) 
		:	CommandOperation(interface, machine_simulator, 0) 
	{
	}

	virtual void execute(const std::vector<std::string> & arguments) {
		interface_->show_message("Help message", false);
	}
};


#endif
