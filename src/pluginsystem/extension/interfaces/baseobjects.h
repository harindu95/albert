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
#include <QDesktopServices>
#include <QClipboard>
#include <QProcess>
#include <QUrl>
#include <QString>
#include <QIcon>
#include <QVariant>
#include <memory>
#include <vector>
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
#include "albertapp.h"


/** ****************************************************************************
* @brief The Action
* An activatable AbstractItem
*/
struct Action {
    virtual ~Action() {}

    /**
     * @brief Exectute the Action.
     * Reimplement this if you want your item to do some actions on activation
     */
    virtual void activate() = 0;
};



/** ****************************************************************************
 * @brief The AbstractItem
 * Displayable base class for all albert items.
 */
struct ActionItem : public Action {
    virtual ~ActionItem() {}

    /**
     * @brief Access to the data to display.
     *
     * Used to supply item data to views and delegates. Generally, models only
     * need to supply data for Qt::DisplayRole and any application-specific
     * user roles, but it is also good practice to provide data for
     * Qt::ToolTipRole, Qt::AccessibleTextRole, and
     * Qt::AccessibleDescriptionRole. See the Qt::ItemDataRole enum
     * documentation for information about the types associated with each role.
     * http://doc.qt.io/qt-5/qt.html#ItemDataRole-enum
     *
     * @param role The types of requested data
     * @return The requested data
     */
    virtual QString name() const = 0;
    virtual QString info() const = 0;
    virtual QIcon icon() const = 0;
};



/** ****************************************************************************
 * @brief The A(bstract)A(lbert)Item - A3Item for convenience
 * This the most prominent class here. This is the base class for all you want -
 * let your items be visible in the proposal list. Subclass this item to your
 * liking and add it to the query if you think it matches the query context.
 */
struct A2Item : public ActionItem {
    virtual ~A2Item() {}

    /**
     * @brief Component accessor
     * Return the i-th compontent. Currently albert supports only two layers in
     * the view. The toplevel items and their children, the actions.
     * @return The i-th component.
     */

    virtual bool hasChildren() const = 0;
    virtual vector<shared_ptr<A2Item>> children() = 0;
};



/** ***************************************************************************/
struct A2Leaf : public A2Item {
    virtual ~A2Leaf() {}

    /**
     * @brief Component accessor
     * Return the i-th compontent. Currently albert supports only two layers in
     * the view. The toplevel items and their children, the actions.
     * @return The i-th component.
     */

    bool hasChildren() const override final {
        return false;
    }

    vector<shared_ptr<A2Item>> children() override final {
        return vector<shared_ptr<A2Item>>();
    }
};



/** ***************************************************************************/
class UrlAction final : public Action
{
public:
    UrlAction(QUrl url) : url_(url) {}
    UrlAction(QString url) : url_(QUrl(url)) {}
    void activate() override {
        QDesktopServices::openUrl(url_);
        //QProcess::startDetached(QString("kstart --activate chromium %1").arg(QUrl(_url).toString()));
        //QProcess::startDetached(QString("chromium %1").arg(QUrl(b._url).toString()));
    }
private:
    QUrl url_;
};




/** ***************************************************************************/
class CommandAction final : public Action
{
public:
    CommandAction(QString cmd) : cmd_(cmd) {}
    void activate() override { QProcess::startDetached(cmd_); }
private:
    QString cmd_;
};


/** ***************************************************************************/
class CopyToClipboardAction final : public Action
{
public:
    CopyToClipboardAction(QString text) : text_(text) {}
    void activate() override {
        QApplication::clipboard()->setText(text_);
    }
private:
    QString text_;
};



/** ***************************************************************************/
class StandardItem final : public A2Leaf
{
public:
    StandardItem(){}
    QString name() const override { return name_; }
    QString info() const override { return info_; }
    QIcon icon() const override { return icon_; }
    void activate() override {
        action_->activate();
        qApp->hideWidget();
    }

    void setName(QString name){name_ = name;}
    void setInfo(QString info){info_ = info;}
    void setIcon(QIcon icon){icon_ = icon;}
    void setAction(unique_ptr<Action> action){ action_ = std::move(action);}

private:
    QString name_;
    QString info_;
    QIcon icon_;
    unique_ptr<Action> action_;
};

