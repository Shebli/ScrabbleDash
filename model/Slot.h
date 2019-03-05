#ifndef model_Slot_h_INCLUDED
#define model_Slot_h_INCLUDED

#include "Commons.h"
#include "model/Letter.h"
#include <exception>
#include <map>
#include <set>
#include <memory>

namespace model {

class Round;

class Slot
{
	friend class Board;

public:
	class Exception;
	class UninitializedException;
	class NotPlacedException;
	class AlreadyPlacedException;
	class OutOfBoundsException;

public:
	explicit Slot(SlotFactor factor_, Index row_, Index col_);
	~Slot();

	void checkNotPlaced() const;
	void checkIsPlaced() const;
	void checkHasNeighbor(Orientation orientation) const;
	void placeLetter(std::unique_ptr<Letter>&& aLetter, const Round& aRound);
	bool hasNeighbor(Orientation orKey) const;
		  Slot& neighbor(const Orientation& orientation);
	const Slot& neighbor(const Orientation& orientation) const;
	bool isPlaced() const;

	const Letter& letter() const;

	Index row() const;
	Index col() const;
	int irow() const { return static_cast<int>(row()); }
	int icol() const { return static_cast<int>(col()); }
	int letterFactor() const;
	int wordFactor() const;
	SlotFactor factor() const;

	const Round& placementRound() const { checkIsPlaced(); return *placementRound_; }

private:
	Index row_;
	Index col_;
	SlotFactor factor_;
	const Round* placementRound_;

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

class Slot::NotPlacedException : public Slot::Exception
{
public:
	explicit NotPlacedException(Index row, Index col) : Slot::Exception(row, col) {}
	void fillStream(std::ostream& os) const noexcept override;
};

class Slot::AlreadyPlacedException : public Slot::Exception
{
public:
	explicit AlreadyPlacedException(Index row, Index col, std::unique_ptr<Letter>&& aLetter = std::unique_ptr<Letter>())
		: Slot::Exception(row, col), refusedLetter(std::move(aLetter)) {}
	void fillStream(std::ostream& os) const noexcept override;
	std::unique_ptr<Letter> refusedLetter;
};

class Slot::OutOfBoundsException : public Slot::Exception
{
public:
	explicit OutOfBoundsException(Index row, Index col) : Slot::Exception(row, col) {}
	void fillStream(std::ostream& os) const noexcept override;
};

} // namespace model

#endif // model_Slot_h_INCLUDED
