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

#include "Windows.h"
#include "Windowsx.h"

#include <QWidget>

namespace frameless
{
namespace detail
{
namespace win
{

class native_window
{
public:
  native_window(const int x, const int y, const int width, const int height);
  ~native_window();

  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


  //These six functions exist to restrict native window resizing to whatever you want your app minimum/maximum size to be
  void setMinimumSize(const int width, const int height);
  int getMinimumHeight();
  int getMinimumWidth();

  void setMaximumSize(const int width, const int height);
  int getMaximumHeight();
  int getMaximumWidth();
  void setGeometry(const int x, const int y, const int width, const int height);


  HWND hWnd;

  HWND childWindow;
  QWidget* childWidget;

private:
  struct sizeType
  {
      sizeType()
        : required(false), width(0), height(0)
      {
      }
      bool required;
      int width;
      int height;
  };

  sizeType minimumSize;
  sizeType maximumSize;

  DWORD aero_borderless = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN;
};
} //namespace win
} //namespace detail
} //namespace frameless

#endif // FRAMELESS_native_window_H
