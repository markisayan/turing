#include "machine_instruction.h"

std::string MachineInstruction::get_from_state() const {
	return from_state_;
}

std::string MachineInstruction::get_to_state() const {
	return to_state_;
}

char MachineInstruction::get_from_symbol() const {
	return from_symbol_;
}

char MachineInstruction::get_to_symbol() const {
	return to_symbol_;
}

MachineInstruction::DIRECTION MachineInstruction::get_direction() const {
	return direction_;
}

bool MachineInstruction::operator==(const MachineInstruction & a)
{
	return from_state_ == a.from_state_ && from_symbol_ == a.from_symbol_;
}

// :^)