#include "all_interface.h"
#include "cli_commands.h"

TuringInterface::TuringInterface(MachineSimulator * machine_simulator)
	:	machine_simulator_(machine_simulator), 
		machine_on_(true), 
		command_handler_(new CommandHandler(machine_simulator_, this)){}

void TuringInterface::turn_off()
{
	machine_on_ = false;
}


TuringInterfaceWinCl::TuringInterfaceWinCl(MachineSimulator * machine_simulator) 
	:	TuringInterface(machine_simulator) {}

TuringInterfaceLinuxCl::TuringInterfaceLinuxCl(MachineSimulator * machine_simulator) 
	:	TuringInterface(machine_simulator) {}

void TuringInterfaceLinuxCl::start()
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
	register_command<ExecuteSetTape>("set");
	register_command<ExecuteShowTape>("show");
	register_command<ExecuteClearScreen>("clear");
	register_command<ExecuteShowBeginningState>("showbeg");
	register_command<ExecuteSetBeginningState>("setbeg");
	register_command<ExecuteShowEndingState>("showend");
	register_command<ExecuteSetEndingState>("setend");
}

void TuringInterfaceWinCl::start() {
	show_message("Turing Machine Simulator v0.1", false);
	show_message(std::string(50, '='), false);
	show_message("Type 'help' for a list of commands", false);

	std::string input;

	while (machine_on_) {
		std::cout << "\n$ Turing CLI: ";
		std::getline(std::cin, input);
		try 
		{
			command_handler_->evaluate_command(input);
		}
		catch (std::runtime_error error) 
		{
			show_message(error.what(), true);
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
	std::string line = std::string(89, '=');
	std::string row;
	
	std::cout << line << std::endl;

	for (int i = 0; i < data.size(); i++) {
		
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

std::string CommandHandler::trim_string(const std::string & s) const {
	int beg = 0, end = s.size();
	std::string res;

	for (int i = 0; i < s.size(); i++) {
		if (s[i] != ' ') {
			beg = i;
			break;
		}
	}

	for (int i = s.size() - 1; i >=0 ; i--) {
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

	for (int i = 0; i < s.size(); i++) {
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

// :^)