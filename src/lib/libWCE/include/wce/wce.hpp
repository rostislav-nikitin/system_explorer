#ifndef WCE_HPP
#define WCE_HPP


#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../../abstractions/wce.hpp"

namespace WildCardExpr
{
	/// @brief The class that provides a wilcard finding functionality.
	class WildCardExpr : IWildCardExpr
	{
		char const WCEXPR_NORMAL_START_CHARACTER_SET = '[';
		char const WCEXPR_NORMAL_ANY_CHARACTER = '?';
		char const WCEXPR_NORMAL_ANY_CHARACTERS = '*';
		char const WCEXPR_CHARACTER_SET_STARTED_END_CHARACTER_SET = ']';

		enum class WildCardExprState
		{
			None = 0,
			Normal = 1,
			CharacterSetStarted = 2
		};
		
		std::string _expr_str;
		std::map<WildCardExprState, std::vector<char>> _state_modifying_characters =
		{
			{WildCardExprState::Normal, {WCEXPR_NORMAL_START_CHARACTER_SET, WCEXPR_NORMAL_ANY_CHARACTER, WCEXPR_NORMAL_ANY_CHARACTERS}},
			{WildCardExprState::CharacterSetStarted, {WCEXPR_CHARACTER_SET_STARTED_END_CHARACTER_SET}}
		};
			
	
	public:
		/// @brief The constructor.
		/// @param expr_str The parameter that represents a string with wildcard expression.
		WildCardExpr(std::string expr_str);

		/// @brief The method that finds a wild cars expression within input string.
		/// @param in_str The parameter that represents an input string.
		/// @param in_pos The parameter that represents a position to start find from.
		/// @return The first position of the found wild card expression inside an input string.
		virtual size_t find(std::string const &in_str, size_t in_pos = 0) override;
	};
}

#endif