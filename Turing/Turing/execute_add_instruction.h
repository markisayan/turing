#ifndef TURING_CLI_EXECUTE_ADD_INSTRUCTION_H
#define TURING_CLI_EXECUTE_ADD_INSTRUCTION_H

#include "all_interface.h"
#include <algorithm>

class ExecuteAddInstruction : public CommandOperation {
public:
	ExecuteAddInstruction(	TuringInterface * interface,
							MachineSimulator * machine_simulator)
		: CommandOperation(interface, machine_simulator, 5)
	{
		args_format_ = "[from_state] [to_state] [from_symbol] [to_symbol] [direction l/r/s]";
	}

	virtual void execute(const std::vector<std::string> & args) {
		MachineInstruction::DIRECTION dir;
		
		std::string from_state	= args[0];
		std::string to_state	= args[1];
		std::string from_symbol	= args[2];
		std::string to_symbol	= args[3];
		std::string direction	= args[4];

		if (from_symbol.size() != 1) {
			throw std::runtime_error("Wrong from symbol");
		}

		if (to_symbol.size() != 1) {
			throw std::runtime_error("Wrong to symbol");
		}

		if (direction.size() != 1) {
			throw std::runtime_error("Wrong direction format. Expecting l/r/s");
		}

		std::transform(direction.begin(), direction.end(), direction.begin(), ::tolower);
		
		switch (direction[0]) {
			case 'l':
				dir = MachineInstruction::LEFT;
				break;
			case 'r':
				dir = MachineInstruction::RIGHT;
				break;
			case 's':
				dir = MachineInstruction::STOP;
				break;
			default:
				throw std::runtime_error("Wrong direction. Expecting l/r/s");
		}

		MachineInstruction instruction(from_state, to_state, from_symbol[0], to_symbol[0], dir);
		machine_simulator_->add_instruction(instruction);
		interface_->show_message("Instruction added!", false);
	}
};


#endif