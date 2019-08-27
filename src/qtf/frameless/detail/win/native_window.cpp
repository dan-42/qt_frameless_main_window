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
#  pragma comment(lib, "user32.lib")
#endif

#include <qtf/frameless/detail/win/native_window.hpp>

#include <Windowsx.h>
#include <dwmapi.h>

#include <stdexcept>

namespace qtf
{
namespace frameless
{
namespace detail
{
namespace win
{
///
///
native_window::native_window(const QRect& rect)
  : native_window_handle_(nullptr)
  , flag_size_chaning_{false}
{

  HINSTANCE hInstance = GetModuleHandle(nullptr);
  WNDCLASSEX wcx{};

  wcx.cbSize = sizeof(WNDCLASSEX);
  wcx.style = CS_HREDRAW | CS_VREDRAW;
  wcx.hInstance = hInstance;
  wcx.lpfnWndProc = WndProc;
  wcx.cbClsExtra = 0;
  wcx.cbWndExtra = 0;
  wcx.lpszClassName = L"WindowClass"; 
  wcx.hCursor = LoadCursor(hInstance, IDC_APPSTARTING);

  RegisterClassEx(&wcx);
  if (FAILED(RegisterClassEx(&wcx)))
  {
    throw std::runtime_error("Couldn't register window class");
  }

  //Create a native window with the appropriate style
  native_window_handle_ = CreateWindow(L"WindowClass", L"WindowTitle", aero_borderless_, rect.x(), rect.y(), rect.width(), rect.height(), nullptr, nullptr, hInstance, nullptr);
  if (!native_window_handle_)
  {
    throw std::runtime_error("couldn't create window because of reasons");
  }

  SetWindowLongPtr(native_window_handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  //This code may be required for aero shadows on some versions of Windows
  //const MARGINS aero_shadow_on = { 1, 1, 1, 1 };
  //DwmExtendFrameIntoClientArea(native_window_handle_, &aero_shadow_on);
  SetWindowPos(native_window_handle_, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

///
///
native_window::~native_window() noexcept
{  
  ShowWindow(native_window_handle_, SW_HIDE);
  DestroyWindow(native_window_handle_);
}

///
///
auto native_window::minimum_size(const QSize& size) -> void
{
  minimum_size_.required = true;
  minimum_size_.width = size.width();
  minimum_size_.height = size.height();
}

///
///
auto native_window::maximum_size(const QSize& size) -> void
{
  maximum_size_.required = true;
  maximum_size_.width = size.width();
  maximum_size_.height = size.height();
}

///
///
auto native_window::geometry(const QRect& g) -> void
{
  if(!flag_size_chaning_)
  {
    flag_size_chaning_ = true;
    MoveWindow(native_window_handle_, g.x(), g.y(), g.width(), g.height(), 1);
    flag_size_chaning_ = false;
  }
}

///
///
auto native_window::device_pixel_ratio(long r) -> void
{
  device_pixel_ratio_ = r;
}

///
///
auto native_window::update() -> void
{
  RECT winrect;
  GetClientRect(native_window_handle_, &winrect);

  WINDOWPLACEMENT wp;
  wp.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(native_window_handle_, &wp);

  if (wp.showCmd == SW_MAXIMIZE)
  {
    const auto border = 8 * device_pixel_ratio_;
    emit geometry_changed(QRect{
        border
      , border
      , winrect.right -  (2 * border)
      , winrect.bottom - (2 * border)                            
    });
  }
  else
  {
    emit geometry_changed(QRect{0, 0, winrect.right,  winrect.bottom});
  }
}

///
///
auto native_window::native_handle() const -> HWND
{
  return native_window_handle_;
}

///
///
void native_window::saveFocus()
{
  if (!prev_focus_)
  {
    prev_focus_ = ::GetFocus();
  }
  if (!prev_focus_)
  {
    prev_focus_ = native_window_handle_;
  }
}

///
///
void native_window::resetFocus()
{
  if (prev_focus_)
  {
    ::SetFocus(prev_focus_);
  }
  else
  {
    ::SetFocus(native_window_handle_);
  }
}

///
///
LRESULT CALLBACK native_window::WndProc(HWND native_window_handle_, UINT message, WPARAM wParam, LPARAM lParam)
{
  native_window *window = reinterpret_cast<native_window*>(GetWindowLongPtr(native_window_handle_, GWLP_USERDATA));
  if (!window)
  {
    return DefWindowProc(native_window_handle_, message, wParam, lParam);
  }

  switch (message)
  {
    // ALT + SPACE or F10 system menu
    case WM_SYSCOMMAND:
    {
      if (wParam == SC_KEYMENU)
      {
        RECT winrect;
        GetWindowRect(native_window_handle_, &winrect);
        TrackPopupMenu(GetSystemMenu(native_window_handle_, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, native_window_handle_, nullptr);
        break;
      }
      else
      {
        return DefWindowProc(native_window_handle_, message, wParam, lParam);
      }
    }
    case WM_NCCALCSIZE:
    {
      //this kills the window frame and title bar we added with
      //WS_THICKFRAME and WS_CAPTION
      return 0;
    }
    // If the parent window gets any close messages,
    // send them over to QWinWidget and don't actually close here
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
      // This value can be arbitrarily large
      // as only intentionally-HTTRANSPARENT'd messages arrive here
      const LONG border_width = 8 * window->device_pixel_ratio_;
      RECT winrect;
      GetWindowRect(native_window_handle_, &winrect);
      const auto x = GET_X_LPARAM(lParam);
      const auto y = GET_Y_LPARAM(lParam);

      //bottom left corner
      if (x >= winrect.left && x < winrect.left + border_width &&
          y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        return HTBOTTOMLEFT;
      }
      //bottom right corner
      if (x < winrect.right && x >= winrect.right - border_width &&
          y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        return HTBOTTOMRIGHT;
      }
      //top left corner
      if (x >= winrect.left && x < winrect.left + border_width &&
          y >= winrect.top && y < winrect.top + border_width)
      {
        return HTTOPLEFT;
      }
      //top right corner
      if (x < winrect.right && x >= winrect.right - border_width &&
          y >= winrect.top && y < winrect.top + border_width)
      {
        return HTTOPRIGHT;
      }
      //left border
      if (x >= winrect.left && x < winrect.left + border_width)
      {
        return HTLEFT;
      }
      //right border
      if (x < winrect.right && x >= winrect.right - border_width)
      {
        return HTRIGHT;
      }
      //bottom border
      if (y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        return HTBOTTOM;
      }
      //top border
      if (y >= winrect.top && y < winrect.top + 2)
      {
        return HTTOP;
      }
      // If it wasn't a border but we still got the message,
      // return HTCAPTION to allow click-dragging the window
      return HTCAPTION;

      break;
    }

    // When this native window changes size,
    // it needs to manually resize the QWinWidget child
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
      auto minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
      if (window->minimum_size_.required) {
        minMaxInfo->ptMinTrackSize.x = window->minimum_size_.width;
        minMaxInfo->ptMinTrackSize.y = window->minimum_size_.height;
      }

      if (window->maximum_size_.required) {
        minMaxInfo->ptMaxTrackSize.x = window->maximum_size_.width;
        minMaxInfo->ptMaxTrackSize.y = window->maximum_size_.height;
      }
      return 0;
    }
  }
  return DefWindowProc(native_window_handle_, message, wParam, lParam);
}

} //namespace win
} //namespace detail
} //namespace frameless
} //namespace qtf