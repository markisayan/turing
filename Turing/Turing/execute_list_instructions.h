#ifndef TURING_CLI_EXECUTE_LIST_INSTRUCTIONS_H
#define TURING_CLI_EXECUTE_LIST_INSTRUCTIONS_H

#include "all_interface.h"
#include <algorithm>

class ExecuteListInstructions : public CommandOperation {
public:
	ExecuteListInstructions(TuringInterface * interface,
		MachineSimulator * machine_simulator)
		: CommandOperation(interface, machine_simulator, 0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		std::string message;
		std::string lines = std::string(81, '-');
		const std::vector<MachineInstruction> instructions = machine_simulator_->get_instructions();

		interface_->show_message(lines, false);
		interface_->show_message("| From state\t| To state\t| From symbol\t| To symbol\t| Direction\t| ", false);
		interface_->show_message(lines, false);

		for (MachineInstruction instr : instructions) {
			message.clear();
			message += "| " + instr.get_from_state() + "\t\t| " + instr.get_to_state() + "\t\t| " + instr.get_from_symbol() + "\t\t| " + instr.get_to_symbol() + "\t\t| ";
			
			switch (instr.get_direction()) {
			case MachineInstruction::LEFT:
				message += "l";
				break;
			case MachineInstruction::RIGHT:
				message += "r";
				break;
			case MachineInstruction::STOP:
				message += "s";
				break;
			}

			message += "\t\t|";

			interface_->show_message(message, false);
		}
		interface_->show_message(lines, false);

	}
};


#endif