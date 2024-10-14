#include "BigInt.h"
#include <iomanip>

using std::uint64_t;


// Реализация конструкторов
BigInt::BigInt() : sign(true), digits(1, 0) {}


BigInt::BigInt(int64_t l) {
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
    if (l < 0) { this->sign = false; l = -l; }
    else this->sign = true;
    do {
        this->digits.push_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(uint64_t l) {
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
    this->sign = true;
    do {
        this->digits.push_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(std::string_view str) {
    BASE = (uint64_t)std::pow(10, COUNT_ZERO_IN_CELL);
    if (str.length() == 0) {
        this->sign = true;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->sign = false;
        }
        else {
            this->sign = true;
        }

        for (long long i = str.length(); i >= 0; i -= COUNT_ZERO_IN_CELL) {
            if (i < COUNT_ZERO_IN_CELL)
                this->digits.push_back(std::stoi(std::string(str.substr(0, i))));
            else
                this->digits.push_back(std::stoi(std::string(str.substr(i - COUNT_ZERO_IN_CELL, COUNT_ZERO_IN_CELL))));
        }

        this->removeLeadingZeros();
    }
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
        BigInt result;
        result.digits.resize(maxSize);
        for (int i = 0; i <  maxSize - 1; i++) {
            result.digits[i] = this->digits[i] + other.digits[i];
            result.digits[i + 1] += result.digits[i] / BASE;
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



BigInt BigInt::operator-() const
{
    BigInt temp(*this); // Создаем копию объекта
    temp.sign = !sign; // Инвертируем знак
    return temp;     // Возвращаем копию с измененным знаком
}

const BigInt BigInt::operator +() const {
    return BigInt(*this);
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
    for (int64_t i = digits.size() - 1; i >= 0; --i) {
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


std::ostream& operator <<(std::ostream& os, const BigInt& bi) {
    if (bi.digits.empty()) os << 0;
    else {
        if (bi.sign == false) os << '-';
        os << bi.digits.back();
        // следующие числа нам нужно печатать группами по 9 цифр
        // поэтому сохраним текущий символ-заполнитель, а потом восстановим его
        char old_fill = os.fill('0');
        for (int64_t i = static_cast<int64_t>(bi.digits.size()) - 2; i >= 0; --i) {
            os << std::setw(bi.COUNT_ZERO_IN_CELL) << bi.digits[i];
        }

        os.fill(old_fill);
    }

    return os;
}

BigInt::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
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


BigInt& BigInt::operator +=(const BigInt& value) {
    return *this = (*this + value);
}

BigInt& BigInt::operator -=(const BigInt& value) {
    return *this = (*this - value);
}

const BigInt BigInt::operator++() {
    return (*this += 1ull);
}

const BigInt BigInt::operator ++(int) {
    *this += 1ull;
    return *this - 1ull;
}

const BigInt BigInt::operator --() {
    return *this -= 1ull;
}

const BigInt BigInt::operator --(int) {
    *this -= 1ull;
    return *this + 1ull;
}

void BigInt::_shift_right() {
    this->digits.insert(this->digits.begin(), 0ull);
    this->removeLeadingZeros();
}

BigInt BigInt::operator/(const BigInt& other) const {
    // на ноль делить нельзя
    if (other == 0ull) 
        throw std::runtime_error("Divide by zero");
    
    BigInt result, current;
    result.digits.resize(this->digits.size());
    for (int64_t i = static_cast<int64_t>(this->digits.size() - 1); i >= 0; i--) {
        current._shift_right();
        current.digits[0] = this->digits[i];
        current.removeLeadingZeros();
        uint64_t x = 0, l = 0, r = BigInt::BASE;
        while (l <= r) {
            uint64_t m = (l + r) / 2;
            BigInt t = (*this) * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result.digits[i] = x;
        current = current - (*this) * x;
    }

    result.sign = this->sign != other.sign;
    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator%(const BigInt& right) const {
    BigInt result = (*this) - ((*this) / right) * right;
    if (!result.sign) result += right;
    return result;
}
