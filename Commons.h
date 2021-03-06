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

	template<typename T>
	void incr(T& row, T& col) const;

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

template<typename T>
void Orientation::incr(T& row, T& col) const
{
	switch (key())
	{
	case ORKEY_DOWN:
		++row;
		break;

	case ORKEY_RIGHT:
		++col;
		break;

	case ORKEY_UP:
		--row;
		break;

	case ORKEY_LEFT:
		--col;
		break;
	}
}

std::ostream& operator<< (std::ostream& os, const Orientation& orientation);

struct Commons
{
	static const Index MAX_PLAYERS = 4;
	static const Index MAX_LETTERS_PER_TURN = 7;
	static const Index MAX_LETTERS_PER_WORD = 15;
	static const Index TURN_TIME_BUDGET = 180;
};

enum SlotFactor : unsigned
{
	REGULAR       = 1<<16 | 1,
	LETTER_DOUBLE = 1<<16 | 2,
	LETTER_TRIPLE = 1<<16 | 3,
	WORD_DOUBLE	  = 2<<16 | 1,
	WORD_TRIPLE   = 3<<16 | 1
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
