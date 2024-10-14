#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <sstream>

using std::uint64_t;

class BigInt {
public:
    BigInt(); // ����������� �� ���������
    BigInt(int64_t l);
    BigInt(uint64_t l); // ����������� �� uint64_t
    BigInt(std::string_view str); // ����������� �� ������
    BigInt(const BigInt& other); // ����������� �����������
    BigInt(BigInt&& other);

    bool less_by_module(const BigInt& other) const;

    // ������� ��������
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt operator-() const;
    const BigInt operator +() const;

    // ������������
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other);
    BigInt& operator -=(const BigInt& value);
    BigInt& operator +=(const BigInt& value);

    const BigInt operator++();
    const BigInt operator ++(int);
    const BigInt operator --();
    const BigInt operator --(int);

    // ���������
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    // ����/�����
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
    friend std::istream& operator>>(std::istream& is, BigInt& num);
    operator std::string() const;

    int COUNT_ZERO_IN_CELL = 9;
private:
    BigInt diff_from_greater_to_less(const BigInt& other) const;
    uint64_t extract_number(std::string_view number, int pos, int count);

    std::vector<uint64_t> digits; // ������ ��� �������� �����
    bool sign; // ���� ����� (true - �������������, false - �������������)
    uint64_t BASE = 1'000'000'000;
    
    // ��������������� �������
    void removeLeadingZeros();
    void _shift_right();
    BigInt abs() const;
};