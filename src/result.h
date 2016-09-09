/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2016 Evgeniy Reizner
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

#pragma once

// Stupid alternative to Rust's 'Result' type.

template <typename T, typename E>
class Result
{
public:
    Result(const T &at) : t(at) {}
    Result(const E &ae) : e(ae) {}
    Result(const typename E::Type &ae)
    { e.err = ae; }

    bool hasError() const
    { return e.err != E::Type::None; }

    const E& error() const
    { return e; }

    const T& value() const
    { Q_ASSERT(hasError() == false); return t; }

    const T& operator*() const
    { return value(); }

    operator bool () const
    { return !hasError(); }

private:
    T t;
    E e;
};
