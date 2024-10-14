#include "pch.h"

#include "gtest/gtest.h"
#include "../BigInt/BigInt.h" // Подключите ваш файл с классом BigInt

using std::uint64_t;
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
	EXPECT_EQ(static_cast<std::string>(c), "66666");
}

TEST(BigIntArithmetic, Addition2) {
	EXPECT_TRUE(BigInt("0") + BigInt("0") == BigInt("0"));
	EXPECT_TRUE(BigInt("999") + BigInt("1") == BigInt("1000"));
	EXPECT_TRUE(BigInt("-10") + BigInt("5") == BigInt("-5"));
	EXPECT_TRUE(BigInt("100") + BigInt("-100") == BigInt("0"));
	EXPECT_TRUE(BigInt("12345678901234567890") + BigInt("98765432109876543210") == BigInt("111111111111111111100"));
}

TEST(BigIntArithmetic, Subtraction) {
	EXPECT_TRUE(BigInt("10") - BigInt("5") == BigInt("5"));
	EXPECT_TRUE(BigInt("5") - BigInt("10") == BigInt("-5"));
	EXPECT_TRUE(BigInt("0") - BigInt("10") == BigInt("-10"));
	EXPECT_TRUE(BigInt("100") - BigInt("100") == BigInt("0"));
	EXPECT_TRUE(BigInt("-10") - BigInt("-5") == BigInt("-5"));
	EXPECT_TRUE(BigInt("-5") - BigInt("-10") == BigInt("5"));
	EXPECT_TRUE(BigInt("12345678901234567890") - BigInt("98765432109876543210") == BigInt("-86419753208641975320"));
}

TEST(BigIntArithmetic, Multiplication) {
	EXPECT_TRUE(BigInt("10") * BigInt("5") == BigInt("50"));
	EXPECT_TRUE(BigInt("5") * BigInt("10") == BigInt("50"));
	EXPECT_TRUE(BigInt("0") * BigInt("10") == BigInt("0"));
	EXPECT_TRUE(BigInt("10") * BigInt("0") == BigInt("0"));
	EXPECT_TRUE(BigInt("-10") * BigInt("5") == BigInt("-50"));
	EXPECT_TRUE(BigInt("10") * BigInt("-5") == BigInt("-50"));
	EXPECT_TRUE(BigInt("-10") * BigInt("-5") == BigInt("50"));
	EXPECT_TRUE(BigInt("1234") * BigInt("5678") == BigInt("7006652"));
}

TEST(BigIntArithmetic, Division) {
	EXPECT_TRUE(BigInt("10") / BigInt("5") == BigInt("2"));
	EXPECT_TRUE(BigInt("11") / BigInt("5") == BigInt("2"));
	EXPECT_TRUE(BigInt("0") / BigInt("10") == BigInt("0"));
	EXPECT_TRUE(BigInt("100") / BigInt("-10") == BigInt("-10"));
	EXPECT_TRUE(BigInt("-100") / BigInt("10") == BigInt("-10"));
	EXPECT_TRUE(BigInt("-100") / BigInt("-10") == BigInt("10"));
	EXPECT_TRUE(BigInt("7006652") / BigInt("1234") == BigInt("5678"));

	// Дополнительные проверки для деления:
	BigInt a("100");
	BigInt b("3");
	BigInt q = a / b;
	BigInt r = a % b;
	EXPECT_TRUE(q == BigInt("33"));
	EXPECT_TRUE(r == BigInt("1"));
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
