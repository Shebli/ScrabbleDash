#ifndef model_Letter_h_INCLUDED
#define model_Letter_h_INCLUDED

#include "Commons.h"
#include <map>
#include <memory>

namespace model {

class Letter
{
public:
	typedef int Code;
	typedef int Value;
	class Set;
	class InvalidCharException;
	typedef std::map<Code,Value> ValueMap;
	typedef std::map<Code,Index> BudgetMap;

public:
	/// This chat value represents the 'Joker" Scrabble letter
	static const Code JOKER_CHAR = '*';
	static const Code JOKER_PLACED = '.';
	static const Code JOKER_NULL = '\0';

	/// This is the map of the number of value points attributed to each Scrabble letter.
	static const ValueMap valueMap;

	/// The is the map of the total number of letters available for each Scrabble letter.
	static const BudgetMap budgetMap;

public:
	static Code toupper(Code code_);
	static Code assertValid(Code code_);

public:
	Letter(Code code_ = JOKER_NULL, Value value_ = 0);
	auto code() const { return code_; }
	auto value() const { return value_; }
	char charCode() const { return  static_cast<char>(code()); }
	void validate() { assertValid(code()); }
	bool isNull() const { return code() == JOKER_NULL; }

private:
	Code code_;
	Value value_;
};

class Letter::Set
{
public:
	class NoMoreLetterException;

public:
	Set(const BudgetMap& budgetMap_ = Letter::budgetMap, const ValueMap& valueMap_ = Letter::valueMap);
	Index count(Code letterCode) const;
	Index count() const;
	void addLetter(Code letterCode, Value letterValue);
	std::unique_ptr<Letter> retrieveLetter(Code letterCode);
	void putBackLetter(std::unique_ptr<Letter>&& unplacedLetter);
	const Letter& operator [] (Index i);

private:
	std::multimap<Code, std::unique_ptr<Letter>> letters;
};

class Letter::InvalidCharException : public Exception
{
public:
	explicit InvalidCharException(Code code) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const Code code;
};

class Letter::Set::NoMoreLetterException : public Exception
{
public:
	explicit NoMoreLetterException(Code code = Letter::JOKER_NULL) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const Code code;
};

} // namespace model

#endif // model_Letter_h_INCLUDED
