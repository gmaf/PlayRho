/*
 * Copyright (c) 2020 Louis Langholtz https://github.com/louis-langholtz/PlayRho
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "UnitTests.hpp"

#include <PlayRho/Dynamics/Joints/JointType.hpp>
#include <PlayRho/Dynamics/Joints/DistanceJointConf.hpp>
#include <PlayRho/Dynamics/Joints/FrictionJointConf.hpp>
#include <PlayRho/Dynamics/Joints/GearJointConf.hpp>
#include <PlayRho/Dynamics/Joints/MotorJointConf.hpp>
#include <PlayRho/Dynamics/Joints/PulleyJointConf.hpp>
#include <PlayRho/Dynamics/Joints/PrismaticJointConf.hpp>
#include <PlayRho/Dynamics/Joints/RevoluteJointConf.hpp>
#include <PlayRho/Dynamics/Joints/RopeJointConf.hpp>
#include <PlayRho/Dynamics/Joints/TargetJointConf.hpp>
#include <PlayRho/Dynamics/Joints/WeldJointConf.hpp>
#include <PlayRho/Dynamics/Joints/WheelJointConf.hpp>

using namespace playrho;
using namespace playrho::d2;

namespace {

const char* name1 = "name1";
const char* name2 = "name2";

} // namespace

TEST(JointType, ByteSize)
{
    // Check size at test runtime instead of compile-time via static_assert to avoid stopping
    // builds and to report actual size rather than just reporting that expected size is wrong.
    EXPECT_EQ(sizeof(JointType::underlying_type), sizeof(void*));
    EXPECT_EQ(sizeof(JointType), sizeof(void*));
}

TEST(JointType, EqualsOperator)
{
    EXPECT_TRUE(JointType(&name1) == JointType(&name1));
    EXPECT_TRUE(JointType(&name2) == JointType(&name2));
    EXPECT_FALSE(JointType(&name1) == JointType(&name2));
    EXPECT_FALSE(JointType(&name2) == JointType(&name1));
}

TEST(JointType, NotEqualsOperator)
{
    EXPECT_FALSE(JointType(&name1) != JointType(&name1));
    EXPECT_FALSE(JointType(&name2) != JointType(&name2));
    EXPECT_TRUE(JointType(&name1) != JointType(&name2));
    EXPECT_TRUE(JointType(&name2) != JointType(&name1));
}

TEST(JointType, GetName)
{
}
