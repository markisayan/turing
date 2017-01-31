#include "all_interface.h"
#include "execute_help.h"
#include "execute_exit.h"
#include "execute_add_instruction.h"
#include "execute_list_instructions.h"

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



CommandHandler::CommandHandler(MachineSimulator * machine_simulator, TuringInterface * interface1)
	:	machine_simulator_(machine_simulator), 
		interface_(interface1)
{
	commands.insert(std::make_pair("help", new ExecuteHelp(interface_, machine_simulator_)));
	commands.insert(std::make_pair("exit", new ExecuteExit(interface_, machine_simulator_)));
	commands.insert(std::make_pair("add", new ExecuteAddInstruction(interface_, machine_simulator_)));
	commands.insert(std::make_pair("list", new ExecuteListInstructions(interface_, machine_simulator_)));
}

void TuringInterfaceWinCl::start() {
	show_message("Turing Machine Simulator v0.1", false);
	show_message(std::string(50, '='), false);
	show_message("Type help for options\n", false);

	std::string input;

	while (machine_on_) {
		std::cout << "$ Turing CLI: ";
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
	if (error) 
	{
		std::cout << "Error: ";
	}

	std::cout << message << std::endl;
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

	tokens = tokenize_string(trim_string(command));

	// If the user entered nothing, just exit
	if (tokens.empty())
		return;

	// Getting the first token which should be a command name
	alias = tokens[0];
	tokens.erase(tokens.begin());

	try {
		c = commands.at(alias);
	}
	catch (std::out_of_range exception) {
		throw std::runtime_error("Command doesn't exist!");
	}

	if (c->get_total_arguments() != tokens.size()) {
		std::string message = "Wrong number of arguments. Expected " + std::to_string(c->get_total_arguments()) + " arguments. Type help if you are having trouble.";
		throw std::runtime_error(message);
	}

	c->execute(tokens);
}

