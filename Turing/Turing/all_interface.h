#ifndef TURING_INTERFACE
#define TURING_INTERFACE

#include "machine_simulator.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <memory>

class CommandHandler;
class CommandOperation;

class TuringInterface {
	friend class CommandOperation;
protected:
	bool machine_on_;
	MachineSimulator * machine_simulator_;
	CommandHandler * command_handler_;
public:
	TuringInterface(MachineSimulator * machine_simulator);
	void turn_off();
	virtual void start() = 0;
	virtual void show_message(std::string message, bool error ) = 0;
};

class TuringInterfaceWinCl : public TuringInterface{
public:
	TuringInterfaceWinCl(MachineSimulator * machine_simulator);
	virtual void start();
	void show_message(std::string message, bool error);
};

class TuringInterfaceLinuxCl : public TuringInterface {
public:
	TuringInterfaceLinuxCl(MachineSimulator * machine_simulator);
	virtual void start();
	void show_message(std::string message, bool error );
};

class CommandOperation {
protected:
	int total_arguments_;
	std::string args_format_;
	TuringInterface * interface_;
	MachineSimulator * machine_simulator_;
public:
	explicit CommandOperation(TuringInterface * interface,
		MachineSimulator * machine_simulator,
		int total_arguments = 0) :
		total_arguments_(total_arguments),
		interface_(interface),
		machine_simulator_(machine_simulator),
		args_format_ (""){};

	int get_total_arguments() const {
		return total_arguments_;
	}

	virtual void execute(const std::vector<std::string> & args) = 0;
};

class CommandHandler {
private:
	MachineSimulator * machine_simulator_;
	TuringInterface * interface_;
	std::map<std::string, CommandOperation *> commands;

	// Breaking the string into tokens/words
	std::vector<std::string> tokenize_string(const std::string & s);
	std::string trim_string(const std::string & s) const;
public:
	CommandHandler(MachineSimulator * machine_simulator, TuringInterface * interface1);
	void evaluate_command(const std::string & command);
};

#endif