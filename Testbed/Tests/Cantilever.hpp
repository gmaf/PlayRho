/*
* Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
* Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/PlayRho
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PLAYRHO_CANTILEVER_HPP
#define  PLAYRHO_CANTILEVER_HPP

#include "../Framework/Test.hpp"

namespace playrho {

// It is difficult to make a cantilever made of links completely rigid with weld joints.
// You will have to use a high number of iterations to make them stiff.
// So why not go ahead and use soft weld joints? They behave like a revolute
// joint with a rotational spring.
class Cantilever : public Test
{
public:

    enum
    {
        e_count = 8
    };

    Cantilever()
    {
        const auto ground = m_world.CreateBody();

        // Creates bottom ground
        ground->CreateFixture(std::make_shared<EdgeShape>(GetGroundEdgeConf()));

        // Creates left-end-fixed 8-part plank (below the top one)
        {
            auto conf = PolygonShape::Conf{};
            conf.density = 20_kgpm2;
            const auto shape = std::make_shared<PolygonShape>(0.5_m, 0.125_m, conf);

            auto prevBody = ground;
            for (auto i = 0; i < e_count; ++i)
            {
                BodyDef bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(-14.5f + 1.0f * i, 5.0f) * 1_m;
                const auto body = m_world.CreateBody(bd);
                body->CreateFixture(shape);

                m_world.CreateJoint(WeldJointDef{
                    prevBody, body, Vec2(-15.0f + 1.0f * i, 5.0f) * 1_m
                });

                prevBody = body;
            }
        }

        // Creates left-end-fixed 3-part plank at top
        {
            auto conf = PolygonShape::Conf{};
            conf.density = 20_kgpm2;
            const auto shape = std::make_shared<PolygonShape>(1_m, 0.125_m, conf);

            auto prevBody = ground;
            for (auto i = 0; i < 3; ++i)
            {
                BodyDef bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(-14.0f + 2.0f * i, 15.0f) * 1_m;
                const auto body = m_world.CreateBody(bd);
                body->CreateFixture(shape);

                auto jd = WeldJointDef{prevBody, body, Vec2(-15.0f + 2.0f * i, 15.0f) * 1_m};
                jd.frequency = 5_Hz;
                jd.dampingRatio = 0.7f;
                m_world.CreateJoint(jd);

                prevBody = body;
            }
        }

        // Creates 8-part plank to the right of the fixed planks (but not farthest right)
        {
            auto conf = PolygonShape::Conf{};
            conf.density = 20_kgpm2;
            const auto shape = std::make_shared<PolygonShape>(0.5_m, 0.125_m, conf);

            auto prevBody = ground;
            for (auto i = 0; i < e_count; ++i)
            {
                BodyDef bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(-4.5f + 1.0f * i, 5.0f) * 1_m;
                const auto body = m_world.CreateBody(bd);
                body->CreateFixture(shape);

                if (i > 0)
                {
                    m_world.CreateJoint(WeldJointDef{
                        prevBody, body, Vec2(-5.0f + 1.0f * i, 5.0f) * 1_m
                    });
                }

                prevBody = body;
            }
        }

        // Creates 8-part farthest-right plank
        {
            auto conf = PolygonShape::Conf{};
            conf.density = 20_kgpm2;
            const auto shape = std::make_shared<PolygonShape>(0.5_m, 0.125_m, conf);

            auto prevBody = ground;
            for (auto i = 0; i < e_count; ++i)
            {
                BodyDef bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(5.5f + 1.0f * i, 10.0f) * 1_m;
                const auto body = m_world.CreateBody(bd);
                body->CreateFixture(shape);

                if (i > 0)
                {
                    auto jd = WeldJointDef{prevBody, body, Vec2(5.0f + 1.0f * i, 10.0f) * 1_m};
                    jd.frequency = 8_Hz;
                    jd.dampingRatio = 0.7f;
                    m_world.CreateJoint(jd);
                }

                prevBody = body;
            }
        }

        // Creates triangles
        auto polyshape = std::make_shared<PolygonShape>();
        polyshape->Set({Vec2(-0.5f, 0.0f) * 1_m, Vec2(0.5f, 0.0f) * 1_m, Vec2(0.0f, 1.5f) * 1_m});
        polyshape->SetDensity(1_kgpm2);
        for (auto i = 0; i < 2; ++i)
        {
            BodyDef bd;
            bd.type = BodyType::Dynamic;
            bd.location = Vec2(-8.0f + 8.0f * i, 12.0f) * 1_m;
            const auto body = m_world.CreateBody(bd);
            body->CreateFixture(polyshape);
        }

        // Creates circles
        const auto circleshape = std::make_shared<DiskShape>(0.5_m);
        circleshape->SetDensity(1_kgpm2);
        for (auto i = 0; i < 2; ++i)
        {
            BodyDef bd;
            bd.type = BodyType::Dynamic;
            bd.location = Vec2(-6.0f + 6.0f * i, 10.0f) * 1_m;
            const auto body = m_world.CreateBody(bd);
            body->CreateFixture(circleshape);
        }
    }

    Body* m_middle;
};

} // namespace playrho

#endif
