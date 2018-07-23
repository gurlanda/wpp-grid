#ifndef GRID_H
#define GRID_H

#include <vector>
#include "pair.h"

enum CellState {
	EMPTY,
	S_HEAD,
	S_TAIL,
	S_WIRE,
	W_HEAD,
	W_TAIL,
	W_WIRE
};

class Grid
{
	std::vector<std::vector<CellState>> cellArray;

	// The grid contains x column vectors, each of size y.
	Pair size;

public:
	// Creates a Grid with no cells
	Grid();

	// Creates a Grid with the given size containing empty cells
	Grid(size_t length, size_t height);

	// Creates a Grid with the given size containing empty cells
	Grid(Pair size);

	// Returns the size of this Grid
	Pair getSize() const { return size; }

	// Changes the size of this Grid.
	// If a resize shrinks this Grid or causes some cells to be lost, those
	// cells will be deleted.
	void resize(Pair newSize);
	void resize(size_t length, size_t height)
		{ resize(Pair(length, height)); }

	// Outputs true if and only if the given coordinates represents a valid
	// location in this Grid
	bool isValidLocation(Pair coordinates) const;
	bool isValidLocation(int x, int y) const
		{ return isValidLocation(Pair(x,y)); }

	// Operators
	// Throws out_of_range when the index is invalid
	const std::vector<CellState> operator[] (int i) const;
	std::vector<CellState> operator[] (int i);

	// Static members
	// Given a Grid, location within the Grid, and selection dimensions,
	// returns a Grid containing the requested selection.
	// If part of the selection is out of bounds, this method only returns
	// the part of the selection that's within the given Grid.
	// Throws out_of_range if the location is invalid.
	static Grid getSelection(Grid grid, Pair location, Pair dimensions);
	static Grid getSelection(Grid grid, size_t x, size_t y,
							 size_t length, size_t height)
		{ return getSelection(grid, Pair(x,y), Pair(length,height)); }

	// Given a pattern and a Grid, this method pastes the pattern into the
	// Grid at the given location and returns the result.
	// If part of the pattern lies out of bounds, this will only paste the
	// part of the pattern that lies within the given Grid.
	// Throws out_of_range if the location is invalid.
	static Grid paste(Grid grid, Grid pattern, Pair location);
	static Grid paste(Grid grid, Grid pattern, size_t x, size_t y)
		{ return paste(grid, pattern, Pair(x,y)); }

	// Given a Grid, returns another Grid based on the rules of Wireworld++
	static Grid nextState(Grid grid);
};

#endif // GRID_H
