#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import BigInt;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

	template<>
	inline std::wstring ToString<BigInt>(const BigInt& bigInt) {
		std::string str = bigInt.toString();
		return std::wstring(str.begin(), str.end());
	}
}

namespace Unit_Tests
{
	TEST_CLASS(initializationAndPrint)
	{
	public:
		TEST_METHOD(init_toZero)
		{
			BigInt a_bi{ 0 };
			Assert::AreEqual(std::to_string(0), a_bi.toString());
		}
		TEST_METHOD(init_toNeg_fromString)
		{
			std::string test_value = "-293457029837456029384750239485720394857230948572304985";
			BigInt a_bi{ test_value };
			Assert::AreEqual(test_value, a_bi.toString());
		}
		TEST_METHOD(init_toPos_fromString)
		{
			std::string test_value = "293457029837456029384750239485720394857230948572304985";
			BigInt a_bi{ test_value };
			Assert::AreEqual(test_value, a_bi.toString());
		}
		TEST_METHOD(init_toPos_fromInt64)
		{
			long long a = 239847298347;
			BigInt a_bi{ a };
			Assert::AreEqual(std::to_string(a), a_bi.toString());
		}
		TEST_METHOD(init_toNeg_fromInt64)
		{
			long long a = -239847298347;
			BigInt a_bi{ a };
			Assert::AreEqual(std::to_string(a), a_bi.toString());
		}
	};
	TEST_CLASS(unary)
	{
	public:
		TEST_METHOD(incr_fromZero)
		{
			int a{ 0 };
			BigInt a_bi{ a };
			++a;
			BigInt c_bi{ a };
			++a_bi;
			Assert::AreEqual(c_bi, a_bi);
		}
		TEST_METHOD(incr_toZero)
		{
			int a{ -1 };
			BigInt a_bi{ a };
			++a;
			BigInt c_bi{ a };
			++a_bi;
			Assert::AreEqual(c_bi, a_bi);
		}
		TEST_METHOD(dec_fromZero)
		{
			int a{ 0 };
			BigInt a_bi{ a };
			--a;
			BigInt c_bi{ a };
			--a_bi;
			Assert::AreEqual(c_bi, a_bi);
		}
		TEST_METHOD(dec_toZero)
		{
			int a{ 1 };
			BigInt a_bi{ a };
			--a;
			BigInt c_bi{ a };
			--a_bi;
			Assert::AreEqual(c_bi, a_bi);
		}
	};

	TEST_CLASS(addition)
	{
	public:
		TEST_METHOD(add_PosPos)
		{
			int a{ 56789 }, b{ 96787 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_NegNeg)
		{
			int a{ -52435 }, b{ -32457 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_PosNeg)
		{
			int a{ 2345 }, b{ -7123 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_NegPos)
		{
			int a{ -3545 }, b{ 7132 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_ZeroPos)
		{
			int a{ 0 }, b{ 7132 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_NegZero)
		{
			int a{ -3545 }, b{ 0 }, c{ a + b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
		TEST_METHOD(add_veryLong)
		{
			BigInt a_bi{ "99999999999999999999999999999999" }, b_bi{ "11111111111111111111111111111111" }, c_bi{ "111111111111111111111111111111110"};
			Assert::AreEqual(c_bi, a_bi + b_bi);
		}
	};

	TEST_CLASS(subtraction)
	{
	public:
		TEST_METHOD(sub_PosPos)
		{
			int a{ 56789 }, b{ 96787 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_NegNeg)
		{
			int a{ -52435 }, b{ -32457 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_PosNeg)
		{
			int a{ 2345 }, b{ -7123 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_NegPos)
		{
			int a{ -3545 }, b{ 7132 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_ZeroPos)
		{
			int a{ 0 }, b{ 7132 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_NegZero)
		{
			int a{ -3545 }, b{ 0 }, c{ a - b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
		TEST_METHOD(sub_PosPos_bigNumbers)
		{
			BigInt a_bi{ "3545354535453545354535453545" }, b_bi{ "3545354535453545354123445" }, c_bi{ "3541809180918091809181330100" };
			Assert::AreEqual(c_bi, a_bi - b_bi);
		}
	};
	TEST_CLASS(multiplication)
	{
	public:
		TEST_METHOD(mult_withPosOne)
		{
			int a{ 56789 }, b{ 1 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_withNegOne)
		{
			int a{ 56789 }, b{ -1 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_withZero)
		{
			int a{ 2345 }, b{ 0 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_PosPos)
		{
			long long a{ 3545 }, b{ 7132 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_NegNeg)
		{
			int a{ -234 }, b{ -7132 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_PosNeg)
		{
			int a{ 3545 }, b{ -3456 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_NegPos)
		{
			int a{ -3545 }, b{ 2445 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
		TEST_METHOD(mult_PosPos_bigNumbers)
		{
			BigInt a_bi{ "3545354535453545354535453545" }, b_bi{ "3545354535453545354535453545" }, c_bi{ "12569538782061024384266609476715843401521870358853067025" };
			Assert::AreEqual(c_bi, a_bi * b_bi);
		}
	};
	TEST_CLASS(comparator)
	{
	public:
		TEST_METHOD(equal)
		{
			int a{ 1 }, b{ 1 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi == b_bi);
		}
		TEST_METHOD(not_equal)
		{
			int a{ 1 }, b{ 2 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi != b_bi);
		}
		TEST_METHOD(less)
		{
			int a{ 0 }, b{ 2345 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi < b_bi);
		}
		TEST_METHOD(less_equal)
		{
			long long a{ 3545 }, b{ 7132 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi <= b_bi);
		}
		TEST_METHOD(greater)
		{
			int a{ 2345 }, b{ 23 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi > b_bi);
		}
		TEST_METHOD(greater_equal)
		{
			int a{ 3545 }, b{ -3456 }, c{ a * b };
			BigInt a_bi{ a }, b_bi{ b }, c_bi{ c };
			Assert::AreEqual(true, a_bi >= b_bi);
		}
	};
}
