#include "pch.h"
#include "..\Base\Types\Binary.h"
#include "..\Base\Types\FunDefs.h"
#include <cstddef>
using namespace py;
//TEST(Bytes, count)
//{
//	// Arrange
//	//Binary b('\xb1\x01\xef');
//	//pyBytes b({ 0xDE, 0xAD, 0xC0, 0xDE, 0x00, 0xFF });
//	//Binary b(0xDE);
//	auto b=binary({'a','b'});
//
//	// Act
//	auto count = MEM_FUN(b, count).A(97));
//	auto count1 = MEM_FUN(b, count).A(99));
//
//	// Assert
//	ASSERT_EQ(count,1);
//	ASSERT_EQ(count1, 0);
//}
//TEST(Bytes, endswith)
//{
//	// Arrange
//	auto b = binary({ 'a','b' });
//
//	// Act
//	auto end1 = MEM_FUN(b, endswith).A(97));
//	auto end2 = MEM_FUN(b, endswith).A(98));
//
//	// Assert
//	ASSERT_EQ(end1, 0);
//	ASSERT_EQ(end2, 1);
//}
//
//TEST(Bytes, index)
//{
//	// Arrange
//	auto b = binary({ 'a','b' });
//
//	// Act
//	auto ind = MEM_FUN(b, index).A(1));
//
//	// Assert
//	ASSERT_EQ(ind, 98);
//}
TEST(Bytes, hex)
{
	// Arrange
	auto b = binary({ 'a' });
	// Act
	auto hexstr = MEM_FUN(b, hex));
	// Assert
	ASSERT_EQ(hexstr, '61');
}

