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

#include <core/persistent_string_cache.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define DB_NAME "string_db"

int main(int /* argc */, char** /* argv */)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    system("rm -fr " DB_NAME "/*");
#pragma GCC diagnostic pop

    auto c = core::PersistentStringCache::open(DB_NAME, 1024 * 1024 * 1024, core::CacheDiscardPolicy::lru_only);

    string const bjarne = "Bjarne Stroustrup";

    // Retrieve a non-existent entry.
    auto value = c->get(bjarne);
    assert(!value);

    // Remove a non-existent entry. The return value indicates that it did not exist.
    assert(!c->invalidate(bjarne));

    // Add an entry to the cache.
    c->put(bjarne, "C++ inventor");

    // Retrieve the entry.
    value = c->get(bjarne);
    assert(value);
    assert(*value == "C++ inventor");

    // Test that the entry is there.
    assert(c->contains_key(bjarne));

    // Get metadata for the entry.
    assert(!c->get_metadata(bjarne));       // No metadata exists

    // Get value and metadata for the entry.
    auto data = c->get_data(bjarne);
    assert(data->value == "C++ inventor");
    assert(data->metadata == "");           // No metadata exists

    // Add metadata.
    c->put_metadata(bjarne, "Born 30 December 1950");

    // Check metadata.
    assert(*c->get_metadata(bjarne) == "Born 30 December 1950");

    // It's not possible to delete metadata. The only thing we can do is
    // set it to the empty string.
    c->put_metadata(bjarne, "");
    assert(*c->get_metadata(bjarne) == "");

    // Remove the entry. The return value indicates that it existed and was removed.
    assert(c->invalidate(bjarne));

    // Binary keys and values are fine.
    string bin_key = "a";
    bin_key += '\0';
    bin_key += "b";
    string const bin_value(10, '\0');

    c->put(bin_key, bin_value);

    data = c->get_data(bin_key);
    assert(data->value == bin_value);
}
