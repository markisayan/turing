#include "machine_simulator.h"
#include "machine_instruction.h"
#include <algorithm>
#include <iterator>

// Checking if there are going to be calls to states that weren't by user 
void MachineSimulator::check_for_nonexistent_state_calls_() {
	std::set<std::string> res;

	std::set_difference(to_states_.begin(),
		to_states_.end(),
		from_states_.begin(),
		from_states_.end(),
		std::inserter(res, res.end()));

	if (!res.empty()) {
		throw std::runtime_error("Calling states that do not exist!");
	}
}

void MachineSimulator::set_tape(const std::string & chars)
{
	tape_.set_tape(chars);
	current_state_ = beginning_state_name_;
}

MachineTape MachineSimulator::get_tape() const
{
	return tape_;
}

const std::vector<MachineInstruction> & MachineSimulator::get_instructions() const {
	return instructions_;
}

void MachineSimulator::add_instruction(const MachineInstruction & instruction) {
	instructions_.push_back(instruction);
	from_states_.insert(instruction.get_from_state());
	to_states_.insert(instruction.get_to_state());
}

void MachineSimulator::delete_instruction(const int index)
{
	if (index < 0 || index > instructions_.size() - 1) {
		throw std::runtime_error("Wrong index");
	}

	instructions_.erase(instructions_.begin() + index);
}

std::set<std::string> const & MachineSimulator::get_from_states() const {
	return from_states_;
}

void MachineSimulator::restart()
{

	tape_.reset_tape();
	current_state_ = beginning_state_name_;
}

bool MachineSimulator::is_suitable_instruction_(const MachineInstruction & m) const {
	// Checking if this is the instruction we were searching for
	const char symbol = tape_.get_current_symbol();
	return m.get_from_state() == current_state_ && m.get_from_symbol() == symbol;
}

MachineInstruction MachineSimulator::next()
{
	// Let's see if the suitable instruction exists in our table
	// If it does we do our tape manipulations and the return the instruction
	for (auto instruction : instructions_) {
		if (is_suitable_instruction_(instruction)) {
			tape_.set_current_symbol(instruction.get_to_symbol());
			tape_.move_head(instruction.get_direction());
			current_state_ = instruction.get_to_state();

			return instruction;
		}
	}

	// This gets thrown if user haven't listed what to do in case of the current state-symbol combination 
	throw std::runtime_error("Machine ended unexpcetedly");
}

void MachineSimulator::set_beginning_state_name(const std::string & name)
{
	beginning_state_name_ = name;
}

std::string MachineSimulator::get_beginning_state_name() const
{
	return beginning_state_name_;
}

void MachineSimulator::set_ending_state_name(const std::string & name)
{
	ending_state_name_ = name;
}

std::string MachineSimulator::get_ending_state_name() const
{
	return ending_state_name_;
}

// :^)