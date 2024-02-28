#include <string>
#include <vector>
#include <map>
#include <algorithm>

class WildCardExpr
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
	WildCardExpr(std::string expr_str);

	size_t find(std::string const &in_str, size_t in_pos = 0);
};
