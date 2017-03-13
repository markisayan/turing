#include "all_interface.h"
#include "cli_commands.h"

TuringInterface::TuringInterface(MachineSimulator * machine_simulator)
	:	machine_simulator_(machine_simulator), 
		machine_on_(true), 
		handle_input_(true),
		command_handler_(new CommandHandler(machine_simulator_, this)),
		animation_update_time_(.5){}

void TuringInterface::turn_off()
{
	machine_on_ = false;
}

TuringInterface::~TuringInterface()
{
	delete 
		machine_simulator_, 
		command_handler_;
}


std::string TuringInterface::get_spaced_string(std::string string)
{
	std::string spaced_tape;

	for (int i = 0; i < string.size(); ++i) {
		spaced_tape.push_back(string[i]);
		if (i < string.size() - 1) {
			spaced_tape.push_back(' ');
		}
	}

	return spaced_tape;
}

TuringInterfaceWinCl::TuringInterfaceWinCl(MachineSimulator * machine_simulator)
	:	TuringInterface(machine_simulator) {}

TuringInterfaceLinuxCl::TuringInterfaceLinuxCl(MachineSimulator * machine_simulator) 
	:	TuringInterface(machine_simulator) {}

void TuringInterfaceLinuxCl::start_handling_commands()
{
}

void TuringInterfaceLinuxCl::show_message(std::string message, bool error)
{
}

void TuringInterfaceLinuxCl::show_table(TableData_T data)
{
}

void TuringInterfaceLinuxCl::clear_screen()
{
}

void TuringInterfaceLinuxCl::start_simulation()
{
}

CommandHandler::CommandHandler(MachineSimulator * machine_simulator, TuringInterface * interface1)
	:	machine_simulator_(machine_simulator), 
		interface_(interface1)
{
	register_command<ExecuteHelp>("help");
	register_command<ExecuteExit>("exit");
	register_command<ExecuteAddInstruction>("add");
	register_command<ExecuteListInstructions>("list");
	register_command<ExecuteInfo>("info");
	register_command<ExecuteDeleteInstruction>("delete");
	register_command<ExecuteSetTape>("settape");
	register_command<ExecuteShowTape>("showtape");
	register_command<ExecuteClearScreen>("clear");
	register_command<ExecuteShowBeginningState>("showbeg");
	register_command<ExecuteSetBeginningState>("setbeg");
	register_command<ExecuteShowEndingState>("showend");
	register_command<ExecuteSetEndingState>("setend");
	register_command<ExecuteStartSimulation>("start");
}

void TuringInterfaceWinCl::start_handling_commands() {
	show_message("Turing Machine Simulator v0.1", false);
	show_message(std::string(50, '='), false);
	show_message("Type 'help' for a list of commands", false);

	std::string input;

	while (machine_on_) {
		if (handle_input_ == true) {
			std::cout << "\n$ Turing CLI: ";
			std::getline(std::cin, input);
			try
			{
				command_handler_->evaluate_command(input);
			}
			catch (std::runtime_error error)
			{
				show_message(error.what(), true);
				handle_input_ = true;
			}
		}
	}
}

void TuringInterfaceWinCl::show_message(std::string message, bool error = false) {
	if (error) {
		std::cout << "Error: ";
	}

	std::cout << message << std::endl;
}

// TODO: Change this pls 
void TuringInterfaceWinCl::show_table(TableData_T data)
{
	std::string line(89, '=');
	std::string row;
	
	std::cout << line << std::endl;

	for (int i = 0; i < data.size(); ++i) {
		
		for (int j = 0; j < data[i].size(); j++) {
			row += "| " + data[i][j] + "\t";
			if (i > 0 && j > 0) {
				row += "\t";
			}
		}

		row += "|";
		
		std::cout << row << std::endl;

		if (i == 0) {
			std::cout << line << std::endl;
		}

		row.clear();
	}

	std::cout << line << std::endl;
}

void TuringInterfaceWinCl::clear_screen()
{
	system("CLS");
}

std::string TuringInterfaceWinCl::get_head_string(const int max_length) {
	std::string head = "v";

	for (int i = 0; i < max_length/2 + 1; ++i) {
		head = "  " + head;
	}

	return head;
}

std::string TuringInterfaceWinCl::get_tape_string(const int visible_tape_size, const int head_position) {

	std::string result;

	std::string tape = machine_simulator_->get_tape().get_working_tape();

	int
		tape_size = tape.size(),
		beg, 
		end;

	if (visible_tape_size % 2 == 0)
		beg = head_position - visible_tape_size / 2 - 1;
	else
		beg = head_position - visible_tape_size / 2;

	end = head_position + visible_tape_size / 2;

	result += "[";

	if (beg < 0 && end >= 0)
		result += std::string(std::abs(beg), '_') + tape.substr(0, end + 1);
	
	if (beg >= 0 && beg <= tape_size - 1 && end >= 0)
		result += tape.substr(beg, visible_tape_size);
	
	if (beg <= tape_size - 1 && end >= tape_size) {
		if (result == "[")
			result += tape.substr(std::abs(beg), visible_tape_size - (end - (tape_size - 1)));
		result += std::string(std::abs(end - (tape_size - 1)), '_');
	}
	
	if (end < 0 || beg >= tape_size)
		result += std::string(visible_tape_size, '_');

	/*
	HEADING UNDERSCORES
	----------------------
	If the part that is shown has pointer position that's < 0 we show "_" characters
	before the tape

	Ex:
	tape: 1 2 3 3 4
	visible tape size: 7
	head position: 1

	              v
	Result: _ _ 1 2 3 3 4
	*/

	

	result += "]";

	return result;

}

void TuringInterfaceWinCl::start_simulation()
{
	int head_position = machine_simulator_->get_tape().get_head_position();
	time_t last_update = 0;

	// Rendering the same tape goes in two stages
	// We render the way the tape was changed
	// Then we render the new tape

	// Change character is used to determine which stage is going to be rendered next
	bool change_character = false;
	MachineInstruction instruction;

	std::string
		head_string,
		tape_string;


	// The tape gets reset and the position of the head is set to 0
	machine_simulator_->restart();

	int visible_tape_size = 5;

	// We turn off the normal input handler
	handle_input_ = true;

	// Let's clear the screen and start rendering
	clear_screen();
	

	// Infinite loop cause why the heck not
	// This is horrible tho
	while (true) {

		// We stop the simulation when the user preses "s"
		
		if (GetKeyState('S') & 0x8000)
		{
			// And we start the normal input handler
			handle_input_ = true;
			break;
		}

		if ((clock() - last_update) / (double)CLOCKS_PER_SEC >= animation_update_time_) {
			
			// First of all we start rendering from the top of the screen
			// So we set the cursor to the top of the screen
			COORD coord = { 0 };
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(h, coord);

			std::cout << "Simulating machine..." << std::endl;
			std::cout << "Press 's' to stop" << std::endl;
			std::cout << std::string(50, '=') << std::endl;

			if (change_character == false) {
				head_string = get_head_string(visible_tape_size);
				head_position = machine_simulator_->get_tape().get_head_position();
				
			}
			else {
				instruction = machine_simulator_->next(); 
				if (head_position < 0 && instruction.get_from_symbol() == '_' && instruction.get_to_symbol() != '_')
					head_position = 0;
			}

			tape_string = get_spaced_string(get_tape_string(visible_tape_size, head_position));

			std::cout << head_string << std::endl;
			std::cout << tape_string << std::endl;

			change_character = !change_character;

			if (instruction.get_to_state() == machine_simulator_->get_ending_state_name()) {
				std::cout << "Done!" << std::endl;
				handle_input_ = true;
				break;
			}

			last_update = clock();
		}
	}
}

std::string CommandHandler::trim_string(const std::string & s) const {
	int beg = 0, end = s.size();
	std::string res;

	for (int i = 0; i < s.size(); ++i) {
		if (s[i] != ' ') {
			beg = i;
			break;
		}
	}

	for (int i = s.size() - 1; i >=0 ; --i) {
		if (s[i] != ' ') {
			end = i;
			break;
		}
	}

	res = s.substr(beg, end - beg + 1);
	return res;
}

std::string CommandHandler::remove_multiple_spaces(const std::string & s) const
{
	bool prev_char_is_space = false;
	std::string new_str;

	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == ' ') {
			if (prev_char_is_space == false) {
				new_str.push_back(s[i]);
				prev_char_is_space = true;
			}
		}
		else {
			new_str.push_back(s[i]);
			prev_char_is_space = false;
		}
	}

	return new_str;
}

std::vector<std::string> CommandHandler::tokenize_string(const std::string & s) {
	std::vector<std::string> tokens;
	std::string token;
	std::stringstream ss;

	ss.str(s);
	while (std::getline(ss, token, ' ')) {
		tokens.push_back(token);
	}

	return tokens;
}

void CommandHandler::evaluate_command(const std::string & command) {
	CommandOperation * c;

	std::vector<std::string> tokens;
	std::string alias;

	tokens = tokenize_string(remove_multiple_spaces(trim_string(command)));

	// If the user entered nothing, just exit
	if (tokens.empty())
		return;

	// Getting the first token which should be a command name
	// Ex. info
	alias = tokens[0];
	tokens.erase(tokens.begin());

	try {
		c = commands_.at(alias);
	}
	catch (std::out_of_range exception) {
		throw std::runtime_error(alias + ": command not found");
	}

	if (c->get_total_arguments() != tokens.size()) {
		std::string message = "Wrong number of arguments. Expected " + std::to_string(c->get_total_arguments()) + " arguments. Type 'info " + alias +"' if you are having trouble.";
		throw std::runtime_error(message);
	}

	c->execute(tokens);
}

const CommandHandler::CommandMap_T & CommandHandler::get_commands() const
{
	return commands_;
}

CommandHandler::~CommandHandler()
{
	delete 
		interface_, 
		machine_simulator_;
}

CommandOperation::CommandOperation(	TuringInterface		* interface1, 
									MachineSimulator	* machine_simulator, 
									CommandHandler		* command_handler,
									std::string			description,
									std::string			args_description,
									int					total_arguments)
		:	total_arguments_	(total_arguments),
			interface_			(interface1),
			command_handler_	(command_handler),
			machine_simulator_	(machine_simulator),
			description_		(description),
			args_description_	(args_description) {}


int CommandOperation::get_total_arguments() const
{
	return total_arguments_;
}

std::string CommandOperation::get_args_description() const
{
	return args_description_;
}

std::string CommandOperation::get_description() const
{
	return description_;
}

CommandOperation::~CommandOperation()
{
	delete 
		interface_, 
		machine_simulator_, 
		command_handler_;
}

// :^)