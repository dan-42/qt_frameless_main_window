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
#ifndef FRAMELESS_native_window_H
#define FRAMELESS_native_window_H

#include <Windows.h>

#include <QObject>
#include <QSize>
#include <QWidget>

namespace qtf
{
namespace frameless
{
namespace detail
{
namespace win
{

class native_window : public QObject
{
  Q_OBJECT
public:
  ///
  ///
  ///
  native_window(const QRect& rect);

  ///
  ///
  ///
  ~native_window() noexcept;

  ///
  ///
  ///
  auto minimum_size(const QSize& size) -> void;

  ///
  ///
  ///
  auto maximum_size(const QSize& size) -> void;

  ///
  ///
  ///
  auto geometry(const QRect& g) -> void;

  ///
  ///
  ///
  auto device_pixel_ratio(long r) -> void;

  ///
  ///
  ///
  auto native_handle() const -> HWND;

  ///
  ///
  ///
  auto update() -> void;

  ///
  ///
  ///
  auto saveFocus() -> void;

  ///
  ///
  ///
  auto resetFocus() -> void;

signals:
  ///
  ///
  ///
  void close_event();

  ///
  ///
  ///
  void geometry_changed(const QRect& rect);

  ///
  ///
  ///
  void on_restore();

  ///
  ///
  ///
  void on_maximize();

private:
  ///
  ///
  ///
  static LRESULT CALLBACK WndProc(HWND native_window_handle_, UINT message, WPARAM wParam, LPARAM lParam);

private:
  struct sizeType
  {
      inline sizeType()
        : required(false), width(0), height(0)
      {
      }
      bool required;
      int width;
      int height;
  };

  HWND native_window_handle_;
  HWND prev_focus_;
  DWORD aero_borderless_ = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN;
  bool flag_size_chaning_;
  long device_pixel_ratio_;
  sizeType minimum_size_;
  sizeType maximum_size_;
};
} //namespace win
} //namespace detail
} //namespace frameless
} //namespace qtf

#endif // FRAMELESS_native_window_H
