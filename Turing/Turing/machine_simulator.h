#ifndef TURING_MACHINE_SIMULATOR
#define TURING_MACHINE_SIMULATOR

#include "machine_instruction.h"
#include "machine_tape.h"
#include "machine_simulator.h"
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>

const std::string VERSION = "v0.9";

class MachineSimulator {
private:
	MachineTape tape_;

	std::vector<MachineInstruction> instructions_;

	std::set<std::string> from_states_;
	std::set<std::string> to_states_;

	std::string current_state_;

	std::string beginning_state_name_;
	std::string ending_state_name_;

	// Error check
	void check_for_nonexistent_state_calls_();
	bool is_suitable_instruction_(const MachineInstruction & m) const;
public:
	MachineSimulator() : beginning_state_name_("beg"), ending_state_name_("end"), current_state_("beg") {};

	std::vector<MachineInstruction> const & get_instructions() const;
	void add_instruction(const MachineInstruction & instruction);
	void delete_instruction(const int index);

	std::set<std::string> const & get_from_states() const;

	void set_tape(const std::string & chars);
	MachineTape get_tape() const;

	void set_beginning_state_name(const std::string & name);
	std::string get_beginning_state_name() const;

	void set_ending_state_name(const std::string & name);
	std::string get_ending_state_name() const;

	void restart();
	MachineInstruction next();

};

#endif

// :^)