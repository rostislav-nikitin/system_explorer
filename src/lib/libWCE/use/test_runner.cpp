#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "wild_card_expr.hpp"

// Positive case

class WildCardExprPositiveCaseFixture : public ::testing::TestWithParam<std::pair<std::string, std::string>>
{
};

TEST_P(WildCardExprPositiveCaseFixture, WhenValidThenPosIsNotNPOS)
{
	std::pair<std::string, std::string> param = GetParam();
	std::string expression = param.first;
	std::string in_str = param.second;
	size_t not_expected_pos = std::string::npos;
	// Arrage
	WildCardExpr sut(expression);
	// Act
	size_t actual_pos = sut.find(in_str);
	// Assert
	EXPECT_NE(not_expected_pos, actual_pos);
}

INSTANTIATE_TEST_SUITE_P(WhenValidThenPosIsNotNPOS,
						 WildCardExprPositiveCaseFixture,
						 ::testing::Values(
										   std::make_pair("Test string", "Test string"),
										   std::make_pair("Test string", "First Test string"),
										   std::make_pair("T?s? ?t?in?", "Test string"),
										   std::make_pair("*string", "Test string"),
										   std::make_pair("Test*", "Test string"),
										   std::make_pair("*est strin*", "Test string"),
										   std::make_pair("*t s*", "Test string"),
										   std::make_pair("T?s?*", "Test string"),
										   std::make_pair("*?tr?ng", "Test string"),
										   std::make_pair("Te?t*", "Test string"),
										   std::make_pair("T[e]st string", "Test string"),
										   std::make_pair("T[eEtys]st string", "Test string"),
										   std::make_pair("[T]est string", "Test string"),
										   std::make_pair("[T]*st string", "Test string"),
										   std::make_pair("T[e]* string", "Test string"),
										   std::make_pair("T[e]st [Ss]tring", "Test string"),
										   std::make_pair("T[e]st*[s]tring", "Test string")
										   ));



//Negative case
class WildCardExprNegativeCaseFixture : public ::testing::TestWithParam<std::pair<std::string, std::string>>
{
};


TEST_P(WildCardExprNegativeCaseFixture, WhenInvalidThenPosIsNPOS)
{
	std::pair<std::string, std::string> param = GetParam();
	std::string expression = param.first;
	std::string in_str = param.second;
	size_t expected_pos = std::string::npos;
	// Arrage
	WildCardExpr sut(expression);
	// Act
	size_t actual_pos = sut.find(in_str);
	// Assert
	EXPECT_EQ(expected_pos, actual_pos);
}


INSTANTIATE_TEST_SUITE_P(WhenInvalidThenPosIsNPOS,
						 WildCardExprNegativeCaseFixture,
						 ::testing::Values(
										   std::make_pair("Test string", "Test X string"),
										   std::make_pair("T?s? ?t?in?", "Test sXring"),
										   std::make_pair("*string", "Test Xtring"),
										   std::make_pair("Test*", "TXest string")
										   ));



TEST(TestSuite, TestOne)
{
	WildCardExpr sut("T*s*[Iiz]n?");

	size_t expected_pos = 0;
	size_t actual_pos =  sut.find("Test string");

	ASSERT_EQ(expected_pos, actual_pos);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
