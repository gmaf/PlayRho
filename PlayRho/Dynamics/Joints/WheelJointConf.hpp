/*
 * Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
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

#ifndef PLAYRHO_DYNAMICS_JOINTS_WHEELJOINTCONF_HPP
#define PLAYRHO_DYNAMICS_JOINTS_WHEELJOINTCONF_HPP

#include <PlayRho/Dynamics/Joints/JointConf.hpp>

#include <PlayRho/Common/Math.hpp>

namespace playrho {

struct ConstraintSolverConf;
class StepConf;

namespace d2 {

class World;
class BodyConstraint;

/// @brief Wheel joint definition.
/// @details This requires defining a line of
///   motion using an axis and an anchor point. The definition uses local
///   anchor points and a local axis so that the initial configuration
///   can violate the constraint slightly. The joint translation is zero
///   when the local anchor points coincide in world space. Using local
///   anchors and a local axis helps when saving and loading a game.
struct WheelJointConf : public JointBuilder<WheelJointConf>
{
    /// @brief Super type.
    using super = JointBuilder<WheelJointConf>;
    
    constexpr WheelJointConf() noexcept = default;
    
    /// Initialize the bodies, anchors, axis, and reference angle using the world
    /// anchor and world axis.
    WheelJointConf(BodyID bA, BodyID bB,
                   Length2 laA = Length2{}, Length2 laB = Length2{},
                   UnitVec axis = UnitVec::GetRight()) noexcept;

    /// @brief Uses the given enable motor state value.
    constexpr auto& UseEnableMotor(bool v) noexcept
    {
        enableMotor = v;
        return *this;
    }

    /// @brief Uses the given max motor toque value.
    constexpr auto& UseMaxMotorTorque(Torque v) noexcept
    {
        maxMotorTorque = v;
        return *this;
    }

    /// @brief Uses the given motor speed value.
    constexpr auto& UseMotorSpeed(AngularVelocity v) noexcept
    {
        motorSpeed = v;
        return *this;
    }

    /// @brief Uses the given frequency value.
    constexpr auto& UseFrequency(Frequency v) noexcept
    {
        frequency = v;
        return *this;
    }

    /// @brief Uses the given damping ratio value.
    constexpr auto& UseDampingRatio(Real v) noexcept
    {
        dampingRatio = v;
        return *this;
    }

    /// The local anchor point relative to body A's origin.
    Length2 localAnchorA = Length2{};

    /// The local anchor point relative to body B's origin.
    Length2 localAnchorB = Length2{};

    /// The local translation axis in body-A.
    UnitVec localXAxisA = UnitVec::GetRight();

    UnitVec localYAxisA = GetRevPerpendicular(UnitVec::GetRight());

    /// Enable/disable the joint motor.
    bool enableMotor = false;

    /// The maximum motor torque.
    Torque maxMotorTorque = Torque{0};

    /// The desired angular motor speed.
    AngularVelocity motorSpeed = 0_rpm;

    /// Suspension frequency, zero indicates no suspension
    Frequency frequency = 2_Hz;

    /// Suspension damping ratio, one indicates critical damping
    Real dampingRatio = 0.7f;

    Momentum impulse = 0; ///< Impulse.
    AngularMomentum angularImpulse = 0; ///< Angular impulse.
    Momentum springImpulse = 0; ///< Spring impulse.

    UnitVec ax; ///< Solver A X directional.
    UnitVec ay; ///< Solver A Y directional.

    Length sAx = 0_m; ///< Solver A x location.
    Length sBx = 0_m; ///< Solver B x location.
    Length sAy = 0_m; ///< Solver A y location.
    Length sBy = 0_m; ///< Solver B y location.

    Mass mass = 0_kg; ///< Mass.
    RotInertia angularMass = RotInertia{0}; ///< Motor mass.
    Mass springMass = 0_kg; ///< Spring mass.

    LinearVelocity bias = 0_mps; ///< Bias.
    InvMass gamma = InvMass{0}; ///< Gamma.
};

/// @brief Gets the definition data for the given joint.
/// @relatedalso Joint
WheelJointConf GetWheelJointConf(const Joint& joint);

/// @relatedalso World
WheelJointConf GetWheelJointConf(const World& world, BodyID bodyA, BodyID bodyB,
                                 Length2 anchor, UnitVec axis = UnitVec::GetRight());

/// @relatedalso WheelJointConf
constexpr Momentum2 GetLinearReaction(const WheelJointConf& object)
{
    return object.impulse * object.ay + object.springImpulse * object.ax;
}

/// @relatedalso WheelJointConf
constexpr auto ShiftOrigin(WheelJointConf&, Length2)
{
    return false;
}

/// @brief Initializes velocity constraint data based on the given solver data.
/// @note This MUST be called prior to calling <code>SolveVelocity</code>.
/// @see SolveVelocity.
/// @relatedalso WheelJointConf
void InitVelocity(WheelJointConf& object, std::vector<BodyConstraint>& bodies,
                  const StepConf& step,
                  const ConstraintSolverConf& conf);

/// @brief Solves velocity constraint.
/// @pre <code>InitVelocity</code> has been called.
/// @see InitVelocity.
/// @return <code>true</code> if velocity is "solved", <code>false</code> otherwise.
/// @relatedalso WheelJointConf
bool SolveVelocity(WheelJointConf& object, std::vector<BodyConstraint>& bodies,
                   const StepConf& step);

/// @brief Solves the position constraint.
/// @return <code>true</code> if the position errors are within tolerance.
/// @relatedalso WheelJointConf
bool SolvePosition(const WheelJointConf& object, std::vector<BodyConstraint>& bodies,
                   const ConstraintSolverConf& conf);

} // namespace d2

template <>
struct TypeInfo<d2::WheelJointConf>
{
    static const char* name() noexcept {
        return "d2::WheelJointConf";
    }
};

} // namespace playrho

#endif // PLAYRHO_DYNAMICS_JOINTS_WHEELJOINTCONF_HPP
