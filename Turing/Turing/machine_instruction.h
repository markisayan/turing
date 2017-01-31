#ifndef TURING_MACHINE_INSTRUCTION
#define TURING_MACHINE_INSTRUCTION

#pragma once
#include <string>

class MachineInstruction {
public:
	enum DIRECTION {
		LEFT,
		RIGHT,
		STOP
	};
private:
	std::string from_state_;
	std::string to_state_;
	char from_symbol_;
	char to_symbol_;
	MachineInstruction::DIRECTION direction_;
public:
	MachineInstruction(	const std::string & from_state,
						const std::string & to_state,
						const char from_symbol,
						const char to_symbol,
						const DIRECTION direction) :
							from_state_(from_state),
							to_state_(to_state),
							from_symbol_(from_symbol),
							to_symbol_(to_symbol),
							direction_(direction){}
						
	std::string get_from_state() const;

	std::string get_to_state() const;

	char get_from_symbol() const;

	char get_to_symbol() const;

	DIRECTION get_direction() const;


};

#endif