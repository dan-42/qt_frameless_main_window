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


class QResizeEvent;
class QPushButton;

namespace qtf
{
namespace frameless
{

class windows10 : public QWidget
{
  Q_OBJECT
public:
  ///
  ///
  ///
  explicit windows10();
	
  ///
  ///
  ///
  auto content(QWidget* c) -> void;

  ///
  ///
  ///
  auto restored() -> void;

  ///
  ///
  ///
  auto maximized() -> void;

  ///
  ///
  ///
  auto draggable_widgets() const -> std::vector<QWidget*>;

signals:
  ///
  ///
  ///
  void minimize();
  void maximize();
  void restore();
  void close();

private:
  ///
  ///
  auto resizeEvent(QResizeEvent *event) -> void override;

private:
  QPushButton* button_maximize_;
  QPushButton* button_minimzie_;
  QPushButton* button_restore_;
  QPushButton* button_close_;
  QWidget* buttons_;
  QWidget* top_drag_area_;
};

} //namespace frameless
} //namespace qtf

#endif // FRAMELESS_WIDGET_H