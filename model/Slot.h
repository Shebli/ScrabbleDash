#ifndef model_Slot_h_INCLUDED
#define model_Slot_h_INCLUDED

#include "Commons.h"
#include "model/Letter.h"
#include <exception>
#include <map>
#include <set>
#include <memory>

namespace model {

class Slot
{
	friend class Board;

public:
	class Exception;
	class UninitializedException;
	class EmptyException;
	class AlreadyPlacedException;
	class OutOfBoundsException;

public:
	explicit Slot(SlotFactor factor_, Index row_, Index col_);
	~Slot();

	void placeLetter(std::unique_ptr<Letter>&& nextLetter);
		  Slot* neighbor(const Orientation& orientation);
	const Slot* neighbor(const Orientation& orientation) const;
	bool isPlaced() const;
	bool hasNeighbor(Orientation orKey) const;

	const Letter& letter() const;

	Index row() const;
	Index col() const;
	int irow() const { return static_cast<int>(row()); }
	int icol() const { return static_cast<int>(col()); }
	int letterFactor() const;
	int wordFactor() const;
	SlotFactor factor() const;

private:
	Index row_;
	Index col_;
	SlotFactor factor_;

	std::map<Orientation, Slot*> neighbor_;
	std::unique_ptr<Letter> letter_;
};

class Slot::Exception : public ::Exception
{
public:
	explicit Exception(Index row, Index col) : row(row), col(col) {}
	virtual ~Exception();

public:
	const Index row;
	const Index col;
};

class Slot::UninitializedException : public Slot::Exception
{
public:
	explicit UninitializedException(Index row, Index col) : Slot::Exception(row, col) {}
	void fillStream(std::ostream& os) const noexcept override;
};

class Slot::EmptyException : public Slot::Exception
{
public:
	explicit EmptyException(Index row, Index col) : Slot::Exception(row, col) {}
	void fillStream(std::ostream& os) const noexcept override;
};

class Slot::AlreadyPlacedException : public Slot::Exception
{
public:
	explicit AlreadyPlacedException(Index row, Index col, std::unique_ptr<Letter>&& aLetter)
		: Slot::Exception(row, col), letter(std::move(aLetter)) {}
	void fillStream(std::ostream& os) const noexcept override;
	std::unique_ptr<Letter> letter;
};

class Slot::OutOfBoundsException : public Slot::Exception
{
public:
	explicit OutOfBoundsException(Index row, Index col) : Slot::Exception(row, col) {}
	void fillStream(std::ostream& os) const noexcept override;
};

} // namespace model

#endif // model_Slot_h_INCLUDED
