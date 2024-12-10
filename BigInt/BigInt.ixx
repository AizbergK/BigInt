export module BigInt;

#include <climits>
import std;

enum sign{ positive = 1, negative = -1 };
//uint32
typedef std::uint32_t digit_type;
const std::uint64_t BASE = (std::uint64_t)UINT_MAX + 1;
const std::uint64_t BASE_MAX = (std::uint64_t)UINT_MAX;


static const int __UINT8_MAX = 256;

export class BigInt
{
public:
	//Constructors
	BigInt();
	BigInt(long long number);
	BigInt(std::string number);

	//Copy and Move constructors and assignments
	BigInt(const BigInt& bi);
	BigInt& operator=(const BigInt& bi);
	BigInt(BigInt&& bi);
	BigInt& operator=(BigInt&& bi);

	//Binary operators
	BigInt operator+(const BigInt& bi);
	BigInt operator-(const BigInt& bi);
	BigInt operator*(const BigInt& bi);
	BigInt operator/(const BigInt& bi);
	BigInt operator%(const BigInt& bi);
	BigInt& operator+=(const BigInt& bi);
	BigInt& operator-=(const BigInt& bi);
	BigInt& operator*=(const BigInt& bi);
	BigInt& operator/=(const BigInt& bi);
	BigInt& operator%=(const BigInt& bi);

	//Unary operators
	BigInt operator+() const;
	BigInt operator-() const;
	BigInt& operator++();
	BigInt& operator--();

	//Logic operators
	std::strong_ordering operator<=>(const BigInt& bi) const;
	bool operator==(const BigInt& bi) const;

	//Getters
	std::string toString() const;
	int length() const { return m_digit_count; }

private:
	std::vector<digit_type> m_digits;
	int m_digit_count;
	std::size_t m_capacity;
	sign m_sign;

	//naive arithmetic functions
	BigInt minus_helper_naive(const BigInt& left, const BigInt& right);
	BigInt plus_helper_naive(const BigInt& left, const BigInt& right);
	BigInt mult_helper_naive(const BigInt& left, const BigInt& right);
	std::pair<BigInt, BigInt> div_helper_naive(const BigInt& left, const BigInt& right);

	//input output opearations
	friend std::ostream& operator<<(std::ostream& os, const BigInt& bi);
	friend std::istream& operator>>(std::istream& input, BigInt& bi);


	//other utility functions
	void grow_container(std::size_t expected_size);
	void shrink_container();
	bool is_zero() const;
	bool is_one() const;
};

BigInt::BigInt() :
	m_digits{ std::vector<digit_type>(4) },
	m_digit_count{ 1 },
	m_capacity{ 4 },
	m_sign{ sign::positive }
{
	m_digits[0] = 0;
}

BigInt::BigInt(long long number)
{
	std::string s_number = std::to_string(number);
	BigInt temp{ s_number };
	this->m_digits = std::move(temp.m_digits);
	this->m_sign = temp.m_sign;
	this->m_capacity = temp.m_capacity;
	this->m_digit_count = temp.m_digit_count;
}

std::string divideByBase(std::string number, int base, int& remainder) {
	std::string quotient;
	remainder = 0;
	
	for (char digit : number) {
		int current = remainder * 10 + (digit - '0');
		quotient += (current / base) + '0';
		remainder = current % base;
	}

	// Remove leading zeros from the quotient
	size_t pos = quotient.find_first_not_of('0');
	if (pos != std::string::npos) {
		quotient = quotient.substr(pos);
	}
	else {
		quotient = "0";
	}

	return quotient;
}

// Function to convert a decimal string to a base 32bits string
std::vector<std::uint8_t> convertToBase32bits(const std::string& decimalStr) {
	if (decimalStr == "0") return std::vector<std::uint8_t>(4);

	std::vector<std::uint8_t> result;
	std::string number = decimalStr[0] == '-' ? decimalStr.substr(1) : decimalStr;
	int remainder;

	while (number != "0") {
		// Divide the number by 256 and get the remainder
		number = divideByBase(number, __UINT8_MAX, remainder);
		result.emplace_back(static_cast<std::uint8_t>(remainder));
	}
	int size = result.size();
	if(size % 4 != 0)
		size = (size / 4 + 1) * 4;
	result.resize(size);

	return result;
}

BigInt::BigInt(std::string number) :
	m_sign{ number[0] == '-' ? sign::negative : sign::positive }
{
	std::vector<std::uint8_t> result = convertToBase32bits(number);

	m_digits = std::vector<digit_type>(result.size() / 4 + 1);
	m_digit_count = 0;
	m_capacity = result.size() / 4 + 1;

	for (int i = 0; i != result.size(); i += 4)
	{
		int idx = i / 4;
		m_digits[idx] += result[i + 3];
		m_digits[idx] <<= 8;
		m_digits[idx] += result[i + 2];
		m_digits[idx] <<= 8;
		m_digits[idx] += result[i + 1];
		m_digits[idx] <<= 8;
		m_digits[idx] += result[i + 0];
		++m_digit_count;
	}
}

BigInt::BigInt(const BigInt& bi) :
	m_digits{ std::vector<digit_type>(bi.m_capacity) },
	m_sign{ bi.m_sign },
	m_capacity{ bi.m_capacity },
	m_digit_count{ bi.m_digit_count }
{
	std::copy(bi.m_digits.begin(), bi.m_digits.begin() + bi.m_digit_count, this->m_digits.begin());
}

BigInt& BigInt::operator=(const BigInt& bi)
{
	this->m_digits = std::vector<digit_type>(bi.m_capacity);
	this->m_sign = bi.m_sign;
	this->m_capacity = bi.m_capacity;
	this->m_digit_count = bi.m_digit_count;
	std::copy(bi.m_digits.begin(), bi.m_digits.begin() + bi.m_digit_count, this->m_digits.begin());

	return *this;
}

BigInt::BigInt(BigInt&& bi) :
	m_digits{ std::move(bi.m_digits) },
	m_sign{ bi.m_sign },
	m_capacity{ bi.m_capacity },
	m_digit_count{ bi.m_digit_count }
{
	BigInt init;
	bi = init;
}

BigInt& BigInt::operator=(BigInt&& bi)
{
	this->m_digits = move(bi.m_digits);
	this->m_capacity = bi.m_capacity;
	this->m_digit_count = bi.m_digit_count;
	this->m_sign = bi.m_sign;

	BigInt init;
	bi = init;

	return *this;
}

BigInt BigInt::operator+(const BigInt& bi)
{
	if (bi.m_sign * this->m_sign == 1)
	{
		return plus_helper_naive(*this, bi);
	}
	else if (bi.m_sign == sign::positive)
	{
		return minus_helper_naive(bi, *this);
	}

	return minus_helper_naive(*this, bi);
}

BigInt BigInt::operator-(const BigInt& bi)
{
	if (bi.m_sign * this->m_sign != 1)
	{
		return plus_helper_naive(*this, bi);
	}
	else if (bi.m_sign == sign::negative)
	{
		return minus_helper_naive(bi, *this);
	}

	return minus_helper_naive(*this, -bi);
}

BigInt BigInt::plus_helper_naive(const BigInt& left, const BigInt& right)
{
	int new_length = std::max(right.m_digit_count + 1, left.m_digit_count + 1);
	BigInt result = left;
	result.m_sign = this->m_sign;
	result.grow_container(new_length);

	int i;
	std::uint64_t digit_result, carry = 0;

	for (i = 0; i != right.m_digit_count; ++i)
	{
		result.m_digits[i] += right.m_digits[i] + carry;
		carry = result.m_digits[i] < right.m_digits[i];
	}
	if (carry)
	{
		result.m_digits[i] += carry;
		result.m_digit_count = new_length;
	}
	else
	{
		result.m_digit_count = new_length - 1;
	}
	return result;
}

BigInt BigInt::minus_helper_naive(const BigInt& left, const BigInt& right)
{
	bool cond = left > -right;
	BigInt result;
	cond ? result = left : result = right;
	const BigInt& other = cond ? right : left;
	if (cond) result.m_sign = sign::positive;
	else result.m_sign = sign::negative;

	for (auto i = 0; i != other.m_digit_count; ++i)
	{
		if (other.m_digits[i] > result.m_digits[i])
		{
			result.m_digits[i] += BASE - other.m_digits[i];
			int j = i + 1;
			for (j; result.m_digits[j] == 0; j++)
			{
				--result.m_digits[j];
			}
			--result.m_digits[j];
		}
		else
		{
			result.m_digits[i] -= other.m_digits[i];
		}
	}
	for (auto i = result.m_digit_count - 1; result.m_digits[i] == 0 && i > 0; --i)
		--result.m_digit_count;
	result.shrink_container();
	return result;
}

BigInt BigInt::operator*(const BigInt& bi)
{
	if (this->is_zero() || bi.is_zero()) return BigInt{ 0 };

	if (this->is_one() || bi.is_one())
	{
		BigInt result = this->is_one() ? bi : *this;
		if (result.m_sign * this->m_sign == 1)
			result.m_sign = sign::positive;
		else
			result.m_sign = sign::negative;
		return result;
	}

	return mult_helper_naive(*this, bi);
}

BigInt BigInt::mult_helper_naive(const BigInt& left, const BigInt& right)
{
	BigInt result;
	result.m_sign = sign(left.m_sign * right.m_sign);
	int expected_size = right.m_digit_count + left.m_digit_count + 1;
	result.grow_container(expected_size);
	if (expected_size % 4 != 0)
		result.m_capacity = (expected_size / 4 + 1) * 4;

	std::uint64_t i = 0, j = 0, digit_result = 0, carry = 0;

	for (i = 0; i != right.m_digit_count; ++i)
	{
		carry = 0;
		for (j = 0; j != left.m_digit_count; ++j)
		{
			digit_result = (std::uint64_t)left.m_digits[j] * right.m_digits[i] + carry;
			result.m_digits[i + j] += digit_result;
			carry = digit_result / BASE + (result.m_digits[i + j] < (std::uint32_t)digit_result);
		}
		result.m_digits[i + j] += carry;
	}
	if (carry != 0)
	{
		result.m_digits[i + j - 1] = carry;
		result.m_digit_count = i + j;
	}
	else
	{
		result.m_digit_count = i + j - 1;
	}
	return result;
}

BigInt BigInt::operator/(const BigInt& bi)
{
	if (*this < bi) return BigInt{ 0 };
	auto [reminder, quotient] = div_helper_naive(*this, bi);
	return quotient;
}


BigInt BigInt::operator%(const BigInt& bi)
{
	if (*this < bi) return *this;
	auto [reminder, quotient] = div_helper_naive(*this, bi);
	return reminder;
}

std::pair<BigInt, BigInt> BigInt::div_helper_naive(const BigInt& left, const BigInt& right)
{
	BigInt dividend{ left }, divisor{ right };
	if (divisor.is_zero()) throw std::invalid_argument("Divide by zero exception");
	BigInt quotient, remainder;
	quotient.grow_container(dividend.m_capacity);
	remainder.grow_container(divisor.m_capacity);

	if (dividend < divisor) {
		quotient = BigInt(0);
		remainder = dividend;
		return { remainder, quotient };
	}

	remainder = dividend;
	BigInt current;
	for (int i = dividend.m_digit_count - 1, j = 0; i >= 0; --i, ++j) {
		current.grow_container(j + 1);
		current.m_digits[j] = remainder.m_digits[i];
		current.m_digit_count = j + 1;

		int count = 0;
		while (current >= divisor) {
			current = current.minus_helper_naive(current, divisor);
			count++;
		}
		quotient.m_digits[i] = count;
	}
	remainder = dividend - (divisor * quotient);
	quotient.shrink_container();
	return { remainder, quotient };
}


BigInt& BigInt::operator+=(const BigInt& bi)
{
	*this = *this + bi;
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& bi)
{
	*this = *this - bi;
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& bi)
{
	*this = *this * bi;
	return *this;
}

BigInt& BigInt::operator/=(const BigInt& bi)
{
	*this = *this / bi;
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& bi)
{
	*this = *this % bi;
	return *this;
}

BigInt BigInt::operator+() const
{
	BigInt result = *this;
	return *this;
}

BigInt BigInt::operator-() const
{
	BigInt result = *this;
	if (result.m_sign == sign::negative) result.m_sign = sign::positive;
	else result.m_sign = sign::negative;
	return result;
}

BigInt& BigInt::operator++()
{
	BigInt one{ 1 };
	*this += one;
	return *this;
}

BigInt& BigInt::operator--()
{
	BigInt one{ 1 };
	*this -= one;
	return *this;
}

std::strong_ordering BigInt::operator<=>(const BigInt& bi) const
{
	if (this->m_sign != bi.m_sign)
	{
		if (this->m_sign == sign::positive) return std::strong_ordering::greater;
		else return std::strong_ordering::less;
	}
	if (this->m_digit_count < bi.m_digit_count) return std::strong_ordering::less;
	if (this->m_digit_count > bi.m_digit_count) return std::strong_ordering::greater;
	for (auto i = this->m_digit_count - 1; i != -1; --i)
	{
		if (this->m_digits[i] < bi.m_digits[i]) return std::strong_ordering::less;
		if (this->m_digits[i] > bi.m_digits[i]) return std::strong_ordering::greater;
	}
	return std::strong_ordering::equal;
}

bool BigInt::operator==(const BigInt& bi) const
{
	if (this->m_digit_count != bi.m_digit_count) return false;

	for (auto i = this->m_digit_count - 1; i != -1; --i)
	{
		if (this->m_digits[i] != bi.m_digits[i]) return false;
	}

	return true;
}

export std::ostream& operator<<(std::ostream& os, const BigInt& bi)
{
	BigInt temp = bi;
	os << bi.toString();
	return os;
}

export std::istream& operator>>(std::istream& input, BigInt& bi) {
	std::string ipt;
	input >> ipt;
	bi = BigInt{ ipt };
	return input;
}

void BigInt::grow_container(std::size_t expected_size)
{
	if (expected_size < this->m_capacity) return;
	if (expected_size % 4 != 0)
		expected_size = (expected_size / 4 + 1) * 4;
	std::vector<digit_type> new_digits(expected_size);
	std::copy(this->m_digits.begin(), this->m_digits.begin() + this->m_digit_count, new_digits.begin());
	this->m_digits = std::move(new_digits);
	this->m_capacity = expected_size;
}

void BigInt::shrink_container()
{
	if (this->m_digit_count > this->m_capacity / 2) return;
	int new_size = this->m_digit_count;
	if (new_size % 4 != 0)
		new_size = (new_size / 4 + 1) * 4;
	std::vector<digit_type> new_digits(new_size);
	std::copy(this->m_digits.begin(), this->m_digits.begin() + this->m_digit_count, new_digits.begin());
	this->m_digits = std::move(new_digits);
	this->m_capacity = this->m_digit_count + 1;
}

std::string multiplyBy256(const std::string& number) {
	std::string result;
	int carry = 0;

	for (int i = number.size() - 1; i >= 0; --i) {
		int current = (number[i] - '0') * __UINT8_MAX + carry;
		result.push_back((current % 10) + '0');
		carry = current / 10;
	}

	while (carry > 0) {
		result.push_back((carry % 10) + '0');
		carry /= 10;
	}

	std::reverse(result.begin(), result.end());
	return result;
}

// Helper function to add two large numbers represented as strings
std::string addStrings(const std::string& num1, const std::string& num2) {
	std::string result;
	int carry = 0;
	int i = num1.size() - 1, j = num2.size() - 1;

	while (i >= 0 || j >= 0 || carry > 0) {
		int sum = carry;
		if (i >= 0) sum += num1[i--] - '0';
		if (j >= 0) sum += num2[j--] - '0';

		result.push_back((sum % 10) + '0');
		carry = sum / 10;
	}

	std::reverse(result.begin(), result.end());
	return result;
}

// Function to convert a base 256 string to a base 10 string
std::string convertBase256ToBase10(const std::vector<std::uint8_t>& base256Str) {
	std::string result = "0";

	for (std::uint8_t c : base256Str) {
		// Multiply the current result by 256
		result = multiplyBy256(result);
		// Add the integer value of the current character
		result = addStrings(result, std::to_string((std::uint8_t)c));
	}

	return result;
}

std::string BigInt::toString() const
{
	std::vector<std::uint8_t> to_convert;
	for (int i = 0; i < this->m_digit_count; ++i)
	{
		uint32_t temp = this->m_digits[i];
		to_convert.emplace_back(temp % __UINT8_MAX);
		temp /= __UINT8_MAX;
		to_convert.emplace_back(temp % __UINT8_MAX);
		temp /= __UINT8_MAX;
		to_convert.emplace_back(temp % __UINT8_MAX);
		temp /= __UINT8_MAX;
		to_convert.emplace_back(temp % __UINT8_MAX);
	}
	std::reverse(to_convert.begin(), to_convert.end());
	std::string result;
	if (this->m_sign == sign::negative)
	{
		result = "-" + convertBase256ToBase10(to_convert);
	}
	else
	{
		result = convertBase256ToBase10(to_convert);
	}
	return result;
}

bool BigInt::is_zero() const
{
	if (this->m_digit_count == 1 && this->m_digits[0] == 0)
	{
		return true;
	}
	return false;
}

bool BigInt::is_one() const
{
	if (this->m_digit_count == 1 && this->m_digits[0] == 1)
	{
		return true;
	}
	return false;
}
