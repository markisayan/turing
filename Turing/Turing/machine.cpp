#include "machine.h"

void Machine::start()
{
	turing_interface_->start_handling_commands();
}

Machine::Machine() 
	: machine_simulator_(new MachineSimulator()) 
{
	#ifdef _WIN32
		turing_interface_ = new TuringInterfaceWinCl(machine_simulator_);
	#elif __linux__
		interface_ = new InterfaceLinuxCl(machine_simulator_);
	#endif
}

// :^)