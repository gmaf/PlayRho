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

#ifndef PLAYRHO_DYNAMICS_JOINTS_MOUSEJOINTCONF_HPP
#define PLAYRHO_DYNAMICS_JOINTS_MOUSEJOINTCONF_HPP

#include <PlayRho/Dynamics/Joints/JointConf.hpp>

#include <PlayRho/Common/Math.hpp>

namespace playrho {

struct ConstraintSolverConf;
class StepConf;

namespace d2 {

class BodyConstraint;

/// @brief Target joint definition.
/// @details A target joint is used to make a point on a body track a specified world point.
///   This a soft constraint with a maximum force. This allows the constraint to stretch and
///   without applying huge forces.
/// @ingroup JointsGroup
struct TargetJointConf : public JointBuilder<TargetJointConf>
{
    /// @brief Super type.
    using super = JointBuilder<TargetJointConf>;

    constexpr TargetJointConf() noexcept = default;

    /// @brief Initializing constructor.
    constexpr TargetJointConf(BodyID b) noexcept:
        super{super{}.UseBodyB(b)}
    {
        // Intentionally empty.
    }

    /// @brief Use value for target.
    constexpr auto& UseTarget(Length2 v) noexcept
    {
        target = v;
        return *this;
    }

    /// @brief Use value for the "anchor" (in coordinates local to "body B").
    /// @note Typically this would be the value of:
    ///   <code>bodyB
    ///     ? InverseTransform(target, bodyB->GetTransformation())
    ///     : GetInvalid<Length2>()</code>.
    constexpr auto& UseAnchor(Length2 v) noexcept
    {
        localAnchorB = v;
        return *this;
    }

    /// @brief Use value for max force.
    constexpr auto& UseMaxForce(NonNegative<Force> v) noexcept
    {
        maxForce = v;
        return *this;
    }

    /// @brief Use value for frequency.
    constexpr auto& UseFrequency(NonNegative<Frequency> v) noexcept
    {
        frequency = v;
        return *this;
    }

    /// @brief Use value for damping ratio.
    constexpr auto& UseDampingRatio(NonNegative<Real> v) noexcept
    {
        dampingRatio = v;
        return *this;
    }

    /// The initial world target point. This is assumed
    /// to coincide with the body anchor initially.
    Length2 target = Length2{};

    /// Anchor point.
    Length2 localAnchorB = Length2{};

    /// Max force.
    /// @details
    /// The maximum constraint force that can be exerted
    /// to move the candidate body. Usually you will express
    /// as some multiple of the weight (multiplier * mass * gravity).
    /// @note This may not be negative.
    NonNegative<Force> maxForce = NonNegative<Force>{0_N};
    
    /// Frequency.
    /// @details The has to do with the response speed.
    /// @note This value may not be negative.
    NonNegative<Frequency> frequency = NonNegative<Frequency>(5_Hz);
    
    /// The damping ratio. 0 = no damping, 1 = critical damping.
    NonNegative<Real> dampingRatio = NonNegative<Real>(0.7f);

    InvMass gamma = InvMass{0}; ///< Gamma.

    Momentum2 impulse = Momentum2{}; ///< Impulse.

    // Solver variables. These are only valid after InitVelocityConstraints called.
    Length2 rB = {}; ///< Relative B.
    Mass22 mass = {}; ///< 2-by-2 mass matrix in kilograms.
    LinearVelocity2 C = {}; ///< Velocity constant.
};

/// @brief Gets the definition data for the given joint.
/// @relatedalso Joint
TargetJointConf GetTargetJointConf(const Joint& joint);

/// @relatedalso TargetJointConf
constexpr auto GetLocalAnchorA(const TargetJointConf& conf) noexcept
{
    return Length2{};
}

/// @relatedalso TargetJointConf
constexpr Momentum2 GetLinearReaction(const TargetJointConf& object)
{
    return object.impulse;
}

/// @relatedalso TargetJointConf
constexpr AngularMomentum GetAngularReaction(const TargetJointConf&)
{
    return AngularMomentum{0};
}

/// @relatedalso TargetJointConf
constexpr bool ShiftOrigin(TargetJointConf& object, Length2 newOrigin)
{
    object.target -= newOrigin;
    return true;
}

/// @relatedalso TargetJointConf
constexpr auto GetTarget(const TargetJointConf& object) noexcept
{
    return object.target;
}

/// @relatedalso TargetJointConf
Mass22 GetEffectiveMassMatrix(const TargetJointConf& object, const BodyConstraint& body) noexcept;

/// @brief Initializes velocity constraint data based on the given solver data.
/// @note This MUST be called prior to calling <code>SolveVelocity</code>.
/// @see SolveVelocity.
/// @relatedalso TargetJointConf
void InitVelocity(TargetJointConf& object, std::vector<BodyConstraint>& bodies,
                  const StepConf& step,
                  const ConstraintSolverConf& conf);

/// @brief Solves velocity constraint.
/// @pre <code>InitVelocity</code> has been called.
/// @see InitVelocity.
/// @return <code>true</code> if velocity is "solved", <code>false</code> otherwise.
/// @relatedalso TargetJointConf
bool SolveVelocity(TargetJointConf& object, std::vector<BodyConstraint>& bodies,
                   const StepConf& step);

/// @brief Solves the position constraint.
/// @return <code>true</code> if the position errors are within tolerance.
/// @relatedalso TargetJointConf
bool SolvePosition(const TargetJointConf& object, std::vector<BodyConstraint>& bodies,
                   const ConstraintSolverConf& conf);

} // namespace d2

template <>
struct TypeInfo<d2::TargetJointConf>
{
    static const char* name() noexcept {
        return "d2::TargetJointConf";
    }
};

} // namespace playrho

#endif // PLAYRHO_DYNAMICS_JOINTS_MOUSEJOINTCONF_HPP
