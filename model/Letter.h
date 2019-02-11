#ifndef model_Letter_h_INCLUDED
#define model_Letter_h_INCLUDED

#include "Commons.h"
#include <map>
#include <deque>
#include <memory>

namespace model {

class Letter
{
public:
	class Set;
	class InvalidCharException;
	typedef std::map<char,int> ValueMap;
	typedef std::map<char,Index> BudgetMap;
	typedef std::deque<std::unique_ptr<Letter>> Queue;

public:
	/// This chat value represents the 'Joker" Scrabble letter
	static const char JOKER_CHAR = '*';
	static const char JOKER_PLACED = '.';
	static const char JOKER_NULL = '\0';

	/// This is the map of the number of value points attributed to each Scrabble letter.
	static const ValueMap valueMap;

	/// The is the map of the total number of letters available for each Scrabble letter.
	static const BudgetMap budgetMap;

public:
	static char toupper(char code_);
	static char assertValid(char code_);

public:
	Letter(char code_ = JOKER_NULL, int value_ = 0);
	char code() const { return code_; }
	int value() const { return value_; }
	void validate() { assertValid(code()); }
	bool isNull() const { return code() == JOKER_NULL; }

private:
	char code_;
	int value_;
};

class Letter::Set
{
public:
	class NoMoreLetterException;

public:
	Set(const BudgetMap& budgetMap_ = Letter::budgetMap, const ValueMap& valueMap_ = Letter::valueMap);
	Index count(char letterCode = JOKER_NULL) const;
	void addLetter(char letterCode, int letterValue);
	void addLetter(std::unique_ptr<Letter>&& aLetter);
	std::unique_ptr<Letter> retrieveLetter(char letterCode);

private:
	std::deque<std::unique_ptr<Letter::Queue>> letterQueues;
	std::map<char, Letter::Queue*> letters;
};

class Letter::InvalidCharException : public Exception
{
public:
	explicit InvalidCharException(char code) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const char code;
};

class Letter::Set::NoMoreLetterException : public Exception
{
public:
	explicit NoMoreLetterException(char code) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const char code;
};

} // namespace model

#endif // model_Letter_h_INCLUDED
