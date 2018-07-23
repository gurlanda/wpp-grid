#include <iostream>
#include <string>
#include "grid.h"

using namespace std;

char toString(CellState state) {
	switch(state) {
	case CellState::EMPTY:
		return ' ';
	case CellState::S_HEAD:
		return 'O';
	case CellState::S_TAIL:
		return 'o';
	case CellState::S_WIRE:
		return '#';
	case CellState::W_HEAD:
		return '0';
	case CellState::W_TAIL:
		return '.';
	case CellState::W_WIRE:
		return '+';
	default:
		// This should never be reached
		return '?';
	}
}

string toString(Grid grid) {

}

Grid figureEight() {
	Grid output(12,12);

	// Draw the wires
	output[0][5] = CellState::S_WIRE;
	output[1][5] = CellState::S_WIRE;
	output[2][5] = CellState::S_WIRE;
	output[3][5] = CellState::S_WIRE;
	output[4][5] = CellState::S_WIRE;

	output[7][6] = CellState::S_WIRE;
	output[8][6] = CellState::S_WIRE;
	output[9][6] = CellState::S_WIRE;
	output[10][6] = CellState::S_WIRE;
	output[11][6] = CellState::S_WIRE;

	output[0][0] = CellState::S_WIRE;
	output[1][0] = CellState::S_WIRE;
	output[2][0] = CellState::S_WIRE;
	output[3][0] = CellState::S_WIRE;
	output[4][0] = CellState::S_WIRE;
	output[5][0] = CellState::S_WIRE;

	output[6][11] = CellState::S_WIRE;
	output[7][11] = CellState::S_WIRE;
	output[8][11] = CellState::S_WIRE;
	output[9][11] = CellState::S_WIRE;
	output[10][11] = CellState::S_WIRE;
	output[11][11] = CellState::S_WIRE;

	output[6][0] = CellState::S_WIRE;
	output[6][1] = CellState::S_WIRE;
	output[6][2] = CellState::S_WIRE;
	output[6][3] = CellState::S_WIRE;
	output[6][4] = CellState::S_WIRE;

	output[5][7] = CellState::S_WIRE;
	output[5][8] = CellState::S_WIRE;
	output[5][9] = CellState::S_WIRE;
	output[5][10] = CellState::S_WIRE;
	output[5][11] = CellState::S_WIRE;

	output[0][1] = CellState::S_WIRE;
	output[0][2] = CellState::S_WIRE;
	output[0][3] = CellState::S_WIRE;
	output[0][4] = CellState::S_WIRE;

	output[11][7] = CellState::S_WIRE;
	output[11][8] = CellState::S_WIRE;
	output[11][9] = CellState::S_WIRE;
	output[11][10] = CellState::S_WIRE;

	// Draw the intersection
	output[5][5] = CellState::W_WIRE;
	output[5][6] = CellState::W_WIRE;
	output[6][5] = CellState::W_WIRE;
	output[6][6] = CellState::W_WIRE;

	// Draw the spark
	output[1][5] = CellState::S_TAIL;
	output[2][5] = CellState::S_HEAD;

	return output;
}

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
