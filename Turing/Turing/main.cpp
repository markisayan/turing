#include "machine_simulator.h"
#include "machine.h"
#include <iostream>

using namespace std;

int main() {

	Machine m;
	m.start();

	/*
	MachineSimulator m;

	MachineInstruction i1("q0", "q1", '*', '1', MachineInstruction::LEFT);
	MachineInstruction i2("q1", "q2", '_', '_', MachineInstruction::LEFT);
	MachineInstruction i3("q2", "q3", '_', '_', MachineInstruction::LEFT);
	MachineInstruction i4("q3", "q4", '_', '_', MachineInstruction::LEFT);
	MachineInstruction i5("q4", "q5", '_', '_', MachineInstruction::LEFT);
	MachineInstruction i6("q5", "p0", '_', '*', MachineInstruction::STOP);

	m.set_beginning_state_name("q0");
	m.set_ending_state_name("p0");

	m.add_instruction(i1);
	m.add_instruction(i2);
	m.add_instruction(i3);
	m.add_instruction(i4);
	m.add_instruction(i5);
	m.add_instruction(i6);

	m.set_tape("*");

	auto a = m.get_instructions();

	for (auto x : a) {
		cout << "At state \"" << x.get_from_state() << "\" change symbol \"" << x.get_from_symbol() << "\" to symbol \"" << x.get_to_symbol() << "\", change the state to \"" << x.get_to_state() << "\" ";

		switch (x.get_direction()) {
			case MachineInstruction::RIGHT:
				cout << "and move to the right";
				break;
			case MachineInstruction::LEFT:
				cout << "and move to the left";
				break;
			case MachineInstruction::STOP:
				cout << "and stop";
				break;
		}

		cout << endl;

	}

	try {
		MachineInstruction i = m.next();

		while (i.get_to_state() != m.get_ending_state_name()) {
			i = m.next();
		}
	}
	catch (const runtime_error & ex) {
		cout << ex.what() << endl;
	}

	m.restart();


	cout << "Done";
	*/
}
