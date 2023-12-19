/***************************************************************************
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * Use, modification and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 ***************************************************************************/

#pragma once
#include "json_iarchive_impl.hpp"

namespace boost {
namespace archive {
class json_iarchive
{
public:
    json_iarchive(std::istream& os)
        : json_archive_()
    {
        Json json;
        os >> json;
        json_archive_.set_data(json);
    }

public:
    template<class T>
    json_iarchive& operator>>(const T& t)
    {
        json_archive_ >> t;
        return *this;
    }

    json_iarchive_impl json_archive_;
};
} // namespace archive
} // namespace boost

