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
#ifndef DECORATION_WIDGET_H
#define DECORATION_WIDGET_H

#include <QWidget>

class QPushButton;

namespace qtf
{
namespace decoration
{

class windows10 : public QWidget
{
  Q_OBJECT

// Structors
public:
  ///
  /// \see QWidget
  ///
  explicit windows10(QWidget* parent = nullptr);

  ///
  ///
  ///
  ~windows10() noexcept;

// Setters
public:
  ///
  ///
  ///
  auto restored() -> void;

  ///
  ///
  ///
  auto maximized() -> void;  
  
// Getters
public:
  ///
  ///
  auto drag_areas() const -> std::vector<QWidget*>;

// Signals
signals:  
  void minimize();
  void maximize();
  void restore();
  void close();

// Public overloads
public:
  ///
  ///
  auto setParent(QWidget* parent) -> void;

// Implementation  
private:
  auto set_parent_impl(QWidget* parent) -> void;
  auto update_position() -> void;

// Overrides
private:
  auto eventFilter(QObject *object, QEvent *event) -> bool override;

// Variables
private:
  QPushButton* button_maximize_;
  QPushButton* button_minimzie_;
  QPushButton* button_restore_;
  QPushButton* button_close_;  
  std::vector<QWidget*> drag_areas_;
};

} //namespace decoration
} //namespace qtf

#endif // DECORATION_WIDGET_H