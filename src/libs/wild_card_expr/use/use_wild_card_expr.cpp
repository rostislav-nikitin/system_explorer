#include <iostream>

#include "wild_card_expr.hpp"

int main()
{
	//	WildCardExpr wc_expr("[Tt]est?str*g");
	WildCardExpr wc_expr("*st string");

	std::cout << "Mathed: \t" << wc_expr.find("Test string") << std::endl;
	
	return EXIT_SUCCESS;
}
