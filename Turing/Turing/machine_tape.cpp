#include "machine_tape.h"
#include <math.h>

std::string MachineTape::trim_tape_(std::string tape)
{
	// Deleting all leading underscores
	while (true) {
		if (!tape.empty() && tape[0] == '_') {
			tape = tape.substr(1, tape.size());
		} else {
			break;
		}
	}

	// Deleting all trailing underscores
	while (true) {
		if (!tape.empty() && tape[tape.size() - 1] == '_') {
			tape = tape.substr(0, tape.size() - 1);
		} else {
			break;
		}
	}

	return tape;
}

void MachineTape::set_tape(std::string symbols){
	head_position_ = 0;
	symbols = trim_tape_(symbols);
	working_tape_ = original_tape_ = symbols;
}

std::string MachineTape::get_original_tape() const
{
	return original_tape_;
}

void MachineTape::reset_tape() {
	set_tape(original_tape_);
}

/*

	Since the Turing machine's tape can be infinitely long,
	I decided not use an array for storing symbols, since might fill the memory pretty fast(depending on the Turing Machine).
	Instead, I only store a part of the tape that starts with the first non-empty 
	character and ends with the last non-empty character. 

	Example: * _ _ * 1 1 *

	Head's position is always relative to the leftmost non-empty character on the tape.
	It can be a negative number. So using the previous example, the head's 
	position of -3 would mean the following

		  Head
			v
			_  _  _  *  _  _  *  1  1  * 
		   -3 -2 -1  0  1  2  3  4  5  6
	
	
	But we do not store the empty spaces, so it won't take up too much memory. The only
	thing that is stored is the head's position.
	So what if we decide to replace an empty space with an asterisk at position of -3?
	We just add 3 empty characters from the left, and replace the first one with an 
	asterisk. Also, we want to change the head's position, since it's relative
	to the first non empty character its new position is going to be 0.
	So we will end up with something like this.


		  Head
			v
			*  _  _  *  _  _  *  1  1  * 
		    0  1  2  3  4  5  6  7  8  9

	
*/


void MachineTape::set_current_symbol(const char symbol)
{
	if (head_position_ < 0) {
		if(symbol != '_'){
			std::string empty_space(abs(head_position_), '_');
			working_tape_ = empty_space + working_tape_;
			working_tape_[0] = symbol;
			head_position_ = 0;
		}
	}
	else if (head_position_ >= working_tape_.size()) {
		if (symbol != '_') {
			const int num_of_spaces = head_position_ - working_tape_.size() + 1;

			std::string empty_space(num_of_spaces, '_');
			working_tape_ += empty_space;
			working_tape_[head_position_] = symbol;
		}
	}
	else {
		working_tape_[head_position_] = symbol;

		if (head_position_ == 0 && symbol == '_') {
			working_tape_ = trim_tape_(working_tape_);
			head_position_ = -1;
		}
	}

	working_tape_ = trim_tape_(working_tape_);
}

char MachineTape::get_current_symbol() const
{
	if (head_position_ < 0) {
		return '_';
	}
	else if (head_position_ >= working_tape_.size()) {
		return '_';
	}
	else {
		return working_tape_[head_position_];
	}
}

void MachineTape::move_head(const MachineInstruction::DIRECTION dir)
{
	switch (dir) {
		case MachineInstruction::DIRECTION::LEFT:
			head_position_--;
			break;
		case MachineInstruction::DIRECTION::RIGHT:
			head_position_++;
			break;
	}
}

// :^)