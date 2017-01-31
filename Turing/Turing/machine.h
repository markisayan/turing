#ifndef TURING_MACHINE
#define TURING_MACHINE

#include "machine_simulator.h"
#include "all_interface.h"

class Machine {
private:
	MachineSimulator * machine_simulator_;
	TuringInterface * turing_interface_;
public:
	Machine();
	void start();
};

#endif