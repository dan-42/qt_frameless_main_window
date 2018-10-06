/**
SPDX-License-Identifier:  MIT
Licensed under the MIT License <http://opensource.org/licenses/MIT>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Copyright 2017 github.com/dfct
Copyright 2018 github.com/dan-42
*/

#pragma once
#ifndef FRAMELESS_WIDGET_H
#define FRAMELESS_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QToolBar>

class QResizeEvent;

namespace frameless
{
namespace detail
{

class widget_impl : public QWidget
{
    Q_OBJECT
public:
  explicit widget_impl(QWidget *parent = nullptr);
	
  auto content(QWidget* c) -> void;

	QPushButton* maximizeButton = nullptr;
  QPushButton* restoreButton = nullptr;
	QPushButton* minimizeButton = nullptr;
  QPushButton* closeButton = nullptr;

public:
  QWidget* buttons_;
  QWidget* top_drag_area_;
private:
  ///
  ///
  auto resizeEvent(QResizeEvent *event) -> void override;
};

} //namespace detail
} //namespace frameless

#endif // FRAMELESS_WIDGET_H