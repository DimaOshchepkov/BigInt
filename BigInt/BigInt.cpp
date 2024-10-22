#include "BigInt.h"
#include <iomanip>

using std::uint64_t;


// Реализация конструкторов
BigInt::BigInt() : sign(true), digits(1, 0) {}


BigInt::BigInt(int64_t l) {
    if (l < 0) { this->sign = false; l = -l; }
    else this->sign = true;
    do {
        this->digits.push_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(uint64_t l) {
    this->sign = true;
    do {
        this->digits.push_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(std::string str) {
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

        for (long long i = str.length(); i > 0; i -= COUNT_ZERO_IN_CELL) {
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
} 

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)), sign(std::move(other.sign))
{
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
    if (!this->sign) {
        if (!other.sign) return -(-*this + (-other));
        else return other - (-*this);
    }
    else if (!other.sign) return *this - (-other);

    auto maxSize = std::max(digits.size(), other.digits.size()) + 1;
    BigInt result;
    result.digits.resize(maxSize);
    uint64_t i = 0;
    for (; i < std::min(digits.size(), other.digits.size()); i++) {
        result.digits[i] += this->digits[i] + other.digits[i];
        result.digits[i + 1] += result.digits[i] / BASE;
        result.digits[i] %= BASE;
    }
    for (; i < this->digits.size(); i++) {
        result.digits[i] += this->digits[i];
        result.digits[i + 1] += result.digits[i] / BASE;
        result.digits[i] %= BASE;
    }
    for (; i < other.digits.size(); i++) {
        result.digits[i] += other.digits[i];
        result.digits[i + 1] += result.digits[i] / BASE;
        result.digits[i] %= BASE;
    }
    if (result.digits.back() == 0) {
        result.digits.pop_back();
    }

    return result;

}


BigInt BigInt::operator-(const BigInt& other) const {
    if (!other.sign) return *this + (-other);
    else if (!this->sign) return -(-(*this) + other);
    else if (*this < other) return -(other - *this);
    BigInt left(*this);
    for (uint64_t i = 0; i < other.digits.size(); ++i) {
        if (left.digits[i] < other.digits[i]) {
            uint64_t pos_not_zero = i + 1;
            while (left.digits[pos_not_zero] == 0) {
                left.digits[pos_not_zero] = BigInt::BASE - 1;
                pos_not_zero++;
            }
            left.digits[pos_not_zero]--;
            left.digits[i] = left.digits[i] + BigInt::BASE - other.digits[i];
        }
        else {
            left.digits[i] -= other.digits[i];
        }

    }

    left.removeLeadingZeros();
    return left;
}

BigInt BigInt::operator*(const BigInt& other) const {
    auto length = this->digits.size() + other.digits.size() + 1;
    BigInt res;
    res.digits.resize(length);
    for (int ix = 0; ix < this->digits.size(); ix++) {
        for (int jx = 0; jx < other.digits.size(); jx++) {
            res.digits[ix + jx] += this->digits[ix] * other.digits[jx];
            uint64_t carry_index = 1;
            while (res.digits[ix + jx + carry_index - 1] > BigInt::BASE) {
                res.digits[ix + jx + carry_index] += res.digits[ix + jx + carry_index - 1] / BigInt::BASE;
                res.digits[ix + jx + carry_index - 1] %= BigInt::BASE;
                carry_index++;
            }

        }
    }

    res.sign = !(this->sign ^ other.sign);
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

BigInt& BigInt::operator=(BigInt& other) {  
    std::swap(digits, other.digits);
    std::swap(sign, other.sign);
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept
{
    digits = std::move(other.digits);
    sign = std::move(other.sign);
    return *this;
}



// Реализация сравнения
bool BigInt::operator==(const BigInt& other) const {
    return (sign == other.sign) && (digits ==  other.digits);
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (sign != other.sign) {
        return sign < other.sign;
    }
    if (digits.size() != other.digits.size()) {
        if (sign)
            return (digits.size() < other.digits.size());
        else
            return (digits.size() > other.digits.size());
    }
    for (int64_t i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            if (sign)
                return (digits[i] < other.digits[i]);
            else
                return (digits[i] > other.digits[i]);
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

std::string BigInt::str() const {
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

void BigInt::abs() {
    this->sign = true;
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
        int64_t x = 0, l = 0, r = BigInt::BASE;
        while (l <= r) {
            int64_t m = (l + r) / 2;
            BigInt t = other * m;
            t.abs();
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result.digits[i] = x;
        current = current - other * BigInt(x);
    }

    result.sign = (this->sign == other.sign);
    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator%(const BigInt& right) const {
    auto a = (*this) / right;
    auto b = ((*this) / right) * right;
    BigInt result = *this - b;
    if (!result.sign) result += right;
    return result;
}

BigInt _pow(const BigInt& base, std::uint32_t n) {
    if (n == 0) {
        return BigInt(1ull);
    }
    else if (n % 2 == 0) {
        return _pow(base * base, n / 2);
    }
    else if (n % 2 == 1) {
        return base * _pow(base * base, (n - 1) / 2);
    }
    return BigInt("-1");
}

BigInt BigInt::pow(std::uint32_t p) const {
    return _pow(*this, p);
}
