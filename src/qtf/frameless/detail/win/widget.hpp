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

Copyright 2013 Digia Plc and/or its subsidiary(-ies). BSD
Copyright 2017 github.com/dfct
Copyright 2018 github.com/dan-42
*/

#pragma once
#ifndef FRAMELESS_DETAIL_WIN_WIDGET_H
#define FRAMELESS_DETAIL_WIN_WIDGET_H

#include <QWidget>

namespace qtf
{
namespace frameless
{
namespace detail
{
namespace win
{

class native_window;

class widget : public QWidget
{
    Q_OBJECT
public:
  ///
  ///
  ///
  widget(QWidget* content);

  ///
  ///
  ///
  ~widget() override;

  ///
  ///
  ///
  auto show() -> void;

  ///
  ///
  ///
  auto hide() -> void;

  ///
  ///
  ///
  auto minimize() -> void;

  ///
  ///
  ///
  auto maximize() -> void;

  ///
  ///
  ///
  auto restore() -> void;

  ///
  ///
  ///
  auto close() -> void;

  ///
  ///
  ///
  auto center_to_parent() -> void;

  ///
  ///
  ///
  auto content(QWidget* content) -> void;

  ///
  ///
  ///
  auto geometry(const QRect& rect) -> void;

  ///
  ///
  ///
  auto add_draggable_areas(const std::vector<QWidget*>& areas) -> void;
signals:
  ///
  ///
  ///
  void on_restore();

  ///
  ///
  ///
  void on_maximize();

private:    
    void childEvent( QChildEvent *e ) override;
    bool eventFilter( QObject *o, QEvent *e ) override;
    bool focusNextPrevChild(bool next) override;
    void focusInEvent(QFocusEvent *e) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;    

private:  
  native_window* native_window_;  
  std::vector<QWidget*> allowed_window_dragging_areas_;
  bool reenable_parent_;
  int border_width_;
  int border_top_height_;
};

} //namespace win
} //namespace detail
} //namespace frameless
} //namespace qtf


#endif // FRAMELESS_DETAIL_WIN_WIDGET_H