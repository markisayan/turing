#ifndef TURING_CLI_EXECUTE_EXIT_H
#define TURING_CLI_EXECUTE_EXIT_H

#include "all_interface.h"

class ExecuteExit : public CommandOperation {
public:
	ExecuteExit(TuringInterface * interface,
		MachineSimulator * machine_simulator)
		: CommandOperation(interface, machine_simulator, 0) {}

	virtual void execute(const std::vector<std::string> & arguments) {
		interface_->turn_off();
	}
};


#endif
