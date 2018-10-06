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

#ifdef _MSC_VER
#  pragma comment(lib, "Gdi32.lib")
#endif

#include <frameless/detail/win/native_window.hpp>

#include <dwmapi.h>
#include <stdexcept>

namespace frameless
{
namespace detail
{
namespace win
{
native_window::native_window(const int x, const int y, const int width, const int height)
  : hWnd(nullptr)  
  , flag_size_chaning_{false}
{
  //The native window technically has a background color. You can set it here
  HBRUSH windowBackground = CreateSolidBrush(RGB(255, 255, 255));

  HINSTANCE hInstance = GetModuleHandle(nullptr);
  WNDCLASSEX wcx = { 0 };

  wcx.cbSize = sizeof(WNDCLASSEX);
  wcx.style = CS_HREDRAW | CS_VREDRAW;
  wcx.hInstance = hInstance;
  wcx.lpfnWndProc = WndProc;
  wcx.cbClsExtra = 0;
  wcx.cbWndExtra = 0;
  wcx.lpszClassName = L"WindowClass";
  wcx.hbrBackground = windowBackground;
  wcx.hCursor = LoadCursor(hInstance, IDC_ARROW);

  RegisterClassEx(&wcx);
  if (FAILED(RegisterClassEx(&wcx)))
  {
    throw std::runtime_error("Couldn't register window class");
  }

  //Create a native window with the appropriate style
  hWnd = CreateWindow(L"WindowClass", L"WindowTitle", aero_borderless, x, y, width, height, 0, 0, hInstance, nullptr);
  if (!hWnd)
  {
    throw std::runtime_error("couldn't create window because of reasons");
  }

  SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  //This code may be required for aero shadows on some versions of Windows
  //const MARGINS aero_shadow_on = { 1, 1, 1, 1 };
  //DwmExtendFrameIntoClientArea(hWnd, &aero_shadow_on);
  SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

///
///
native_window::~native_window()
{
  //Hide the window & send the destroy message
  ShowWindow(hWnd, SW_HIDE);
  DestroyWindow(hWnd);
}

///
//
LRESULT CALLBACK native_window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  native_window *window = reinterpret_cast<native_window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  if (!window)
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  switch (message)
  {
    // ALT + SPACE or F10 system menu
    case WM_SYSCOMMAND:
    {
      if (wParam == SC_KEYMENU)
      {
        RECT winrect;
        GetWindowRect(hWnd, &winrect);
        TrackPopupMenu(GetSystemMenu(hWnd, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, hWnd, NULL);
        break;
      }
      else
      {
        return DefWindowProc(hWnd, message, wParam, lParam);
      }
    }
    case WM_NCCALCSIZE:
    {
      //this kills the window frame and title bar we added with
      //WS_THICKFRAME and WS_CAPTION
      return 0;
    }

      //If the parent window gets any close messages, send them over to QWinWidget and don't actually close here
    case WM_CLOSE:
    {
      if (window)
      {
        emit window->close_event();
        return 0;
      }
      break;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }

    case WM_NCHITTEST:
    {

      const LONG borderWidth = 8 * window->device_pixel_ratio_; //This value can be arbitrarily large as only intentionally-HTTRANSPARENT'd messages arrive here
      RECT winrect;
      GetWindowRect(hWnd, &winrect);
      long x = GET_X_LPARAM(lParam);
      long y = GET_Y_LPARAM(lParam);

      //bottom left corner
      if (x >= winrect.left && x < winrect.left + borderWidth &&
          y < winrect.bottom && y >= winrect.bottom - borderWidth)
      {
        return HTBOTTOMLEFT;
      }
      //bottom right corner
      if (x < winrect.right && x >= winrect.right - borderWidth &&
          y < winrect.bottom && y >= winrect.bottom - borderWidth)
      {
        return HTBOTTOMRIGHT;
      }
      //top left corner
      if (x >= winrect.left && x < winrect.left + borderWidth &&
          y >= winrect.top && y < winrect.top + borderWidth)
      {
        return HTTOPLEFT;
      }
      //top right corner
      if (x < winrect.right && x >= winrect.right - borderWidth &&
          y >= winrect.top && y < winrect.top + borderWidth)
      {
        return HTTOPRIGHT;
      }
      //left border
      if (x >= winrect.left && x < winrect.left + borderWidth)
      {
        return HTLEFT;
      }
      //right border
      if (x < winrect.right && x >= winrect.right - borderWidth)
      {
        return HTRIGHT;
      }
      //bottom border
      if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
      {
        return HTBOTTOM;
      }
      //top border
      if (y >= winrect.top && y < winrect.top + 2)
      {
        return HTTOP;
      }

      //If it wasn't a border but we still got the message, return HTCAPTION to allow click-dragging the window
      return HTCAPTION;

      break;
    }

      //When this native window changes size, it needs to manually resize the QWinWidget child
    case WM_SIZE:
    {
      if(window)
      {
        window->update();
      }
      break;
    }
    case WM_GETMINMAXINFO:
    {
      MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
      if (window->minimumSize.required) {
        minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();;
        minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
      }

      if (window->maximumSize.required) {
        minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
        minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
      }
      return 0;
    }

  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

void native_window::setGeometry(const int x, const int y, const int width, const int height)
{
  if(!flag_size_chaning_)
  {
    flag_size_chaning_ = true;
    MoveWindow(hWnd, x, y, width, height, 1);
    flag_size_chaning_ = false;
  }
}

auto native_window::device_pixel_ratio(long r) -> void
{
  device_pixel_ratio_ = r;
}

auto native_window::update() -> void
{
  RECT winrect;
  GetClientRect(hWnd, &winrect);

  WINDOWPLACEMENT wp;
  wp.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(hWnd, &wp);

  if (wp.showCmd == SW_MAXIMIZE)
  {
    const auto border = 8 * device_pixel_ratio_;
    emit geometry_changed(
        border
      , border
      , winrect.right -  (2 * border)
      , winrect.bottom - (2 * border)
    );
  }
  else
  {
    emit geometry_changed(0, 0, winrect.right,  winrect.bottom);
  }
}

auto native_window::native_handle() const -> HWND
{
  return hWnd;
}

void native_window::setMinimumSize(const int width, const int height)
{
    this->minimumSize.required = true;
    this->minimumSize.width = width;
    this->minimumSize.height = height;
}

int native_window::getMinimumWidth()
{
    return minimumSize.width;
}

int native_window::getMinimumHeight()
{
    return minimumSize.height;
}


void native_window::setMaximumSize(const int width, const int height)
{
    this->maximumSize.required = true;
    this->maximumSize.width = width;
    this->maximumSize.height = height;
}

int native_window::getMaximumWidth()
{
    return maximumSize.width;
}

int native_window::getMaximumHeight()
{
    return maximumSize.height;
}

} //namespace win
} //namespace detail
} //namespace frameless