/*
 * Original work Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/PlayRho
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

#include <PlayRho/Dynamics/Joints/Joint.hpp>

#include <PlayRho/Dynamics/Joints/DistanceJointConf.hpp>
#include <PlayRho/Dynamics/Joints/FrictionJointConf.hpp>
#include <PlayRho/Dynamics/Joints/GearJointConf.hpp>
#include <PlayRho/Dynamics/Joints/MotorJointConf.hpp>
#include <PlayRho/Dynamics/Joints/PulleyJointConf.hpp>
#include <PlayRho/Dynamics/Joints/PrismaticJointConf.hpp>
#include <PlayRho/Dynamics/Joints/RevoluteJointConf.hpp>
#include <PlayRho/Dynamics/Joints/TargetJointConf.hpp>
#include <PlayRho/Dynamics/Joints/WeldJointConf.hpp>
#include <PlayRho/Dynamics/Joints/WheelJointConf.hpp>

#include <PlayRho/Dynamics/Contacts/BodyConstraint.hpp>

#include <PlayRho/Defines.hpp>

#include <PlayRho/Common/OptionalValue.hpp>

#include <algorithm>

namespace playrho {
namespace d2 {

static_assert(std::is_nothrow_default_constructible<Joint>::value,
              "Joint must be nothrow default constructible!");
static_assert(std::is_copy_constructible<Joint>::value,
              "Joint must be copy constructible!");
static_assert(std::is_nothrow_move_constructible<Joint>::value,
              "Joint must be nothrow move constructible!");
static_assert(std::is_copy_assignable<Joint>::value,
              "Joint must be copy assignable!");
static_assert(std::is_nothrow_move_assignable<Joint>::value,
              "Joint must be nothrow move assignable!");
static_assert(std::is_nothrow_destructible<Joint>::value,
              "Joint must be nothrow destructible!");

// Free functions...

BodyConstraint& At(std::vector<BodyConstraint>& container, BodyID key)
{
    return container.at(UnderlyingValue(key));
}

Angle GetReferenceAngle(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetReferenceAngle(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return GetReferenceAngle(TypeCast<PrismaticJointConf>(object));
    }
    if (type == GetTypeID<WeldJointConf>()) {
        return GetReferenceAngle(TypeCast<WeldJointConf>(object));
    }
    throw std::invalid_argument("GetReferenceAngle not supported by joint type");
}

UnitVec GetLocalXAxisA(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<WheelJointConf>()) {
        return GetLocalXAxisA(TypeCast<WheelJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return GetLocalXAxisA(TypeCast<PrismaticJointConf>(object));
    }
    throw std::invalid_argument("GetLocalXAxisA not supported by joint type");
}

UnitVec GetLocalYAxisA(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<WheelJointConf>()) {
        return GetLocalYAxisA(TypeCast<WheelJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return GetLocalYAxisA(TypeCast<PrismaticJointConf>(object));
    }
    throw std::invalid_argument("GetLocalYAxisA not supported by joint type");
}

AngularVelocity GetMotorSpeed(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetMotorSpeed(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return GetMotorSpeed(TypeCast<PrismaticJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return GetMotorSpeed(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("GetMotorSpeed not supported by joint type");
}

void SetMotorSpeed(Joint& object, AngularVelocity value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        object = TypeCast<RevoluteJointConf>(object).UseMotorSpeed(value);
        return;
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        object = TypeCast<PrismaticJointConf>(object).UseMotorSpeed(value);
        return;
    }
    if (type == GetTypeID<WheelJointConf>()) {
        object = TypeCast<WheelJointConf>(object).UseMotorSpeed(value);
        return;
    }
    throw std::invalid_argument("SetMotorSpeed not supported by joint type!");
}

RotInertia GetAngularMass(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<FrictionJointConf>()) {
        return GetAngularMass(TypeCast<FrictionJointConf>(object));
    }
    if (type == GetTypeID<MotorJointConf>()) {
        return GetAngularMass(TypeCast<MotorJointConf>(object));
    }
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetAngularMass(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return GetAngularMass(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("GetAngularMass not supported by joint type");
}

Torque GetMaxMotorTorque(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetMaxMotorTorque(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return GetMaxMotorTorque(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("GetMaxMotorTorque not supported by joint type");
}

void SetMaxMotorTorque(Joint& object, Torque value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        object = TypeCast<RevoluteJointConf>(object).UseMaxMotorTorque(value);
        return;
    }
    if (type == GetTypeID<WheelJointConf>()) {
        object = TypeCast<WheelJointConf>(object).UseMaxMotorTorque(value);
        return;
    }
    throw std::invalid_argument("SetMaxMotorTorque not supported by joint type!");
}

Real GetRatio(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<GearJointConf>()) {
        return GetRatio(TypeCast<GearJointConf>(object));
    }
    if (type == GetTypeID<PulleyJointConf>()) {
        return GetRatio(TypeCast<PulleyJointConf>(object));
    }
    throw std::invalid_argument("GetRatio not supported by joint type!");
}

Frequency GetFrequency(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<DistanceJointConf>()) {
        return GetFrequency(TypeCast<DistanceJointConf>(object));
    }
    if (type == GetTypeID<TargetJointConf>()) {
        return GetFrequency(TypeCast<TargetJointConf>(object));
    }
    if (type == GetTypeID<WeldJointConf>()) {
        return GetFrequency(TypeCast<WeldJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return GetFrequency(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("GetFrequency not supported by joint type");
}

void SetFrequency(Joint& object, Frequency value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<DistanceJointConf>()) {
        object = TypeCast<DistanceJointConf>(object).UseFrequency(value);
        return;
    }
    if (type == GetTypeID<TargetJointConf>()) {
        object = TypeCast<TargetJointConf>(object).UseFrequency(value);
        return;
    }
    if (type == GetTypeID<WeldJointConf>()) {
        object = TypeCast<WeldJointConf>(object).UseFrequency(value);
        return;
    }
    if (type == GetTypeID<WheelJointConf>()) {
        object = TypeCast<WheelJointConf>(object).UseFrequency(value);
        return;
    }
    throw std::invalid_argument("SetFrequency not supported by joint type!");
}

AngularMomentum GetAngularMotorImpulse(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetAngularMotorImpulse(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return GetAngularReaction(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("GetAngularMotorImpulse not supported by joint type");
}

Length2 GetTarget(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<TargetJointConf>()) {
        return GetTarget(TypeCast<TargetJointConf>(object));
    }
    throw std::invalid_argument("GetTarget not supported by joint type");
}

void SetTarget(Joint& object, Length2 value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<TargetJointConf>()) {
        object = TypeCast<TargetJointConf>(object).UseTarget(value);
        return;
    }
    throw std::invalid_argument("SetTarget not supported by joint type");
}

Angle GetAngularLowerLimit(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetAngularLowerLimit(TypeCast<RevoluteJointConf>(object));
    }
    throw std::invalid_argument("GetAngularLowerLimit not supported by joint type!");
}

Angle GetAngularUpperLimit(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return GetAngularUpperLimit(TypeCast<RevoluteJointConf>(object));
    }
    throw std::invalid_argument("GetAngularUpperLimit not supported by joint type!");
}

void SetAngularLimits(Joint& object, Angle lower, Angle upper)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        object = TypeCast<RevoluteJointConf>(object).UseLowerAngle(lower).UseUpperAngle(upper);
        return;
    }
    throw std::invalid_argument("SetAngularLimits not supported by joint type!");
}

bool IsLimitEnabled(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return IsLimitEnabled(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return IsLimitEnabled(TypeCast<PrismaticJointConf>(object));
    }
    throw std::invalid_argument("IsLimitEnabled not supported by joint type!");
}

void EnableLimit(Joint& object, bool value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        object = TypeCast<RevoluteJointConf>(object).UseEnableLimit(value);
        return;
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        object = TypeCast<PrismaticJointConf>(object).UseEnableLimit(value);
        return;
    }
    throw std::invalid_argument("EnableLimit not supported by joint type!");
}

bool IsMotorEnabled(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        return IsMotorEnabled(TypeCast<RevoluteJointConf>(object));
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        return IsMotorEnabled(TypeCast<PrismaticJointConf>(object));
    }
    if (type == GetTypeID<WheelJointConf>()) {
        return IsMotorEnabled(TypeCast<WheelJointConf>(object));
    }
    throw std::invalid_argument("IsMotorEnabled not supported by joint type!");
}

void EnableMotor(Joint& object, bool value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<RevoluteJointConf>()) {
        object = TypeCast<RevoluteJointConf>(object).UseEnableMotor(value);
        return;
    }
    if (type == GetTypeID<PrismaticJointConf>()) {
        object = TypeCast<PrismaticJointConf>(object).UseEnableMotor(value);
        return;
    }
    if (type == GetTypeID<WheelJointConf>()) {
        object = TypeCast<WheelJointConf>(object).UseEnableMotor(value);
        return;
    }
    throw std::invalid_argument("EnableMotor not supported by joint type!");
}

Length2 GetLinearOffset(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<MotorJointConf>()) {
        return GetLinearOffset(TypeCast<MotorJointConf>(object));
    }
    throw std::invalid_argument("GetLinearOffset not supported by joint type!");
}

void SetLinearOffset(Joint& object, Length2 value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<MotorJointConf>()) {
        object = TypeCast<MotorJointConf>(object).UseLinearOffset(value);
        return;
    }
    throw std::invalid_argument("SetLinearOffset not supported by joint type!");
}

Angle GetAngularOffset(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<MotorJointConf>()) {
        return GetAngularOffset(TypeCast<MotorJointConf>(object));
    }
    throw std::invalid_argument("GetAngularOffset not supported by joint type!");
}

void SetAngularOffset(Joint& object, Angle value)
{
    const auto type = GetType(object);
    if (type == GetTypeID<MotorJointConf>()) {
        object = TypeCast<MotorJointConf>(object).UseAngularOffset(value);
        return;
    }
    throw std::invalid_argument("SetAngularOffset not supported by joint type!");
}

Length2 GetGroundAnchorA(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<PulleyJointConf>()) {
        return GetGroundAnchorA(TypeCast<PulleyJointConf>(object));
    }
    throw std::invalid_argument("GetGroundAnchorA not supported by joint type!");
}

Length2 GetGroundAnchorB(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<PulleyJointConf>()) {
        return GetGroundAnchorB(TypeCast<PulleyJointConf>(object));
    }
    throw std::invalid_argument("GetGroundAnchorB not supported by joint type!");
}

Momentum GetLinearMotorImpulse(const Joint& object)
{
    const auto type = GetType(object);
    if (type == GetTypeID<PrismaticJointConf>()) {
        return GetLinearMotorImpulse(TypeCast<PrismaticJointConf>(object));
    }
    throw std::invalid_argument("GetLinearMotorImpulse not supported by joint type!");
}

} // namespace d2
} // namespace playrho
