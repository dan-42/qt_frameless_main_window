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
Copyright 2021 github.com/dan-42
*/

#pragma once
#ifndef QTF_DETAIL_WIN_WIDGET_H
#define QTF_DETAIL_WIN_WIDGET_H

#include <qtf/config.hpp>
#if (defined QTF_CONFIG_WINDOWS)

#include <QWidget>

namespace qtf { namespace decoration {
  class windows10;
}}

namespace qtf
{

class widget : public QWidget
{
// Constructors
public:
  ///
  /// \see QWidget
  ///
  widget(QWidget* parent = nullptr);

  ///
  /// \see ~QWidget
  ///
  ~widget() override;
  
// Setter
public:
  ///
  /// Sets the Window decoration, e.g. a windows 10 style
  ///
  auto setDecoration(decoration::windows10* decoration) -> void;

// Implementation
private:
  auto minimize() -> void;
  auto maximize() -> void;
  auto restore() -> void;
  auto close() -> void;  
  auto decoration_set_restored() -> void;  
  auto decoration_set_maximized() -> void;
  auto decoration_raise() -> void;
  auto decoration_cleanup() -> void;
  auto native_window_handle() -> HWND;

// Overrides
private:
  auto nativeEvent(const QByteArray &eventType, void *message, long *result) -> bool override;    

// variables
private:
  decoration::windows10* decoration_;
  std::vector<QWidget*> drag_areas_;
};

} // namespace qtf

#endif //(defined QTF_CONFIG_WINDOWS)

#endif // QTF_DETAIL_WIN_WIDGET_H
