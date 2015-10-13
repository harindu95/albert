// albert - a simple application launcher for linux
// Copyright (C) 2014-2015 Manuel Schneider
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include "abstractobjects.h"
class Extension;

namespace Websearch {

class SearchEngine final : public A2Leaf
{
    friend class Extension;

public:
    SearchEngine() : enabled_(false) {}

    QString name() const override;
    QString info() const override;
    QIcon icon() const override;
    void activate() override;

private:
    bool    enabled_;
    QString name_;
    QString url_;
    QString trigger_;
    QString searchTerm_;
    QString iconPath_;
    QIcon   icon_;
};

}
