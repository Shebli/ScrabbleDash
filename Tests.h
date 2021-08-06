#ifndef TESTS_H
#define TESTS_H

#include "model/Game.h"
#include <iostream>

class Tests
{
public:
	Tests(std::ostream& os = std::cout);
	void print(const std::string& msg) const;
	void printLine() const;
	void printBoard() const;
	void printUnusedLetters() const;
	void run();

private:
	std::ostream& os;
	model::Game game;
};

#endif // TESTS_H
