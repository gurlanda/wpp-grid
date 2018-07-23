#include <stdexcept>
#include "grid.h"

using namespace std;

// Helper function forward declarations
Grid getNeighborhood(Grid grid, Pair location);

Grid::Grid() : size(Pair(0,0)) {

}

// Creates an empty Grid with the given size
Grid::Grid(size_t length, size_t height) : Grid(Pair(length,height)) {

}

// Creates an empty Grid with the given size
Grid::Grid(Pair size) {
	this->size = size;

	// Create an empty column vector of size height
	vector<CellState> columnVector;
	for (int j = 0; j < size.y; j++) {
		columnVector.push_back(CellState::EMPTY);
	}

	// Push the same column vector into the cellArray until it reaches the
	// correct length. We can do this since all the column vectors are
	// identical.
	for (int i = 0; i < size.x; i++) {
		cellArray.push_back(columnVector);
	}
}

// Changes the size of this Grid.
// If a resize shrinks this Grid or causes some cells to be lost, those
// cells will be deleted.
void Grid::resize(Pair newSize) {
	// Take care of the number of column vectors
	if (size.x > newSize.x) {
		// Delete column vectors until the current size matches the new size
		while (size.x > newSize.x) {
			cellArray.pop_back();
		}
	}
	else if (size.x < newSize.x) {
		// Create an empty column vector of the new size
		vector<CellState> columnVector;
		for (int i = 0; i < newSize.y; i++) {
			columnVector.push_back(CellState::EMPTY);
		}

		// Add column vectors until the current size matches the new size
		while (size.x < newSize.x) {
			cellArray.push_back(columnVector);
		}
	}

	// Take care of the size of the column vectors
	if (size.y > newSize.y) {
		// Delete cells from each column vector until they are the right size
		for (int i = 0; i < newSize.x; i++) {
			while (cellArray[i].size() > newSize.y) {
				cellArray[i].pop_back();
			}
		}
	}
	else if (size.y < newSize.y) {
		// Add empty cells to each column vector until they are the right size
		for (int i = 0; i < newSize.x; i++) {
			while (cellArray[i].size() < newSize.y) {
				cellArray[i].push_back(CellState::EMPTY);
			}
		}
	}

	// Update size
	size.x = newSize.x;
	size.y = newSize.y;
}

// Outputs true if and only if the given coordinates represents a valid
// location in this Grid
bool Grid::isValidLocation(Pair coordinates) const {
	if (coordinates.x < 0 || coordinates.x >= size.x ||
		coordinates.y < 0 || coordinates.y >= size.y) {
		return false;
	}
	else {
		return true;
	}
}


// Operators
// Throws out_of_range when the index is invalid
const std::vector<CellState> Grid::operator[] (int i) const {
	if (!isValidLocation(Pair(i, 0))) {
		throw out_of_range("In Grid::operator[] const");
	}

	return cellArray[i];
}

std::vector<CellState> Grid::operator[] (int i) const {
	if (!isValidLocation(Pair(i, 0))) {
		throw out_of_range("In Grid::operator[]");
	}

	return cellArray[i];
}


// Static members
// Given a Grid, location within the Grid, and selection dimensions,
// returns a Grid containing the requested selection.
// If part of the selection is out of bounds, this method only returns
// the part of the selection that's within the given Grid.
// Throws out_of_range if the location is invalid.
static Grid Grid::getSelection(Grid grid, Pair location, Pair dimensions) {
	if (!grid.isValidLocation(location)) {
		throw out_of_range("In Grid::getSelection");
	}

	// Calculate the dimensions of the selection within the given Grid
	Pair validDimensions;

	if (location.x + dimensions.x > grid.getSize().x) {
		validDimensions.x = grid.getSize().x - location.x;
	}
	else {
		validDimensions.x = dimensions.x;
	}

	if (location.y + dimensions.y > grid.getSize().y) {
		validDimensions.y = grid.getSize().y - location.y;
	}
	else {
		validDimensions.y = dimensions.y;
	}

	// Get the selection
	Grid selection(validDimensions);
	for (int i = 0; i < validDimensions.x; i++) {
		for (int j = 0; j < validDimensions.y; j++) {
			selection[i][j] = grid[location.x + i][location.y + j];
		}
	}

	return selection;
}

// Given a pattern and a receiving Grid, this method pastes the pattern
// into the receiver at the given location and returns the result.
// If part of the pattern lies out of bounds of the receiver, this method
// will only paste the part of the pattern that lies within the receiving
// Grid.
// Throws out_of_range if the location is invalid.
static Grid Grid::paste(Grid receiver, Grid pattern, Pair location) {
	if (!receiver.isValidLocation(location)) {
		throw out_of_range("In Grid::paste()");
	}

	// Paste the pattern into the receiving Grid
	for (int i = 0; i < pattern.getSize().x &&
					i + location.x < receiver.getSize().x; i++) {
		for (int j = 0; j < pattern.getSize().y &&
						j + location.y < receiver.getSize().y; j++) {
			receiver[location.x + i][location.y + j]
				= pattern[i][j];
		}
	}

	return receiver;
}

// Given a Grid, returns another Grid based on the rules of Wireworld++
static Grid Grid::nextState(Grid grid) {
	Grid output(grid.getSize());

	for (int i = 0; i < grid.getSize().x; i++) {
		for (int j = 0; j < grid.getSize().y; j++) {
			Grid neighborhood;
			int sHeadCount;
			int wHeadCount;

			switch(grid[i][j]) {
			case CellState::EMPTY:
				// Empty cells stay empty
				break;
			case CellState::S_HEAD:
				// Strong heads turn into strong tails
				output[i][j] = CellState::S_TAIL;
				break;
			case CellState::S_TAIL:
				// Strong tails turn into strong wire
				output[i][j] = CellState::S_WIRE;
				break;
			case CellState::W_HEAD:
				// Weak heads turn into weak tails
				output[i][j] = CellState::W_TAIL;
				break;
			case CellState::W_TAIL:
				// Weak tails turn into weak wire
				output[i][j] = CellState::W_WIRE;
				break;
			case CellState::S_WIRE:
				// Strong wire turns into a strong head if either:
				// 1) One or two of its neighbors are strong heads
				// 2) Exactly two of its neighbors are weak heads
				neighborhood = getNeighborhood(grid, Pair(i,j));

				sHeadCount = getStateCount(neighborhood, CellState::S_HEAD);
				wHeadCount = getStateCount(neighborhood, CellState::W_HEAD);

				if (sHeadCount == 1 || sHeadCount == 2 || wHeadCount == 2) {
					output[i][j] = CellState::S_HEAD;
				}
				else {
					output[i][j] = CellState::S_WIRE;
				}
				break;
			case CellState::W_WIRE:
				// Weak wire turns into a weak head if either:
				// 1) One or two of its neighbors are weak heads
				// 2) Exactly one of its neighbors is a strong head
				neighborhood = getNeighborhood(grid, Pair(i,j));

				sHeadCount = getStateCount(neighborhood, CellState::S_HEAD);
				wHeadCount = getStateCount(neighborhood, CellState::W_HEAD);

				if (wHeadCount == 1 || wHeadCount == 2 || sHeadCount == 1) {
					output[i][j] = CellState::W_HEAD;
				}
				else {
					output[i][j] = CellState::W_WIRE;
				}
				break;
			default:
				// This should never be reached
				break;
			}
		}
	}

	return output;
}

// Helper functions
// Returns the Moore neighborhood of the cell at the given location.
// Does not copy the center cell itself.
// If the cell is on the edge of the given Grid, the cells that lie out of
// bound will be empty.
// Throw out_of_range if the location is invalid.
Grid getNeighborhood(Grid grid, Pair location) {
	// Throw out_of_range if the location is invalid
	if (!grid.isValidLocation(location)) {
		throw out_of_range("In getNeighborhood()");
	}

	// Access and copy down each cell in the Moore neighborhood of the
	// given cell
	Grid output(3,3);
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!grid.isValidLocation(location.x + i, location.y + j)) {
				output[i+1][j+1] = CellState::EMPTY;
			}
			else {
				output[i+1][j+1] = grid[location.x + i][location.y + j];
			}
		}
	}

	// Do not copy the center cell.
	output[1][1] = CellState::EMPTY;

	return output;
}

// Returns the number of cells in the Grid that are in a given state
int getStateCount(Grid grid, CellState state) {
	int count = 0;
	for (int i = 0; i < grid.getSize().x; i++) {
		for (int j = 0; j < grid.getSize().y; j++) {
			if (grid[i][j] == state) {
				count++;
			}
		}
	}

	return count;
}
