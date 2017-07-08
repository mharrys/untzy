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

#include "core/volume.h"

#include "gmock/gmock.h"

using ::testing::DoubleNear;

#define CMP_DBL(value, eq) ASSERT_THAT(value, DoubleNear(eq, 0.0000001));

TEST(VolumeTest, StartState)
{
    Volume v1;
    CMP_DBL(v1.get_level(), 1.0);
    CMP_DBL(v1.get_minimum(), 0.0);
    CMP_DBL(v1.get_maximum(), 1.0);

    Volume v2(-10.0, 100.0);
    CMP_DBL(v2.get_level(), 100.0);
    CMP_DBL(v2.get_minimum(), -10.0);
    CMP_DBL(v2.get_maximum(), 100.0);
}

TEST(VolumeTest, IncorrectMinMax)
{
    // min may not be greater than max
    ASSERT_THROW(Volume(1.0, 0.0), std::invalid_argument);
    // equal is ok
    ASSERT_NO_THROW({Volume(1.0, 1.0);});
}

TEST(VolumeTest, IncrementDecrement)
{
    Volume v1;
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.8);
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.6);
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.4);
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.2);
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.0);
    // locked to minimum
    v1.decrease();
    CMP_DBL(v1.get_level(), 0.0);
    v1.increase();
    CMP_DBL(v1.get_level(), 0.2);
    v1.increase();
    CMP_DBL(v1.get_level(), 0.4);
    v1.increase();
    CMP_DBL(v1.get_level(), 0.6);
    v1.increase();
    CMP_DBL(v1.get_level(), 0.8);
    v1.increase();
    CMP_DBL(v1.get_level(), 1.0);
    // locked to maximum
    v1.increase();
    CMP_DBL(v1.get_level(), 1.0);

    // different sizes
    Volume v2(-1.0, 2.0, 1.0);
    CMP_DBL(v2.get_level(), 2.0);
    v2.decrease();
    CMP_DBL(v2.get_level(), 1.0);
    v2.decrease();
    CMP_DBL(v2.get_level(), 0.0);
    v2.decrease();
    CMP_DBL(v2.get_level(), -1.0);
    // locked to minimum
    v2.decrease();
    CMP_DBL(v2.get_level(), -1.0);
    v2.increase();
    CMP_DBL(v2.get_level(), 0.0);
    v2.increase();
    CMP_DBL(v2.get_level(), 1.0);
    v2.increase();
    CMP_DBL(v2.get_level(), 2.0);
    // locked to maximum
    v2.increase();
    CMP_DBL(v2.get_level(), 2.0);
}

TEST(VolumeTest, SetLevel)
{
    Volume v1;
    CMP_DBL(v1.get_level(), 1.0);
    v1.set_level(0.5);
    CMP_DBL(v1.get_level(), 0.5);
    v1.set_level(100.0);
    CMP_DBL(v1.get_level(), 1.0);
    v1.set_level(-100.0);
    CMP_DBL(v1.get_level(), 0.0);
}
