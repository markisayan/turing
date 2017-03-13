#ifndef TURING_MACHINE_TAPE
#define TURING_MACHINE_TAPE

#pragma once
#include <vector>
#include "machine_instruction.h"

class MachineTape {
private:
	// Head position is relative to the first non empty character
	int head_position_;
	std::string original_tape_;
	std::string working_tape_;
	// Removes all trailing and leading underscores
	std::string trim_tape_(std::string tape);
public:
	MachineTape(): head_position_(0){}
	void MachineTape::set_tape(std::string symbols);
	std::string get_original_tape() const;
	std::string get_working_tape() const;
	void reset_tape();
	char get_current_symbol() const;
	void set_current_symbol(const char symbol);
	int get_head_position() const;
	void move_head(const MachineInstruction::DIRECTION dir);
};

#endif

// :^)