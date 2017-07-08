// This file is part of Untzy.
//
// Untzy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Untzy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Untzy.  If not, see <http://www.gnu.org/licenses/>.

#include "core/duration.h"

#include "gmock/gmock.h"

#include <QString>

using ::testing::StrEq;

#define CMP_STR(d, str) ASSERT_THAT(d.get_str().toStdString(), StrEq(str));

TEST(DurationTest, UnknownState)
{
    Duration d1;
    ASSERT_EQ(d1.is_unknown(), true);
    ASSERT_EQ(d1.get_hours(), 0);
    ASSERT_EQ(d1.get_minutes(), 0);
    ASSERT_EQ(d1.get_seconds(), 0);
    CMP_STR(d1, "?");
}

TEST(DurationTest, StartState)
{
    Duration d1(1, 5, 13);
    ASSERT_EQ(d1.is_unknown(), false);
    ASSERT_EQ(d1.get_hours(), 1);
    ASSERT_EQ(d1.get_minutes(), 5);
    ASSERT_EQ(d1.get_seconds(), 13);
    CMP_STR(d1, "1:5:13");

    Duration d2(7147);
    ASSERT_EQ(d2.is_unknown(), false);
    ASSERT_EQ(d2.get_hours(), 1);
    ASSERT_EQ(d2.get_minutes(), 59);
    ASSERT_EQ(d2.get_seconds(), 7);
    CMP_STR(d2, "1:59:07");

    Duration d3(5);
    ASSERT_EQ(d3.is_unknown(), false);
    ASSERT_EQ(d3.get_hours(), 0);
    ASSERT_EQ(d3.get_minutes(), 0);
    ASSERT_EQ(d3.get_seconds(), 5);
    CMP_STR(d3, "0:05");

    Duration d4(100, 0, 0);
    ASSERT_EQ(d4.is_unknown(), false);
    ASSERT_EQ(d4.get_hours(), 100);
    ASSERT_EQ(d4.get_minutes(), 0);
    ASSERT_EQ(d4.get_seconds(), 0);
    CMP_STR(d4, "100:0:00");

    Duration d5(0);
    ASSERT_EQ(d5.is_unknown(), false);
    ASSERT_EQ(d5.get_hours(), 0);
    ASSERT_EQ(d5.get_minutes(), 0);
    ASSERT_EQ(d5.get_seconds(), 0);
    CMP_STR(d5, "0:00");
}
