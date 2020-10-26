/*
 * Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2020 Louis Langholtz https://github.com/louis-langholtz/PlayRho
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

#include <PlayRho/Dynamics/WorldImplFixture.hpp>

#include <PlayRho/Dynamics/WorldImpl.hpp>
#include <PlayRho/Dynamics/Fixture.hpp> // for use of GetFixture
#include <PlayRho/Dynamics/Body.hpp> // for use of WorldImpl::GetBody
#include <PlayRho/Dynamics/Contacts/Contact.hpp> // for use of WorldImpl::GetBody

namespace playrho {
namespace d2 {

FixtureID CreateFixture(WorldImpl& world, const FixtureConf& def, bool resetMassData)
{
    return world.CreateFixture(def, resetMassData);
}

bool Destroy(WorldImpl& world, FixtureID id, bool resetMassData)
{
    return world.Destroy(id, resetMassData);
}

BodyID GetBody(const WorldImpl& world, FixtureID id)
{
    return GetBody(world.GetFixture(id));
}

Shape GetShape(const WorldImpl& world, FixtureID id)
{
    return GetShape(world.GetFixture(id));
}

bool IsSensor(const WorldImpl& world, FixtureID id)
{
    return IsSensor(world.GetFixture(id));
}

void SetSensor(WorldImpl& world, FixtureID id, bool value)
{
    auto fixture = world.GetFixture(id);
    SetSensor(fixture, value);
    world.SetFixture(id, fixture);
}

AreaDensity GetDensity(const WorldImpl& world, FixtureID id)
{
    return GetDensity(world.GetFixture(id));
}

const std::vector<ContactCounter>& GetProxies(const WorldImpl& world, FixtureID id)
{
    return world.GetProxies(id);
}

Filter GetFilterData(const WorldImpl& world, FixtureID id)
{
    return GetFilterData(world.GetFixture(id));
}

void FlagContactsForFiltering(WorldImpl& world, FixtureID id)
{
    const auto contacts = world.GetBody(GetBody(world.GetFixture(id))).GetContacts();
    std::for_each(cbegin(contacts), cend(contacts), [&world, id](const auto& ci) {
        auto& contact = world.GetContact(std::get<ContactID>(ci));
        if ((contact.GetFixtureA() == id) || (contact.GetFixtureB() == id)) {
            contact.FlagForFiltering();
        }
    });
}

void Refilter(WorldImpl& world, FixtureID id)
{
    FlagContactsForFiltering(world, id);
    world.AddProxies(world.GetProxies(id));
}

void SetFilterData(WorldImpl& world, FixtureID id, const Filter& value)
{
    auto fixture = world.GetFixture(id);
    SetFilterData(fixture, value);
    world.SetFixture(id, fixture);
}

ContactCounter GetProxy(const WorldImpl& world, FixtureID id, ChildCounter child)
{
    return GetProxies(world, id).at(child);
}

} // namespace d2
} // namespace playrho
