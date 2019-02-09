/*
 * Copyright (C) 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Michi Henning <michi.henning@canonical.com>
 */

#include <core/persistent_cache.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Person
{
    string name;
    int age;
};

namespace core  // Specializations must be placed into namespace core.
{

template <>
string CacheCodec<Person>::encode(Person const& p)
{
    ostringstream s;
    s << p.age << ' ' << p.name;
    return s.str();
}

template <>
Person CacheCodec<Person>::decode(string const& str)
{
    istringstream s(str);
    Person p;
    s >> p.age >> p.name;
    return p;
}

}  // namespace core

#define DB_NAME "person_db"

int main(int /* argc */, char** /* argv */)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    system("rm -fr " DB_NAME "/*");
#pragma GCC diagnostic pop

    using PersonCache = core::PersistentCache<Person, string>;

    auto c = PersonCache::open(DB_NAME, 1024 * 1024 * 1024, core::CacheDiscardPolicy::lru_only);

    Person bjarne{"Bjarne Stroustrup", 65};
    c->put(bjarne, "C++ inventor");
    auto value = c->get(bjarne);
    if (value)
    {
        cout << bjarne.name << ": " << *value << endl;
    }
    Person person{"no such person", 0};
    value = c->get(person);
    assert(!value);
}
