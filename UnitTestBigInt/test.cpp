#include "pch.h"

#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_int.hpp>
#include "../BigInt/BigInt.h" // Подключите ваш файл с классом BigInt

using std::uint64_t;
using namespace boost::multiprecision;

// Тесты конструкторов
TEST(BigIntConstructors, DefaultConstructor) {
	BigInt a;
	EXPECT_EQ(a, 0ull);
}

TEST(BigIntConstructors, Uint64Constructor) {
	BigInt a(1234567890ull);
	EXPECT_EQ(a, 1234567890ull);
}

TEST(BigIntConstructors, EmptyConstructor) {
	BigInt a{};
	EXPECT_EQ(a, 0ull);
}

TEST(BigIntConstructors, Uint64ZeroConstructor) {
	BigInt a(0ull);
	EXPECT_EQ(a, 0ull);
}
TEST(BigIntConstructors, StringConstructor) {
	BigInt a("123456789012345678901234567890");
	EXPECT_EQ(static_cast<std::string>(a), "123456789012345678901234567890");
}

TEST(BigIntConstructors, CopyConstructor) {
	BigInt a("1234567890");
	BigInt b(a);
	EXPECT_EQ(a, b);
}

TEST(BigIntConstructors, MoveConstructor) {
	BigInt a("1234567890");
	BigInt b(std::move(a));
	EXPECT_EQ(static_cast<std::string>(b), "1234567890");
}

// Тесты операторов сравнения
TEST(BigIntComparison, Equal) {
	BigInt a("12345");
	BigInt b("12345");
	EXPECT_TRUE(a == b);
}

TEST(BigIntComparison, NotEqual) {
	BigInt a("12345");
	BigInt b("54321");
	EXPECT_TRUE(a != b);
}

TEST(BigIntComparison, LessThan) {
	BigInt a("12345");
	BigInt b("12346");
	EXPECT_TRUE(a < b);
}

// ... (добавьте тесты для >, <=, >=)

// Тесты арифметических операций
TEST(BigIntArithmetic, Addition) {
	BigInt a("12345");
	BigInt b("54321");
	BigInt c = a + b;

	cpp_int a_boost("12345");
	cpp_int b_boost("54321");
	cpp_int c_boost = a_boost + b_boost;

	EXPECT_EQ(static_cast<std::string>(c), c_boost.str());
}

TEST(BigIntArithmetic, Addition2) {
	EXPECT_EQ(static_cast<std::string>(BigInt("0") + BigInt("0")),
		(cpp_int("0") + cpp_int("0")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("999") + BigInt("1")),
		(cpp_int("999") + cpp_int("1")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-10") + BigInt("5")),
		(cpp_int("-10") + cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("100") + BigInt("-100")),
		(cpp_int("100") + cpp_int("-100")).str());

	EXPECT_EQ(static_cast<std::string>(BigInt("12345678901234567890") + BigInt("98765432109876543210")),
		(cpp_int("12345678901234567890") + cpp_int("98765432109876543210")).str());
	auto res = BigInt("1000000000") + BigInt("900000000");
	EXPECT_EQ(static_cast<std::string>(BigInt("1000000000") + BigInt("900000000")),
		(cpp_int("1000000000") + cpp_int("900000000")).str());
}

TEST(BigIntArithmetic, Subtraction) {
	EXPECT_EQ(static_cast<std::string>(BigInt("10") - BigInt("5")), (cpp_int("10") - cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("5") - BigInt("10")), (cpp_int("5") - cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("0") - BigInt("10")), (cpp_int("0") - cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("100") - BigInt("100")), (cpp_int("100") - cpp_int("100")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-10") - BigInt("-5")), (cpp_int("-10") - cpp_int("-5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-5") - BigInt("-10")), (cpp_int("-5") - cpp_int("-10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("12345678901234567890") - BigInt("98765432109876543210")), (cpp_int("12345678901234567890") - cpp_int("98765432109876543210")).str());
}

TEST(BigIntArithmetic, Multiplication) {
	EXPECT_EQ(static_cast<std::string>(BigInt("10") * BigInt("5")), (cpp_int("10") * cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("5") * BigInt("10")), (cpp_int("5") * cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("0") * BigInt("10")), (cpp_int("0") * cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("10") * BigInt("0")), (cpp_int("10") * cpp_int("0")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-10") * BigInt("5")), (cpp_int("-10") * cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("10") * BigInt("-5")), (cpp_int("10") * cpp_int("-5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-10") * BigInt("-5")), (cpp_int("-10") * cpp_int("-5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("1234") * BigInt("5678")), (cpp_int("1234") * cpp_int("5678")).str());
}

TEST(BigIntArithmetic, Division) {

	EXPECT_EQ(static_cast<std::string>(BigInt("10") / BigInt("5")), (cpp_int("10") / cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("11") / BigInt("5")), (cpp_int("11") / cpp_int("5")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("0") / BigInt("10")), (cpp_int("0") / cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("100") / BigInt("-10")), (cpp_int("100") / cpp_int("-10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-100") / BigInt("10")), (cpp_int("-100") / cpp_int("10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("-100") / BigInt("-10")), (cpp_int("-100") / cpp_int("-10")).str());
	EXPECT_EQ(static_cast<std::string>(BigInt("7006652") / BigInt("1234")), (cpp_int("7006652") / cpp_int("1234")).str());

	BigInt a("100");
	BigInt b("3");
	BigInt q = a / b;
	BigInt r = a % b;

	cpp_int a_boost("100");
	cpp_int b_boost("3");
	cpp_int q_boost = a_boost / b_boost;
	cpp_int r_boost = a_boost % b_boost;

	EXPECT_EQ(static_cast<std::string>(q), q_boost.str());
	EXPECT_EQ(static_cast<std::string>(r), r_boost.str());
}
// ... (добавьте тесты для -, *, /, %, ++, --, +=, -=)

// Тесты ввода/вывода
TEST(BigIntIO, Output) {
	BigInt a("12345");
	std::stringstream ss;
	ss << a;
	EXPECT_EQ(ss.str(), "12345");
}

TEST(BigIntIO, Input) {
	BigInt a;
	std::stringstream ss("12345");
	ss >> a;
	EXPECT_EQ(static_cast<std::string>(a), "12345");
}


// ... (добавьте тесты для less_by_module, diff_from_greater_to_less, 
//                      extract_number, removeLeadingZeros, _shift_right, abs) 
