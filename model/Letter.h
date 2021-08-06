#ifndef model_Letter_h_INCLUDED
#define model_Letter_h_INCLUDED

#include "Commons.h"
#include <map>
#include <memory>
#include <ostream>

namespace model {

class Letter
{
public:
	using Code = int;
	using Value = int;
	using Token = std::unique_ptr<Letter>;
	class Pool;
	class InvalidCharException;
	using ValueMap = std::map<Code,Value>;
	using BudgetMap = std::map<Code,Index>;

public:
	/// This char value represents the 'Joker" Scrabble letter
	static const Code JOKER_CHAR = '*';
	static const Code JOKER_PLACED = '.';
	static const Code JOKER_NULL = '\0';

	/// This is the map of the number of value points attributed to each Scrabble letter.
	static const ValueMap valueMap;

	/// The is the map of the total number of letters available for each Scrabble letter.
	static const BudgetMap budgetMap;

public:
	static Code toupper(Code code_);
	static Code assertValid(Code code_, bool allowNull = false);

public:
	explicit Letter(Code code_ = JOKER_NULL, Value value_ = 0);
	auto code() const { return code_; }
	auto value() const { return value_; }
	char charCode() const { return  static_cast<char>(code()); }
	void validate() { assertValid(code(), true); }
	bool isNull() const { return code() == JOKER_NULL or code() == JOKER_PLACED; }

private:
	Code code_;
	Value value_;
};

class Letter::Pool
{
public:
	class NoMoreLetterException;

public:
	Pool(const BudgetMap& budgetMap_ = Letter::budgetMap, const ValueMap& valueMap_ = Letter::valueMap);
	Index count(Code letterCode) const;
	Index count() const;
	void addLetter(Code letterCode, Value letterValue);
	Token retrieveLetter(Code letterCode);
	bool isAvailable(Code letterCode);
	void checkAvailable(Code letterCode);
	const Letter& putBackLetter(Token&& unplacedLetter);
	const Letter& operator [] (Index i) const;

private:
	std::multimap<Code, Letter::Token> letters;
};

class Letter::InvalidCharException : public Exception
{
public:
	explicit InvalidCharException(Code code) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const Code code;
};

class Letter::Pool::NoMoreLetterException : public Exception
{
public:
	explicit NoMoreLetterException(Code code = Letter::JOKER_NULL) : code(code) {}
	void fillStream(std::ostream& os) const noexcept;

public:
	const Code code;
};

} // namespace model

std::ostream& operator<< (std::ostream& os, const model::Letter& aLetter);

#endif // model_Letter_h_INCLUDED
