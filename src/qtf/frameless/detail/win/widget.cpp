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


#include <qtf/frameless/detail/win/widget.hpp>

#include <qtf/frameless/detail/win/native_window.hpp>

#include <QApplication>
#include <QEvent>
#include <QFocusEvent>
#include <QWindow>
#include <QVBoxLayout>

#include <Windowsx.h>
#include <Windows.h>

namespace qtf
{
namespace frameless
{
namespace detail
{
namespace win
{

widget::widget(QWidget* _content)
  : QWidget{nullptr}
  , native_window_{nullptr}  
  , allowed_window_dragging_areas_{}
  , reenable_parent_{false}
  , border_width_{6}
  , border_top_height_{40}
{
  const auto device_pixel_ratio =  window()->devicePixelRatio();
  //Create a native window and give it geometry values * devicePixelRatio for HiDPI support  
  native_window_ = new native_window(QRect{
      1 * device_pixel_ratio
    , 1 * device_pixel_ratio
    , 1 * device_pixel_ratio
    , 1 * device_pixel_ratio
  });
  native_window_->device_pixel_ratio(device_pixel_ratio);

  //Create the child window & embed it into the native one
  if (native_window_->native_handle())
  {
    setWindowFlags(Qt::FramelessWindowHint);
    setProperty("_q_embedded_native_parent_handle", reinterpret_cast<WId>(native_window_->native_handle()));
    SetWindowLong(reinterpret_cast<HWND>(winId()), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    SetParent(reinterpret_cast<HWND>(winId()), native_window_->native_handle());
    QEvent e(QEvent::EmbeddingControl);
    QApplication::sendEvent(this, &e);

    connect(native_window_, &native_window::close_event, [this]() {   hide();  });
    connect(native_window_, &native_window::geometry_changed, [this](const QRect& g) { QWidget::setGeometry(g); });
    connect(native_window_, &native_window::on_restore, [this]() { emit on_restore(); });
    connect(native_window_, &native_window::on_maximize, [this]() { emit on_maximize(); });
  }

  //Clear margins & spacing & add the layout to prepare for the content widget
  auto _layout = new QHBoxLayout{};
  setContentsMargins(0, 0, 0, 0);
  _layout->setContentsMargins(0, 0, 0, 0);
  _layout->setSpacing(0);
  setLayout(_layout);

  content(_content);
}

///
///
widget::~widget() = default;

///
///
auto widget::show() -> void
{
  ShowWindow(native_window_->native_handle(), SW_SHOWNORMAL);
  native_window_->saveFocus();
  QWidget::show();
}

///
///
auto widget::hide() -> void
{
  ShowWindow(native_window_->native_handle(), SW_HIDE);
  native_window_->resetFocus();
  QWidget::hide();
}

///
///
auto widget::minimize() -> void
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

///
///
auto widget::maximize() -> void
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

///
///
auto widget::restore() -> void
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_RESTORE, 0);
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
auto widget::center_to_parent() -> void
{
  const QWidget *child = findChild<QWidget*>();
  if (child && !child->isWindow()) {
    qWarning("widget::center: Call this function only for widgets with top_level children");
  }
  RECT r;
  GetWindowRect(native_window_->native_handle(), &r);
  setGeometry(
      (r.right-r.left)/2+r.left
    , (r.bottom-r.top)/2+r.top
    , 0
    , 0
  );
}

///
///
auto widget::content(QWidget* c) -> void
{
  if(layout()->count() > 0)
  {
    layout()->removeItem(layout()->itemAt(0));
  }
  c->setParent(this, Qt::Widget);
  c->setVisible(true);
  layout()->addWidget(c);
  //Send the parent native window a WM_SIZE message to update the widget size
  native_window_->update();
  allowed_window_dragging_areas_.push_back(c);
}

///
///
auto widget::geometry(const QRect& g) -> void
{
  const auto device_pixel_ratio =  window()->devicePixelRatio();
  native_window_->geometry(QRect
  {
      g.x()      * device_pixel_ratio
    , g.y()      * device_pixel_ratio
    , g.width()  * device_pixel_ratio
    , g.height() * device_pixel_ratio
  });
}

///
///
auto widget::add_draggable_areas(const std::vector<QWidget*>& areas) -> void
{
  allowed_window_dragging_areas_.insert(std::begin(allowed_window_dragging_areas_), std::begin(areas), std::end(areas));
}

///
///
void widget::childEvent(QChildEvent *e)
{
  QObject *obj = e->child();
  if (obj->isWidgetType())
  {
    if (e->added())
    {
      if (obj->isWidgetType())
      {
        obj->installEventFilter(this);
      }
    }
    else if (e->removed() && reenable_parent_)
    {
      reenable_parent_ = false;
      EnableWindow(native_window_->native_handle(), true);
      obj->removeEventFilter(this);
    }
  }
  QWidget::childEvent(e);
}

///
///
bool widget::nativeEvent(const QByteArray &, void *message, long *result)
{
  auto msg = reinterpret_cast<MSG*>(message);

  if (msg->message == WM_SETFOCUS)
  {
    Qt::FocusReason reason;
    if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
    {
      reason = Qt::MouseFocusReason;
    }
    else if (::GetKeyState(VK_SHIFT) < 0)
    {
      reason = Qt::BacktabFocusReason;
    }
    else
    {
      reason = Qt::TabFocusReason;
    }
    QFocusEvent e(QEvent::FocusIn, reason);
    QApplication::sendEvent(this, &e);
  }

  //Double check WM_SIZE messages to see if the parent native window is maximized
  if (msg->message == WM_SIZE)
  {
    //Get the window state
    WINDOWPLACEMENT wp;
    GetWindowPlacement(native_window_->native_handle(), &wp);

    //If we're maximized,
    if (wp.showCmd == SW_MAXIMIZE)
    {
      emit on_maximize();
    }
    else
    {
      emit on_restore();
    }
  }

  //Pass NCHITTESTS on the window edges as determined by border_width_ & border_top_height_ through to the parent native window
  if (msg->message == WM_NCHITTEST)
  {
    //If the mouse is over top of the toolbar area BUT is actually positioned over a child widget of the toolbar,
    //Then we don't want to enable dragging. This allows for buttons in the toolbar, eg, a Maximize button, to keep the mouse interaction
    const auto widget_at_cursor = QApplication::widgetAt(QCursor::pos());
    for(auto w : allowed_window_dragging_areas_)
    {
      if(w && w == widget_at_cursor)
      {
        //The mouse is over a widget, wich area shall allow dragging so pass this message
        //through to the native window for HTCAPTION dragging
        *result = HTTRANSPARENT;
        return true;
      }
    }

    RECT window_rect;
    int x, y;

    GetWindowRect(msg->hwnd, &window_rect);
    x = GET_X_LPARAM(msg->lParam) - window_rect.left;
    y = GET_Y_LPARAM(msg->lParam) - window_rect.top;

    if (x < border_width_ && y < border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > window_rect.right - window_rect.left - border_width_ && y < border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > window_rect.right - window_rect.left - border_width_ && y > window_rect.bottom - window_rect.top - border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x < border_width_ && y > window_rect.bottom - window_rect.top - border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x < border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (y < border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > window_rect.right - window_rect.left - border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (y > window_rect.bottom - window_rect.top - border_width_)
    {
      *result = HTTRANSPARENT;
      return true;
    }

    return false;
  }

  return false;
}

///
///
bool widget::eventFilter(QObject *o, QEvent *e)
{
  QWidget *w = reinterpret_cast<QWidget*>(o);

  switch (e->type())
  {
    case QEvent::WindowDeactivate:
      if (w->isModal() && w->isHidden())
      {
        BringWindowToTop(native_window_->native_handle());
      }
      break;
    case QEvent::Hide:
      if (reenable_parent_)
      {
        EnableWindow(native_window_->native_handle(), true);
        reenable_parent_ = false;
      }
      native_window_->resetFocus();

      if (w->testAttribute(Qt::WA_DeleteOnClose) && w->isWindow())
      {
        deleteLater();
      }
      break;
    case QEvent::Show:
      if (w->isWindow())
      {
        native_window_->saveFocus();
        hide();
        if (w->isModal() && !reenable_parent_)
        {
          EnableWindow(native_window_->native_handle(), false);
          reenable_parent_ = true;
        }
      }
      break;
    case QEvent::Close:
    {
      ::SetActiveWindow(native_window_->native_handle());
      if (w->testAttribute(Qt::WA_DeleteOnClose))
      {
        deleteLater();
      }
      break;
    }
    default:
      break;
  }

  return QWidget::eventFilter(o, e);
}

///
///
void widget::focusInEvent(QFocusEvent *e)
{
  QWidget *candidate = this;

  switch (e->reason())
  {
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
      while (!(candidate->focusPolicy() & Qt::TabFocus))
      {
        candidate = candidate->nextInFocusChain();
        if (candidate == this)
        {
          candidate = nullptr;
          break;
        }
      }
      if (candidate)
      {
        candidate->setFocus(e->reason());
        if (e->reason() == Qt::BacktabFocusReason || e->reason() == Qt::TabFocusReason)
        {
          candidate->setAttribute(Qt::WA_KeyboardFocusChange);
          candidate->window()->setAttribute(Qt::WA_KeyboardFocusChange);
        }
        if (e->reason() == Qt::BacktabFocusReason)
        {
          QWidget::focusNextPrevChild(false);
        }
      }
      break;
    default:
      break;
  }
}

///
///
bool widget::focusNextPrevChild(bool next)
{
  QWidget *current_focus = focusWidget();
  if (!next)
  {
    if (!current_focus->isWindow())
    {
      QWidget *next_focus = current_focus->nextInFocusChain();
      QWidget *previous_focus = nullptr;
      QWidget *top_level = nullptr;
      while (next_focus != current_focus)
      {
        if (next_focus->focusPolicy() & Qt::TabFocus)
        {
          previous_focus = next_focus;
          top_level = nullptr;
        }
        next_focus = next_focus->nextInFocusChain();
      }

      if (!top_level)
      {
        return QWidget::focusNextPrevChild(false);
      }
    }
  }
  else
  {
    QWidget *next_focus = current_focus;
    while (1 && next_focus != nullptr)
    {
      next_focus = next_focus->nextInFocusChain();
      if (next_focus->focusPolicy() & Qt::TabFocus)
      {
        return QWidget::focusNextPrevChild(true);
      }
    }
  }
  ::SetFocus(native_window_->native_handle());
  return true;
}

} //namespace win
} //namespace detail
} //namespace frameless
} //namespace qtf