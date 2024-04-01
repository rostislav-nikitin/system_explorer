#include "../include/wce/wce.hpp"

#include <iostream>

namespace WildCardExpr
{
	WildCardExpr::WildCardExpr(std::string expr_str) : _expr_str(expr_str)
	{
	}

	size_t WildCardExpr::find(std::string const &in_str, size_t in_pos)
	{
		//	std::cout << _expr_str << "::" << in_str << std::endl;
		if(_expr_str.empty())
			return std::string::npos;

		WildCardExprState state(WildCardExprState::Normal);
		bool alt_state_any_chars = false;
		
		std::string expr_substr;
		std::string char_set;
		
		size_t first_found_pos = std::string::npos;
		
		for(size_t expr_pos = 0; expr_pos < _expr_str.size(); ++expr_pos)
		{
			char expr_current_char = _expr_str[expr_pos];
			std::vector<char> &modifiers = _state_modifying_characters[state];
			
			std::vector<char>::const_iterator expr_current_modifier = 
				std::find(modifiers.begin(), modifiers.end(), expr_current_char);
			
			if(state == WildCardExprState::Normal)
			{
				//std::cout << "NOR" << std::endl;
				if(expr_current_modifier == modifiers.end())
				{
					expr_substr.append(1, _expr_str[expr_pos]);
				}
				else
				{
					// CHECK IS BUFFER EMPTY
					if(!expr_substr.empty())
					{
						
						size_t substr_pos = in_str.find(expr_substr, in_pos);
						//std::cout << *expr_current_modifier << "::"  << expr_substr << "::" << substr_pos << "::" << in_pos << std::endl;

						if(substr_pos == std::string::npos)
						{
							//std::cout << "EXIT 79" << std::endl;
							return std::string::npos;
						}
						else
						{
							//					std::cout << substr_pos << "::" << expr_substr << "::" << in_pos << std::endl;
						
							if((substr_pos == in_pos) || alt_state_any_chars)
							{
								in_pos = substr_pos + expr_substr.size();
								expr_substr.clear();
								alt_state_any_chars = false;
							}
							else
							{
								//std::cout << "END" << std::endl;
								return std::string::npos;
							}
							
							if(first_found_pos == std::string::npos)
							{
								first_found_pos = substr_pos;
							}

						}
					}

					// MODIFIERS
					if(*expr_current_modifier == WCEXPR_NORMAL_START_CHARACTER_SET)
					{
						state = WildCardExprState::CharacterSetStarted;
						//std::cout << "CHS" << std::endl;
						//char_set.append(1, *expr_current_modifier);
					}
					else if(*expr_current_modifier == WCEXPR_NORMAL_ANY_CHARACTER)
					{
						//std::cout << "ANYCH::ENTER" << std::endl;
						if(in_pos >= in_str.size())
						{
							//std::cout << "ANYCH::OUT OF SIZE" << std::endl;
							return std::string::npos;
						}
						
						++in_pos;
					}
					else if(*expr_current_modifier == WCEXPR_NORMAL_ANY_CHARACTERS)
					{
						//std::cout << "Asterik!!!" << std::endl;
						alt_state_any_chars = true;
						if(first_found_pos == std::string::npos)
						{
							first_found_pos = in_pos;
						}

					}
				}		
						
			}
			else if(state == WildCardExprState::CharacterSetStarted)
			{
				//std::cout << "CSS" << std::endl;
				if(expr_current_modifier == modifiers.end())
				{
					char_set.append(1, _expr_str[expr_pos]);
					//std::cout << _expr_str[expr_pos] << std::endl;
				}
				else if(*expr_current_modifier == WCEXPR_CHARACTER_SET_STARTED_END_CHARACTER_SET)
				{
					//std::cout << "~CHS" << std::endl;
					size_t pos = in_str.find_first_of(char_set, in_pos);
					//				std::cout << pos << "::" << in_pos << std::endl;
					
					if((pos == std::string::npos) || ((pos != in_pos) && !alt_state_any_chars))
						return std::string::npos;
					
					if(first_found_pos == std::string::npos)
						first_found_pos = pos;

					char_set.clear();
					in_pos = pos + 1;
					state = WildCardExprState::Normal;
					alt_state_any_chars = false;

				}					
				
			}

		}

		//std::cout << "Final check!" << std::endl;
		if((state == WildCardExprState::Normal) && (expr_substr.size() > 0))
		{
			//		std::cout << "TT" << std::endl;
			//std::cout << expr_substr << "::" << in_pos << std::endl;
			size_t substr_pos = in_str.find(expr_substr, in_pos);
			//std::cout << in_str << "::" << expr_substr << "::" << substr_pos << std::endl;

			if(substr_pos != std::string::npos && ((substr_pos == in_pos) || alt_state_any_chars))
			{
				in_pos = substr_pos + expr_substr.size();

				if(first_found_pos == std::string::npos)
					first_found_pos = substr_pos;
			}
			else
			{
				return std::string::npos;
			}
		}				

		if(in_pos == in_str.size() || ((expr_substr.size() == 0) && alt_state_any_chars))
		{
			//std::cout << expr_substr << "::" << in_str << std::endl;
			return first_found_pos;
		}
		else
			return std::string::npos;
	}
}