#ifndef Commons_h_INCLUDED
#define Commons_h_INCLUDED

#include <iostream>
#include <exception>
#include <sstream>

typedef size_t Index;

class Orientation
{
public:
	Orientation(const Orientation& o) : key_(o.key()) {}
	Orientation& operator= (const Orientation& o) { key_=o.key(); return *this; }
	Orientation operator- () const { return Orientation(opposite[key_]); }
	Orientation operator~ () const { return Orientation(cmplment[key_]); }
	bool operator== (const Orientation& o) const { return key_==o.key(); }
	bool operator< (const Orientation& o) const { return key_ < o.key(); }
	int key() const { return key_; }
	void incr(Index& row, Index& col) const;

public:
	static const int ORKEY_RIGHT = 0;
	static const int ORKEY_UP = 1;
	static const int ORKEY_LEFT = 2;
	static const int ORKEY_DOWN = 3;
	static const Orientation RIGHT;
	static const Orientation UP;
	static const Orientation LEFT;
	static const Orientation DOWN;

private:
	Orientation(int key_) : key_(key_) {}

private:
	int key_;
	static const int cmplment[];
	static const int opposite[];
};

std::ostream& operator<< (std::ostream& os, const Orientation& orientation);

struct Commons
{
	static const Index MAX_PLAYERS = 4;
	static const Index MAX_LETTERS_PER_TURN = 7;
	static const Index MAX_LETTERS_PER_WORD = 15;
	static const Index TURN_TIME_BUDGET = 180;
};

class Exception : public std::exception
{
public:
	const std::string& msg() const noexcept;
	const char* what() const noexcept override;

protected:
	virtual void fillStream(std::ostream& os) const noexcept = 0;

private:
	mutable std::string msg_;
};


#endif // Commons_h_INCLUDED
