/*
 * units.cpp
 *
 *  Created on: Oct 27, 2009
 *      Author: dc
 */


#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>
#include <barrett/units.h>


namespace {
using namespace barrett;


// template parameters list the types the following tests should be run over
typedef ::testing::Types<units::Array<5>, units::JointAngles<5>,
	units::JointTorques<5> > UATypes;

template<typename T>
class ArrayTypedTest : public ::testing::Test {
public:
	ArrayTypedTest() :
		a() {}
protected:
	T a;
};

TYPED_TEST_CASE(ArrayTypedTest, UATypes);


TYPED_TEST(ArrayTypedTest, DefaultCtor) {
	for (size_t i = 0; i < this->a.size(); ++i) {
		EXPECT_EQ(0.0, this->a[i]);
	}
}

TYPED_TEST(ArrayTypedTest, IsZero) {
	this->a.assign(0.0);
	EXPECT_TRUE(this->a.isZero());

	this->a[0] = 1.0;
	EXPECT_FALSE(this->a.isZero());

	this->a.assign(-5.8);
	EXPECT_FALSE(this->a.isZero());
}

TYPED_TEST(ArrayTypedTest, InitialValueCtor) {
	TypeParam a(-487.9);

	for (size_t i = 0; i < a.size(); ++i) {
		EXPECT_EQ(-487.9, a[i]);
	}
}

TYPED_TEST(ArrayTypedTest, CopyFromTypeParam) {
	this->a << 5, 42.8, 37, -12, 1.4;
	TypeParam a_copy = this->a;  // uses copy constructor

	EXPECT_EQ(this->a, a_copy);
}

TYPED_TEST(ArrayTypedTest, AssignFromTypeParam) {
	this->a << 5, 42.8, 37, -12, 1.4;
	TypeParam a_copy;
	a_copy = this->a;  // uses assignment operator

	EXPECT_EQ(this->a, a_copy);
}

TYPED_TEST(ArrayTypedTest, CopyFromArray) {
	units::Array<5> array;
	array << 5, 42.8, 37, -12, 1.4;
	TypeParam array_copy = array;  // uses copy constructor

	EXPECT_EQ(array, array_copy);
}

TYPED_TEST(ArrayTypedTest, AssignFromArray) {
	units::Array<5> array;
	array << 5, 42.8, 37, -12, 1.4;
	TypeParam array_copy;
	array_copy = array;  // uses assignment operator

	EXPECT_EQ(array, array_copy);
}

TYPED_TEST(ArrayTypedTest, ExplicitAssignment) {
	this->a << 5, 42.8, 37, -12, 1.4;

	double expected[] = { 5, 42.8, 37, -12, 1.4 };
	for (size_t i = 0; i < this->a.size(); ++i) {
		EXPECT_EQ(expected[i], this->a[i]);
	}
}

TYPED_TEST(ArrayTypedTest, ExplicitAssignmentZeroFills) {
	this->a << 5, 42.8, 37;

	double expected[] = { 5, 42.8, 37, 0, 0 };
	for (size_t i = 0; i < this->a.size(); ++i) {
		EXPECT_EQ(expected[i], this->a[i]);
	}
}

TYPED_TEST(ArrayTypedTest, ExplicitAssignmentThrows) {
	EXPECT_THROW((this->a << 5, 42.8, 37, -12, 1.4, -2.6),
			std::out_of_range);
}

TYPED_TEST(ArrayTypedTest, AccessAndModifyMembersByIndex) {
	for (size_t i = 0; i < this->a.size(); ++i) {
		this->a.at(i) = i/10.0 - 1;  // test both at()
		EXPECT_EQ(i/10.0 - 1, this->a[i]);  // and []
	}
}

TYPED_TEST(ArrayTypedTest, AccessAndModifyMembersByIterator) {
	typename TypeParam::iterator i;
	for (i = this->a.begin(); i != this->a.end(); ++i) {
		*i = -38.1e7;
		EXPECT_EQ(-38.1e7, *i);
	}
}

// we just want this to compile
TYPED_TEST(ArrayTypedTest, BoostArrayOperators) {
	TypeParam a_copy = this->a;

	EXPECT_TRUE(this->a == a_copy);
	EXPECT_FALSE(this->a != a_copy);

	EXPECT_TRUE(this->a <= a_copy);
	EXPECT_FALSE(this->a > a_copy);

	EXPECT_TRUE(this->a >= a_copy);
	EXPECT_FALSE(this->a < a_copy);
}

TYPED_TEST(ArrayTypedTest, VectorArithmetic) {
	TypeParam a1, a2, expected, result;

	a1 << 1, 2, 3, 4, 5;
	a2 << 5, 4, 3, 2, 1;

	result = a1 + a2;
	expected.assign(6);
	EXPECT_EQ(expected, result);

	result = a1 - a2;
	expected << -4, -2, 0, 2, 4;
	EXPECT_EQ(expected, result);

	result = a1 * a2;
	expected << 5, 8, 9, 8, 5;
	EXPECT_EQ(expected, result);

	result = a1 / a2;
	expected << 0.2, 0.5, 1, 2, 5;
	EXPECT_EQ(expected, result);

	result = -a1;
	expected << -1, -2, -3, -4, -5;
	EXPECT_EQ(expected, result);

	result = ((a1/a2) + (a1*a2)) / (-a1);
	expected << -5.2, -4.25, -10.0/3.0, -2.5, -2;
	EXPECT_EQ(expected, result);
}

TYPED_TEST(ArrayTypedTest, VectorScalerArithmetic) {
	TypeParam a, expected, result;

	a << 1, 2, 3, 4, 5;

	result = a + 5;
	expected << 6, 7, 8, 9, 10;
	EXPECT_EQ(expected, result);

	result = 5 + a;
	expected << 6, 7, 8, 9, 10;
	EXPECT_EQ(expected, result);

	result = a - 5;
	expected << -4, -3, -2, -1, 0;
	EXPECT_EQ(expected, result);

	result = 5 - a;
	expected << 4, 3, 2, 1, 0;
	EXPECT_EQ(expected, result);

	result = a * 5;
	expected << 5, 10, 15, 20, 25;
	EXPECT_EQ(expected, result);

	result = 5 * a;
	expected << 5, 10, 15, 20, 25;
	EXPECT_EQ(expected, result);

	result = a / 5;
	expected << 0.2, 0.4, 0.6, 0.8, 1;
	EXPECT_EQ(expected, result);

	result = 5 / a;
	expected << 5, 2.5, 5.0/3.0, 1.25, 1;
	EXPECT_EQ(expected, result);

	// TODO(dc): need an approxEqual() method
//	result = (0.6*a + 8) / 0.6 - a;
//	expected.assign(8.0/0.6);
//	EXPECT_EQ(expected, result);
}

TYPED_TEST(ArrayTypedTest, OstreamOperator) {
	std::stringstream strs;
	strs << this->a;
	EXPECT_EQ("[0, 0, 0, 0, 0]", strs.str());
}


}