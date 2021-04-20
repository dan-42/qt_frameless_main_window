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

#include <qtf/config.hpp>
#if (defined QTF_CONFIG_WINDOWS)

#include <qtf/detail/win/widget.hpp>
#include <qtf/decoration/windows10.hpp>

#include <QApplication>

#include <Windowsx.h>
#include <shellscalingapi.h>

#pragma comment(lib, "Shcore.lib")

namespace 
{
///
///
HWND getHWNDForWidget(const QWidget* widget)
{
  if(widget && widget->nativeParentWidget())
  {
    return (HWND)widget->nativeParentWidget()->winId();
  }
  if(widget)
  {
    return (HWND)widget->winId();
  }
  return {};
}

} //namespace

namespace qtf
{

///
///
widget::widget(QWidget* parent)
  : QWidget(parent)
  , decoration_{nullptr}
  , drag_areas_{}
{  
  drag_areas_.push_back(this);
}

///
///
widget::~widget()
{
  decoration_cleanup();
}

///
///
auto widget::setDecoration(decoration::windows10* decoration) -> void
{  
  decoration_cleanup();
  decoration_ = decoration;
  for(auto da : decoration_->drag_areas())
  {
    drag_areas_.push_back(da);
  }  

  decoration->setParent(this);
  connect(decoration, &decoration::windows10::minimize, [this]() { minimize(); });
  connect(decoration, &decoration::windows10::maximize, [this]() { maximize(); });
  connect(decoration, &decoration::windows10::restore, [this]() { restore(); });
  connect(decoration, &decoration::windows10::close, [this]() { close(); });

  decoration_raise();
}

///
///
auto widget::minimize() -> void
{
  SendMessage(native_window_handle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

///
///
auto widget::maximize() -> void
{
  SendMessage(native_window_handle(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  decoration_set_maximized();
}

///
///
auto widget::restore() -> void
{
  SendMessage(native_window_handle(), WM_SYSCOMMAND, SC_RESTORE, 0);
  decoration_set_restored();
}

///
///
auto widget::close() -> void
{
  hide();
  QWidget::close();
}

///
///
auto widget::decoration_set_restored() -> void
{
  if(decoration_)
  {
    decoration_->restored();
  }
}

///
///
auto widget::decoration_set_maximized() -> void
{
  if(decoration_)
  {
    decoration_->maximized();
  }
}

///
///
auto widget::decoration_raise() -> void
{
  if(decoration_)
  {
    decoration_->show();
    decoration_->raise();
  }
}

///
///
auto widget::decoration_cleanup() -> void
{
  if(decoration_)
  {
    decoration_->hide();
    decoration_->deleteLater();
  }
}

///
///
auto widget::native_window_handle() -> HWND
{
  return getHWNDForWidget(this);
}

///
///
auto widget::nativeEvent(const QByteArray &, void *native_event, long *result) -> bool
{
  constexpr auto stop_event_propagation = true;
  constexpr auto continue_event_propagation = false;

  const auto event = reinterpret_cast<MSG*>(native_event);
  const auto msg = event->message;
  const auto& lParam = event->lParam;
  const auto& wParam = event->wParam;

  switch(msg)
  {
  case WM_NCCALCSIZE:
  {
    if(wParam == TRUE)
    {
      // this kills the window frame and title bar
      // see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-nccalcsize#remarks
      *result = 0;
      return stop_event_propagation;
    }
    else
    {
      return continue_event_propagation;
    }
  }  
  case WM_NCHITTEST:
    {
      constexpr LONG border_width = 4;
      RECT winrect;
      const auto hwnd = getHWNDForWidget(this);
      GetWindowRect(hwnd, &winrect);
      const auto x = GET_X_LPARAM(lParam);
      const auto y = GET_Y_LPARAM(lParam);

      //bottom left corner
      if (x >= winrect.left && x < winrect.left + border_width &&
          y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        *result = HTBOTTOMLEFT;
        return stop_event_propagation;
      }
      //bottom right corner
      else if (x < winrect.right && x >= winrect.right - border_width &&
          y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        *result = HTBOTTOMRIGHT;
        return stop_event_propagation;
      }
      //top left corner
      else if (x >= winrect.left && x < winrect.left + border_width &&
          y >= winrect.top && y < winrect.top + border_width)
      {
        *result = HTTOPLEFT;
        return stop_event_propagation;
      }
      //top right corner
      else if (x < winrect.right && x >= winrect.right - border_width &&
          y >= winrect.top && y < winrect.top + border_width)
      {
        *result = HTTOPRIGHT;
        return stop_event_propagation;
      }
      //left border
      else if (x >= winrect.left && x < winrect.left + border_width)
      {
        *result = HTLEFT;
        return stop_event_propagation;
      }
      //right border
      else if (x < winrect.right && x >= winrect.right - border_width)
      {
        *result = HTRIGHT;
        return stop_event_propagation;
      }
      //bottom border
      else if (y < winrect.bottom && y >= winrect.bottom - border_width)
      {
        *result = HTBOTTOM;
        return stop_event_propagation;
      }
      //top border
      else if (y >= winrect.top && y < winrect.top + 2)
      {
        *result = HTTOP;
        return stop_event_propagation;
      }      
      // If it wasn't a border but we still got the message,
      // return HTCAPTION to allow click-dragging the window
      else
      {
        const auto widget_at_cursor = QApplication::widgetAt(QCursor::pos());
        for(auto drag_areas : drag_areas_)
        {
          if (widget_at_cursor == drag_areas)
          {
            *result = HTCAPTION;
            return stop_event_propagation;
          }
        }
      }
      return continue_event_propagation;
      break;
    }
  }
  return continue_event_propagation;
}


} // namespace qtf

#endif //(defined QTF_CONFIG_WINDOWS)