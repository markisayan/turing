#ifndef TURING_INTERFACE
#define TURING_INTERFACE

#ifdef _WIN32
#include <Windows.h>
#endif

#include "machine_simulator.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <memory>
#include <ctime>

class CommandHandler;
class CommandOperation;

class TuringInterface {
protected:
	float				animation_update_time_;
	bool				machine_on_;
	bool				handle_input_;
	MachineSimulator	* machine_simulator_;
	CommandHandler		* command_handler_;
public:
	typedef std::vector<std::vector<std::string>> TableData_T;
	TuringInterface(MachineSimulator * machine_simulator);

	void			turn_off();
	virtual void	start_handling_commands() = 0;
	virtual void	start_simulation() = 0;
	virtual void	show_message(std::string message, bool error ) = 0;
	virtual void	show_table(TableData_T data) = 0;
	virtual void	clear_screen() = 0;
	std::string		get_spaced_string(std::string tape);
	virtual			~TuringInterface();
};

class TuringInterfaceWinCl : public TuringInterface{
private:
	std::string get_head_string(const int length);
	std::string get_tape_string(const int length, const int head_position);
public:
	TuringInterfaceWinCl(MachineSimulator * machine_simulator);

	virtual void	start_handling_commands();
	virtual void	show_message(std::string message, bool error);
	virtual void	show_table(TableData_T data);
	virtual void	clear_screen();
	virtual void	start_simulation();
};

class TuringInterfaceLinuxCl : public TuringInterface {
public:
	TuringInterfaceLinuxCl(MachineSimulator * machine_simulator);

	virtual void	start_handling_commands();
	virtual void	show_message(std::string message, bool error );
	virtual void	show_table(TableData_T data);
	virtual void	clear_screen();
	virtual void	start_simulation();
};

class CommandOperation {
protected:
	int					total_arguments_;
	std::string			args_description_;
	std::string			description_;
	TuringInterface		* interface_;
	MachineSimulator	* machine_simulator_;
	CommandHandler		* command_handler_;
public:
	explicit CommandOperation(	TuringInterface		* interface1,
								MachineSimulator	* machine_simulator,
								CommandHandler		* command_handler,
								std::string			description,
								std::string			args_desciption = "",
								int					total_arguments = 0);

	int				get_total_arguments()	const;
	std::string		get_args_description()	const;
	std::string		get_description()		const;

	virtual void	execute(const std::vector<std::string> & args) = 0;
	virtual			~CommandOperation();
};

class CommandHandler {
public:
	typedef std::map<std::string, CommandOperation *> CommandMap_T;
private:
	MachineSimulator	* machine_simulator_;
	TuringInterface		* interface_;
	CommandMap_T		commands_;

	// Breaks the string into tokens/words
	std::vector<std::string>	tokenize_string(const std::string & s);
	// Removes trailing/heading spaces
	std::string					trim_string(const std::string & s) const;
	// Removes multiple spaces inbetween words
	std::string					remove_multiple_spaces(const std::string & s) const;


	template <class T>
	void register_command(const std::string & command) {
		commands_.insert(std::make_pair(command, new T(interface_, machine_simulator_, this)));
	}
public:
	CommandHandler(MachineSimulator * machine_simulator, TuringInterface * interface1);

	// If command + args combination exists, executes it
	void					evaluate_command(const std::string & command);
	const CommandMap_T &	get_commands() const;
	virtual					~CommandHandler();
};

#endif

// :^)