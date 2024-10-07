#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>

using std::uint64_t;

class BigInt {
public:
    BigInt(); // Конструктор по умолчанию
    BigInt(uint64_t count); // Конструктор из uint64_t
    BigInt(const std::string& str); // Конструктор из строки
    BigInt(const BigInt& other); // Конструктор копирования
    BigInt(BigInt&& other);

    bool less_by_module(const BigInt& other) const;

    // Базовые операции
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    BigInt operator-() const;

    // Присваивание
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other);


    // Сравнение
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    // Ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
    friend std::istream& operator>>(std::istream& is, BigInt& num);

private:
    BigInt diff_from_greater_to_less(const BigInt& other) const;
    uint64_t extract_number(std::string_view number, int pos, int count);

    std::vector<uint64_t> digits; // Вектор для хранения чисел
    bool sign; // Знак числа (true - положительное, false - отрицательное)
    int COUNT_ZERO_IN_CELL = 9;
    uint64_t BASE;
    // Вспомогательные функции
    void removeLeadingZeros();
    BigInt abs() const;
};

// Реализация конструкторов
BigInt::BigInt() : sign(true) {}

BigInt::BigInt(uint64_t count): sign(true), digits(count) {
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
}


BigInt::BigInt(const std::string& str) {
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
    sign = true;
    uint64_t start = 0ull;
    if (!str.empty() && (str[0] == '-' || str[0] == '+')) {
        sign = (str[0] == '+');
        start = 1;
    }
    else {
        for (uint64_t i = std::max(start, str.size() - 1 - COUNT_ZERO_IN_CELL);
            i >= start; i -= COUNT_ZERO_IN_CELL) {
            digits.push_back(extract_number(str, i, COUNT_ZERO_IN_CELL));
        }
    }
    removeLeadingZeros();
}

BigInt::BigInt(const BigInt& other) : digits(other.digits), sign(other.sign) 
{
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
}

BigInt::BigInt(BigInt&& other) : digits(std::move(other.digits)), sign(std::move(sign))
{
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
}

bool BigInt::less_by_module(const BigInt& other) const
{
    return std::equal(digits.begin(), digits.end(), other.digits.begin());
}

BigInt BigInt::diff_from_greater_to_less(const BigInt& y) const
{
    BigInt x(*this);
    auto length = std::max(this->digits.size(), y.digits.size());
    BigInt z(length);
    for (int ix = 0; ix < (length - 1); ix++) // проход по всем разрядам числа, начиная с последнего, не доходя до первого
    {
        if (ix < (length - 1)) // если текущий разряд чисел не первый
        {
            x.digits[ix + 1]--; // в следующуем разряде большего числа занимаем 1.
            z.digits[ix] += 10 + x.digits[ix]; // в ответ записываем сумму значения текущего разряда большего числа и 10-ти

        }
        else  // если текущий разряд чисел - первый
            z.digits[ix] += x.digits[ix]; // в ответ суммируем значение текущего разряда большего числа

        z.digits[ix] -= y.digits[ix]; // вычитаем значение текущего разряда меньшего числа

        if (z.digits[ix] / 10 > 0) // если значение в текущем разряде двухразрядное
        {
            z.digits[ix + 1]++; // переносим единицу в старший разряд
            z.digits[ix] %= 10; // в текущем разряде отсекаем ее
        }
    }
    return z;
}


uint64_t BigInt::extract_number(std::string_view number, int pos, int count) {
    count = std::min<int>(count, number.size() - pos); // корректное ограничение count

    uint64_t result = 0;
    int position_shift = (int)std::pow(10, count);
    for (int i = pos; i < pos + count; i++) {
        result += (number[i] - '0') * position_shift;
        position_shift /= 10;
    }
    return result;
}

// Реализация операторов
BigInt BigInt::operator+(const BigInt& other) const {
    if (sign == other.sign) { // Если знаки одинаковые
        auto maxSize = std::max(digits.size(), other.digits.size()) + 1;
        BigInt result(maxSize);
        for (int i = 0; i <  maxSize; i++) {
            result.digits[i] = this->digits[i] + other.digits[i];
            result.digits[i + 1] += result.digits[i] % BASE;
            result.digits[i] %= BASE;
        }
        if (result.digits.back() == 0) {
            result.digits.pop_back();
        }

        return result;
    }
    else { // Если знаки разные
        if (sign) { // this положительное
            return *this - (-other);
        }
        else { // this отрицательное
            return other - (-*this);
        }
    }
}


BigInt BigInt::operator-(const BigInt& other) const {
    if (other.sign != this->sign ) {
        return *this + other;
    }
    if (this->less_by_module(other)) {
        return other.diff_from_greater_to_less(*this);
    }
    return this->diff_from_greater_to_less(other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    auto length = this->digits.size() + other.digits.size() + 1;
    BigInt res(length);
    for (int ix = 0; ix < this->digits.size(); ix++)
        for (int jx = 0; jx < other.digits.size(); jx++)
            res.digits[ix + jx - 1] += this->digits[ix] * other.digits[jx];

    for (int ix = 0; ix < length; ix++)
    {
        res.digits[ix + 1] += res.digits[ix] / BASE;
        res.digits[ix] %= BASE;
    }

    res.removeLeadingZeros();
    return res;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other == BigInt(0)) {
        throw std::runtime_error("Division by zero");
    }
    BigInt dividend = abs();
    BigInt divisor = other.abs();
    BigInt quotient;
    BigInt remainder;

    for (uint64_t i = dividend.digits.size() - 1; i >= 0; --i) {
        remainder.digits.insert(remainder.digits.begin(), dividend.digits[i]);
        remainder.removeLeadingZeros();
        uint64_t curDigit = 0;
        while (remainder >= divisor) {
            remainder -= divisor;
            ++curDigit;
        }
        quotient.digits.insert(quotient.digits.begin(), curDigit);
    }

    quotient.sign = (sign == other.sign);
    quotient.removeLeadingZeros();
    return quotient;
}

BigInt BigInt::operator%(const BigInt& other) const {
    return *this - (*this / other) * other;
}

BigInt BigInt::operator-() const
{
    BigInt temp(*this); // Создаем копию объекта
    temp.sign = !sign; // Инвертируем знак
    return temp;     // Возвращаем копию с измененным знаком
}

// Реализация присваивания
BigInt& BigInt::operator=(const BigInt& other) {
    BigInt temp(other); // Создаем временный объект через конструктор копирования
    std::swap(*this, temp); // Обмениваем данные с временным объектом
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other)
{
    if (this != &other) {
        BigInt temp(std::move(other)); // Создаем временный объект через конструктор перемещения
        std::swap(*this, temp); // Обмениваем данные с временным объектом
    }
    return *this;
}


// Реализация сравнения
bool BigInt::operator==(const BigInt& other) const {
    return (sign == other.sign) && (std::equal(digits.begin(), digits.end(), other.digits.begin()));
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (sign != other.sign) {
        return sign < other.sign;
    }
    if (digits.size() != other.digits.size()) {
        return (digits.size() < other.digits.size()) ^ !sign;
    }
    for (uint64_t i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return (digits[i] < other.digits[i]) ^ !sign;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt& other) const {
    return !(*this <= other);
}

bool BigInt::operator<=(const BigInt& other) const {
    return (*this < other) || (*this == other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

// Реализация ввода/вывода
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (!num.sign) {
        os << '-';
    }
    for (uint64_t i = num.digits.size() - 1; i >= 0; --i) {
        os << num.digits[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str);
    return is;
}

// Реализация вспомогательных функций
void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
        sign = true; // Ноль всегда положительный
    }
}

BigInt BigInt::abs() const {
    BigInt result = *this;
    result.sign = true;
    return result;
}

uint64_t main() {
    BigInt num1(12345);
    BigInt num2("9876543210");

    std::cout << "num1: " << num1 << std::endl;
    std::cout << "num2: " << num2 << std::endl;

    BigInt sum = num1 + num2;
    std::cout << "sum: " << sum << std::endl;

    BigInt diff = num2 - num1;
    std::cout << "diff: " << diff << std::endl;

    BigInt product = num1 * num2;
    std::cout << "product: " << product << std::endl;

    BigInt quotient = num2 / num1;
    std::cout << "quotient: " << quotient << std::endl;

    BigInt remainder = num2 % num1;
    std::cout << "remainder: " << remainder << std::endl;

    return 0;
}