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
#ifndef QTF_WIDGET_H
#define QTF_WIDGET_H

#include <atomic>

#include <QWidget>

namespace qtf { namespace frameless {
  class frameless_window;
  class windows10;
}}

namespace qtf
{

class widget_base : public QWidget
{
public:
  widget_base(QWidget* parent = nullptr);
  ~widget_base() override;

  auto setParent(QWidget* parent) -> void;
  auto setParent(QWidget* parent, Qt::WindowFlags f) -> void;

private:
  auto set_parent_impl(QWidget* parent) -> void;

  auto closeEvent(QCloseEvent *event) -> void override;
  auto event(QEvent *event) -> bool override;

private:
  frameless::frameless_window* window_;
  frameless::windows10* decoration_;
};

} // namespace qtf
#endif // WIDGET_HPP
