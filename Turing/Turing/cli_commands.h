#ifndef TURING_CLI_COMMANDS
#define TURING_CLI_COMMANDS

#include "all_interface.h"
#include <algorithm>
#include <iterator>

class ExecuteExit : public CommandOperation {
public:
	ExecuteExit(	TuringInterface		* interface1,
					MachineSimulator	* machine_simulator,
					CommandHandler		* command_handler)
		: CommandOperation(
			interface1, 
			machine_simulator, 
			command_handler,
			"Exit the program",
			"", 
			0) {}

	virtual void execute(const std::vector<std::string> & arguments) {
		interface_->turn_off();
	}
};

class ExecuteHelp : public CommandOperation {
public:
	ExecuteHelp(	TuringInterface		* interface1,
					MachineSimulator	* machine_simulator,
					CommandHandler		* command_handler)
		: CommandOperation(
			interface1, 
			machine_simulator, 
			command_handler, 
			"Show this message",
			"", 
			0)
	{}

	virtual void execute(const std::vector<std::string> & arguments){
		const CommandHandler::CommandMap_T & commands = command_handler_->get_commands();

		interface_->show_message("All available commands", false);
		interface_->show_message("Type 'info name' to find out more about the function 'name'.", false);

		for (auto const command : commands) {
			interface_->show_message("\t" + command.first + "\t" + command.second->get_description(), false);
		}


	}
};

class ExecuteAddInstruction : public CommandOperation {
private:
	bool similar_exists(const MachineInstruction & new_instruction){

		for (auto instruction : machine_simulator_->get_instructions()) 
			if (instruction == new_instruction)
				return false;
		
		return true;
	}
public:
	ExecuteAddInstruction(TuringInterface		* interface1,
		MachineSimulator	* machine_simulator,
		CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Add an instruction to the turing machine",
			"[from_state] [to_state] [from_symbol] [to_symbol] [direction l/r/s]",
			5
		)
	{

	}

	virtual void execute(const std::vector<std::string> & args) {
		MachineInstruction::DIRECTION dir;

		std::string from_state = args[0];
		std::string to_state = args[1];
		std::string from_symbol = args[2];
		std::string to_symbol = args[3];
		std::string direction = args[4];

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

		if(similar_exists(instruction) == false)
			throw std::runtime_error("Similar instruction already exists");


		machine_simulator_->add_instruction(instruction);
		interface_->show_message("Instruction added!", false);
	}


};

class ExecuteDeleteInstruction : public CommandOperation {
public:
	ExecuteDeleteInstruction(	TuringInterface		* interface1,
								MachineSimulator	* machine_simulator,
								CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Delete instruction from the turing machine",
			"[instruction_number]",
			1
		)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		int instruction_number = stoi(args[0]);
		
		const std::vector<MachineInstruction> & instructions = machine_simulator_->get_instructions();

		if (instruction_number <= 0 || instruction_number > instructions.size()) {
			throw std::runtime_error("Instruction with that number doesn't exist");
		}

		machine_simulator_->delete_instruction(instruction_number - 1);
		interface_->show_message("Instruction deleted!", false);
	}
};




class ExecuteListInstructions : public CommandOperation {
public:
	ExecuteListInstructions(	TuringInterface		* interface1,
								MachineSimulator	* machine_simulator,
								CommandHandler		* command_handler)
		: CommandOperation(
			interface1, 
			machine_simulator, 
			command_handler, 
			"List instructions that were added to the turing machine",
			"", 
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		int counter = 1;
		TuringInterface::TableData_T data;
		std::string direction;
		data.push_back({"No", "From state", "To state", "From symbol", "To symbol", "Direction"});

		const std::vector<MachineInstruction> & instructions = machine_simulator_->get_instructions();

		for (MachineInstruction instr : instructions) {
			switch (instr.get_direction()) {
			case MachineInstruction::LEFT:
				direction = "left";
				break;
			case MachineInstruction::RIGHT:
				direction = "right";
				break;
			case MachineInstruction::STOP:
				direction = "stop";
				break;
			}

			data.push_back(
				{
					std::to_string(counter++), 
					instr.get_from_state(), 
					instr.get_to_state(), 
					std::string(1, instr.get_from_symbol()), 
					std::string(1, instr.get_to_symbol()), 
					direction 
				}
			);
			
		}
		
		interface_->show_table(data);
	}
};

class ExecuteInfo : public CommandOperation {
public:
	ExecuteInfo(	TuringInterface		* interface1,
					MachineSimulator	* machine_simulator,
					CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Display info about a command",
			"[command]",
			1)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		std::string command_str = args[0];
		const CommandHandler::CommandMap_T & commands = command_handler_->get_commands();
		auto command = commands.find(command_str);
		
		if(command == commands.end()){
			throw std::runtime_error(command_str + ": command not found");
		}

		interface_->show_message("Format: " + command->first + " " + command->second->get_args_description(), false);
		interface_->show_message("\t" + command->second->get_description(), false);
	}
};

class ExecuteSetTape : public CommandOperation {
public:
	ExecuteSetTape(	TuringInterface		* interface1,
					MachineSimulator	* machine_simulator,
					CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Set the initial contents of the tape",
			"[symbols]",
			1)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		machine_simulator_->set_tape(args[0]);
		interface_->show_message("Tape set!", false);
	}
};

class ExecuteShowTape : public CommandOperation {
public:
	ExecuteShowTape(	TuringInterface		* interface1,
						MachineSimulator	* machine_simulator,
						CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Display the initial contents of the tape",
			"",
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		const std::string tape = machine_simulator_->get_tape().get_original_tape();
		std::string spaced_tape = interface_->get_spaced_string(tape);

		interface_->show_message("Tape: [ " + spaced_tape + " ]", false);
	}
};

class ExecuteClearScreen : public CommandOperation {
public:
	ExecuteClearScreen(	TuringInterface		* interface1,
						MachineSimulator	* machine_simulator,
						CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Clear screen",
			"",
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		interface_->clear_screen();
	}
};

class ExecuteSetBeginningState : public CommandOperation {
public:
	ExecuteSetBeginningState(	TuringInterface		* interface1,
								MachineSimulator	* machine_simulator,
								CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Set beginning state name",
			"[name]",
			1)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		machine_simulator_->set_beginning_state_name(args[0]);
		interface_->show_message("Beginning state name set!", false);
	}
};

class ExecuteShowBeginningState : public CommandOperation {
public:
	ExecuteShowBeginningState(	TuringInterface		* interface1,
								MachineSimulator	* machine_simulator,
								CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Show beginning state name",
			"",
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		interface_->show_message("Beggining state name: '" + machine_simulator_->get_beginning_state_name() + "'", false);
	}
};

class ExecuteSetEndingState : public CommandOperation {
public:
	ExecuteSetEndingState(	TuringInterface		* interface1,
							MachineSimulator	* machine_simulator,
							CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Set ending state name",
			"[name]",
			1)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		machine_simulator_->set_ending_state_name(args[0]);
		interface_->show_message("Ending state name set!", false);
	}
};

class ExecuteShowEndingState : public CommandOperation {
public:
	ExecuteShowEndingState(	TuringInterface		* interface1,
							MachineSimulator	* machine_simulator,
							CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Show ending state name",
			"",
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		interface_->show_message("Ending state name: '" + machine_simulator_->get_ending_state_name() + "'", false);
	}
};

class ExecuteStartSimulation : public CommandOperation {
public:
	ExecuteStartSimulation(	TuringInterface		* interface1,
							MachineSimulator	* machine_simulator,
							CommandHandler		* command_handler)
		: CommandOperation(
			interface1,
			machine_simulator,
			command_handler,
			"Start simulating the Turing Machine",
			"",
			0)
	{}

	virtual void execute(const std::vector<std::string> & args) {
		interface_->start_simulation();
	}
};


#endif

// :^)