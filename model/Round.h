#ifndef model_Round_h_INCLUDED
#define model_Round_h_INCLUDED

#include "model/Placement.h"
#include "model/Letter.h"
#include "Commons.h"
#include <deque>
#include <memory>
#include <ostream>

namespace model
{

class Slot;

class Round
{
public:
	class EmptyException;
	class FullException;
	class IllegalException;

public:
	Round(size_t id, Letter::Pool& letterPool);
	Round(size_t id, Placement placement_, Letter::Pool& letterPool);
	Round& reset();
	Round& reset(const Placement& placement_);
	Round& clear();

	const Placement& placement() const { return *checkNotEmpty().pm_placement; }
		  Placement& placement()       { return *checkNotEmpty().pm_placement; }
	const Letter::Pool& letterPool() const { return m_letterPool; }
		  Letter::Pool& letterPool()       { return m_letterPool; }
	bool isEmpty() const { return !pm_placement; }
	bool isFull() const { return m_isFull; }
	bool isLegal(bool asIs = false) const;
	Round&& checkLegal(bool asIs = false);
	const Round& checkNotEmpty() const;
		  Round& checkNotEmpty();
	const Slot& checkNotFull() const;
		  Slot& checkNotFull();
	size_t slotCount() const { return m_slotList.size(); }
	Orientation orientation() const { return placement().orientation; }

	const Slot& lastSlot() const { checkNotEmpty(); return *m_slotList.back(); }
		  Slot& lastSlot()       { checkNotEmpty(); return *m_slotList.back(); }
	const Slot& firstSlot() const { checkNotEmpty(); return *m_slotList.front(); }
		  Slot& firstSlot()       { checkNotEmpty(); return *m_slotList.front(); }
	const Letter *popSlot();
	auto begin() const { return m_slotList.begin(); }
	auto end() const { return m_slotList.end(); }

	const Letter& placeLetter(Letter::Code letterCode);
	const Letter* popLetter();
	size_t placeString(const std::string& word);

public:
	const size_t id;

private:
	std::unique_ptr<Placement> pm_placement;
	std::deque<Slot*> m_slotList;
	bool m_isFull;
	Letter::Pool& m_letterPool;
};

class Round::EmptyException : public Exception
{
public:
	explicit EmptyException(size_t numRound) : roundId(numRound) {}
	void fillStream(std::ostream& os) const noexcept override;

public:
	const size_t roundId;
};

class Round::FullException : public Exception
{
public:
	explicit FullException(size_t numRound) : roundId(numRound) {}
	void fillStream(std::ostream& os) const noexcept override;

public:
	const size_t roundId;
};

class Round::IllegalException : public Exception
{
public:
	explicit IllegalException(Placement placement) : placement(placement) {}
	void fillStream(std::ostream& os) const noexcept override;

public:
	const Placement placement;
};

} // namespace model

std::ostream& operator<< (std::ostream& os, model::Round& round);

#endif // model_Round_h_INCLUDED
