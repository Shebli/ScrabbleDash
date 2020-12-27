/**
 * @file Letter.cpp
 * @author Shebli Anvar (shebli.anvar@gmail.com)
 * @date 2018-10-30
 *
 * This C++ source file is part of the ScrabbleDash project featuring a Scrabble dashboard.
 * Implementation of the Letter class and its nested classes.
 */

#include "Letter.h"
#include <cctype>

namespace model {

const std::map<Letter::Code, Letter::Value> Letter::valueMap
{
	{'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1}, {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8},
	{'K',10}, {'L', 1}, {'M', 2}, {'N', 1}, {'O', 1}, {'P', 3}, {'Q', 8}, {'R', 1}, {'S', 1}, {'T', 1},
	{'U', 1}, {'V', 4}, {'W',10}, {'X',10}, {'Y',10}, {'Z',10}, {JOKER_CHAR, 0}
};

const std::map<Letter::Code, Index> Letter::budgetMap
{
	{'A', 9}, {'B', 2}, {'C', 2}, {'D', 3}, {'E',15}, {'F', 2}, {'G', 2}, {'H', 2}, {'I', 8}, {'J', 1},
	{'K', 1}, {'L', 5}, {'M', 3}, {'N', 6}, {'O', 6}, {'P', 2}, {'Q', 1}, {'R', 6}, {'S', 6}, {'T', 6},
	{'U', 6}, {'V', 2}, {'W', 1}, {'X', 1}, {'Y', 1}, {'Z', 1}, {JOKER_CHAR, 2}
};

/**
 * @brief Letter::Letter Constructs a Letter object with specified ASCII code and points value.
 * @param code_ ASCII code of constructed letter.
 * @param value_ Points value of constructed letter.
 */
Letter::Letter(Letter::Code code_, Value value_)
	: code_(code_), value_(value_)
{
	validate();
}

/**
 * @brief Letter::toupper transforms letter to its upper case version.
 * @param code_ ASCII code of the letter to be checked.
 * @return Upper case of code_
 */
Letter::Code
Letter::toupper(Letter::Code code_)
{
	return static_cast<Letter::Code>(std::toupper(code_));
}

/**
 * @brief Asserts whether the capitalized version of a letter is valid, i.e. belongs to the global map of letters.
 * @param code_ ASCII code of the letter to be checked.
 * @return capitalized (upper case) version of code_
 */
Letter::Code
Letter::assertValid(Letter::Code code_)
{
	code_ = toupper(code_);
	if (budgetMap.find(toupper(code_)) == budgetMap.end())
		throw InvalidCharException(code_);

	return code_;
}

/**
 * @brief Letter::Set::Set Constructs the map (or set) of all available letters for a game.
 * @param budgetMap_ Map specifying the total available number of every letter in a single game.
 * @param valueMap_ Map specifying for each letter its points value.
 */
Letter::Set::Set(const BudgetMap& budgetMap_, const ValueMap& valueMap_)
{
	for (auto it : budgetMap_)
	{
		auto letterCount = it.second;
		while (letterCount-- > 0)
		{
			addLetter(it.first, valueMap_.at(it.first));
		}
	}
}

/**
 * @brief Letter::Set::addLetter Adds a new letter and its value to the board letter map.
 * @param letterCode The ASCII code of the letter to be added.
 * @param letterValue The points value of the added letter.
 */
void
Letter::Set::addLetter(Code letterCode, Value letterValue)
{
	letterCode = assertValid(letterCode);
	letters.emplace(letterCode, std::make_unique<Letter>(letterCode, letterValue));
}

/**
 * @brief Letter::Set::letterCount returns the number of remaining letters.
 * @param letterCode The code of the letter whose count is to be returned.
 * @return the remaining number of letters corresponding to letterCode.
 */
Index
Letter::Set::count(Letter::Code letterCode) const
{
	return letters.count(letterCode);
}

/**
 * @brief Letter::Set::letterCount returns the number of remaining letters.
 * @param letterCode The code of the letter whose count is to be returned.
 * @return the total number of remaining letters is returned.
 */
Index
Letter::Set::count() const
{
	return letters.size();
}

/**
 * @brief Letter::Set::retrieveLetter Retrieves specified letter from board letter map.
 * @param letterCode ASCII code of the letter to be retrieved.
 * @return The retrieved letter if there was any such letter in the set.
 * @throws NoMoreCharException if there are no more letters with specified ASCII code; InvalidCharException if
 *         letterCode is invalid.
 */
std::unique_ptr<Letter>
Letter::Set::retrieveLetter(Letter::Code letterCode)
{
	letterCode = assertValid(letterCode);
	auto it = letters.find(letterCode);

	if (it == letters.end())
		throw NoMoreLetterException(letterCode);

	std::unique_ptr<Letter> theLetter(it->second.release());
	letters.erase(it);
	return theLetter;
}

/**
 * @brief Letter::Set::putBackLetter Puts back letter that was refused for placement on board.
 *        This may happen because there was an attempt to place the letter on a non-empty slot.
 * @param unplacedLetter The letter that should be put back into the letter set.
 */
void
Letter::Set::putBackLetter(std::unique_ptr<Letter>&& unplacedLetter)
{
	if (unplacedLetter)
	{
		auto code = unplacedLetter->code();
		letters.emplace(code, std::move(unplacedLetter));
	}
}

/**
 * @brief Letter::Set::operator []
 * @param idx The index of the letter to be referenced
 * @return A const reference to the i-th letter (modulo the number of letters left) of the letter set.
 * @throw NoMoreLetterException if there are no letters left.
 */
const Letter&
Letter::Set::operator [] (Index idx)
{
	if (count() != 0)
	{
		idx = idx % count();
		for (const auto& l : letters)
		{
			if (idx-- == 0)
			{
				return *l.second;
			}
		}
	}
	throw NoMoreLetterException();
}

void
Letter::InvalidCharException::fillStream(std::ostream& os) const noexcept
{
	os << "Attempt to set Letter with invalid character: '" << std::dec << static_cast<int>(code)
	   << std::hex << "' (" << static_cast<int>(code) << ")";
}

void
Letter::Set::NoMoreLetterException::fillStream(std::ostream& os) const noexcept
{
	os << "No more '" << static_cast<char>(code) << "' letters left";
}

} // namespace model
