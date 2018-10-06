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

#ifdef _MSC_VER
#pragma comment(lib, "user32.lib")
#endif

#include <frameless/detail/win/widget.hpp>

#include <QApplication>
#include <QEvent>
#include <QFocusEvent>
#include <qt_windows.h>
#include <QWindow>

#include <Windows.h>

namespace frameless
{
namespace detail
{
namespace win
{

widget::widget()
  : QWidget(nullptr),
    m_Layout(),
    p_Widget(nullptr),
    _prevFocus(nullptr),
    _reenableParent(false)
{

  //Create a native window and give it geometry values * devicePixelRatio for HiDPI support
  native_window_ = new native_window(1  * window()->devicePixelRatio()
                                     , 1 * window()->devicePixelRatio()
                                     , 1 * window()->devicePixelRatio()
                                     , 1 * window()->devicePixelRatio());
  native_window_->device_pixel_ratio( window()->devicePixelRatio());

  //If you want to set a minimize size for your app, do so here
  //native_window_->setMinimumSize(1024 * window()->devicePixelRatio(), 768 * window()->devicePixelRatio());

  //If you want to set a maximum size for your app, do so here
  //native_window_->setMaximumSize(1024 * window()->devicePixelRatio(), 768 * window()->devicePixelRatio());


  //Save the native window handle for shorthand use

  Q_ASSERT(native_window_->native_handle());


  //Create the child window & embed it into the native one
  if (native_window_->native_handle())
  {
    setWindowFlags(Qt::FramelessWindowHint);
    setProperty("_q_embedded_native_parent_handle", (WId)native_window_->native_handle());
    SetWindowLong((HWND)winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    SetParent((HWND)winId(), native_window_->native_handle());
    QEvent e(QEvent::EmbeddingControl);
    QApplication::sendEvent(this, &e);

    connect(native_window_, &native_window::close_event, [this]() {
      ShowWindow(native_window_->native_handle(), false);
      //And then quit
      QApplication::quit();
    });
    connect(native_window_, &native_window::geometry_changed, [this](int x, int y, int w, int h)
    {
      QWidget::setGeometry(x, y, w, h);
    });
  }

  //Clear margins & spacing & add the layout to prepare for the MainAppWidget
  setContentsMargins(0, 0, 0, 0);
  setLayout(&m_Layout);
  m_Layout.setContentsMargins(0, 0, 0, 0);
  m_Layout.setSpacing(0);

  //Create the true app widget
  p_Widget = new widget_impl(this);
  m_Layout.addWidget(p_Widget);
  p_Widget->setParent(this, Qt::Widget);
  p_Widget->setVisible(true);

  //Update the BORDERWIDTH value if needed for HiDPI displays
  BORDERWIDTH = BORDERWIDTH * window()->devicePixelRatio();

  //Update the TOOLBARHEIGHT value to match the height of toolBar * if needed, the HiDPI display
  if (p_Widget->buttons_)
  {
    TOOLBARHEIGHT = p_Widget->buttons_->height() * window()->devicePixelRatio();
  }

  connect(p_Widget->minimizeButton, &QPushButton::clicked, this, &widget::onMinimizeButtonClicked);
  connect(p_Widget->maximizeButton, &QPushButton::clicked, this, &widget::onMaximizeButtonClicked);
  connect(p_Widget->restoreButton, &QPushButton::clicked, this, &widget::onRestoreButtonClicked);
  connect(p_Widget->closeButton, &QPushButton::clicked, this, &widget::onCloseButtonClicked);

  p_Widget->restoreButton->hide();
  p_Widget->maximizeButton->show();

  //Send the parent native window a WM_SIZE message to update the widget size
  native_window_->update();
}


/*!
    Destroys this object, freeing all allocated resources.
*/
widget::~widget()
{

}

/*!
    Returns the handle of the native Win32 parent window.
*/
HWND widget::getParentWindow() const
{
  return native_window_->native_handle();
}

/*!
    \reimp
*/
void widget::childEvent(QChildEvent *e)
{
  QObject *obj = e->child();
  if (obj->isWidgetType()) {
    if (e->added()) {
      if (obj->isWidgetType()) {
        obj->installEventFilter(this);
      }
    } else if (e->removed() && _reenableParent) {
      _reenableParent = false;
      EnableWindow(native_window_->native_handle(), true);
      obj->removeEventFilter(this);
    }
  }
  QWidget::childEvent(e);
}

/*! \internal */
void widget::saveFocus()
{
  if (!_prevFocus)
    _prevFocus = ::GetFocus();
  if (!_prevFocus)
    _prevFocus = getParentWindow();
}

/*!
    Shows this widget. Overrides QWidget::show().

    \sa showCentered()
*/
void widget::show()
{
  ShowWindow(native_window_->native_handle(), true);
  saveFocus();
  QWidget::show();
}

/*!
    Centers this widget over the native parent window. Use this
    function to have Qt toplevel windows (i.e. dialogs) positioned
    correctly over their native parent windows.

    \code
    widget qwin(hParent);
    qwin.center();

    QMessageBox::information(&qwin, "Caption", "Information Text");
    \endcode

    This will center the message box over the client area of hParent.
*/
void widget::center()
{
  const QWidget *child = findChild<QWidget*>();
  if (child && !child->isWindow()) {
    qWarning("widget::center: Call this function only for widgets with toplevel children");
  }
  RECT r;
  GetWindowRect(native_window_->native_handle(), &r);
  setGeometry((r.right-r.left)/2+r.left, (r.bottom-r.top)/2+r.top,0,0);
}

void widget::setGeometry(int x, int y, int w, int h)
{
  native_window_->setGeometry(x * window()->devicePixelRatio()
                              , y * window()->devicePixelRatio()
                              , w * window()->devicePixelRatio()
                              , h * window()->devicePixelRatio());
}

///
///
auto widget::content(QWidget* c) -> void
{
  p_Widget->content(c);
}

/*!
    Sets the focus to the window that had the focus before this widget
    was shown, or if there was no previous window, sets the focus to
    the parent window.
*/
void widget::resetFocus()
{
  if (_prevFocus)
    ::SetFocus(_prevFocus);
  else
    ::SetFocus(getParentWindow());
}

void widget::onMinimizeButtonClicked()
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void widget::onMaximizeButtonClicked()
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  p_Widget->restoreButton->show();
  p_Widget->maximizeButton->hide();
}

void widget::onRestoreButtonClicked()
{
  SendMessage(native_window_->native_handle(), WM_SYSCOMMAND, SC_RESTORE, 0);
  p_Widget->restoreButton->hide();
  p_Widget->maximizeButton->show();
}

void widget::onCloseButtonClicked()
{
  if(true /* put your check for it if it safe to close your app here */) //eg, does the user need to save a document
  {
    //Safe to close, so hide the parent window
    ShowWindow(native_window_->native_handle(), false);

    //And then quit
    QApplication::quit();
  }
  else
  {
    //Do nothing, and thus, don't actually close the window
  }
}

bool widget::nativeEvent(const QByteArray &, void *message, long *result)
{
  MSG *msg = (MSG *)message;

  if (msg->message == WM_SETFOCUS)
  {
    Qt::FocusReason reason;
    if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
      reason = Qt::MouseFocusReason;
    else if (::GetKeyState(VK_SHIFT) < 0)
      reason = Qt::BacktabFocusReason;
    else
      reason = Qt::TabFocusReason;
    QFocusEvent e(QEvent::FocusIn, reason);
    QApplication::sendEvent(this, &e);
  }

  //Double check WM_SIZE messages to see if the parent native window is maximized
  if (msg->message == WM_SIZE)
  {
    if (p_Widget && p_Widget->maximizeButton)
    {
      //Get the window state
      WINDOWPLACEMENT wp;
      GetWindowPlacement(native_window_->native_handle(), &wp);

      //If we're maximized,
      if (wp.showCmd == SW_MAXIMIZE)
      {
        p_Widget->maximizeButton->hide();
        p_Widget->restoreButton->show();
      }
      else
      {
        p_Widget->maximizeButton->show();
        p_Widget->restoreButton->hide();
      }
    }
  }

  //Pass NCHITTESTS on the window edges as determined by BORDERWIDTH & TOOLBARHEIGHT through to the parent native window
  if (msg->message == WM_NCHITTEST)
  {
    RECT WindowRect;
    int x, y;

    GetWindowRect(msg->hwnd, &WindowRect);
    x = GET_X_LPARAM(msg->lParam) - WindowRect.left;
    y = GET_Y_LPARAM(msg->lParam) - WindowRect.top;

    if (x >= BORDERWIDTH && x <= WindowRect.right - WindowRect.left - BORDERWIDTH && y >= BORDERWIDTH && y <= TOOLBARHEIGHT)
    {
      if (p_Widget->buttons_)
      {
        //If the mouse is over top of the toolbar area BUT is actually positioned over a child widget of the toolbar,
        //Then we don't want to enable dragging. This allows for buttons in the toolbar, eg, a Maximize button, to keep the mouse interaction
        const auto widget_at_cursor = QApplication::widgetAt(QCursor::pos());
        if (widget_at_cursor != p_Widget->buttons_ && widget_at_cursor != p_Widget->top_drag_area_)
          return false;
      }

      //The mouse is over the toolbar area & is NOT over a child of the toolbar, so pass this message
      //through to the native window for HTCAPTION dragging
      *result = HTTRANSPARENT;
      return true;

    }
    else if (x < BORDERWIDTH && y < BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y < BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x < BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x < BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (y < BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }
    else if (y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
    {
      *result = HTTRANSPARENT;
      return true;
    }

    return false;
  }

  return false;
}

/*!
    \reimp
*/
bool widget::eventFilter(QObject *o, QEvent *e)
{
  QWidget *w = (QWidget*)o;

  switch (e->type())
  {
    case QEvent::WindowDeactivate:
      if (w->isModal() && w->isHidden())
        BringWindowToTop(native_window_->native_handle());
      break;

    case QEvent::Hide:
      if (_reenableParent) {
        EnableWindow(native_window_->native_handle(), true);
        _reenableParent = false;
      }
      resetFocus();

      if (w->testAttribute(Qt::WA_DeleteOnClose) && w->isWindow())
        deleteLater();
      break;

    case QEvent::Show:
      if (w->isWindow()) {
        saveFocus();
        hide();
        if (w->isModal() && !_reenableParent) {
          EnableWindow(native_window_->native_handle(), false);
          _reenableParent = true;
        }
      }
      break;

    case QEvent::Close:
    {
      ::SetActiveWindow(native_window_->native_handle());
      if (w->testAttribute(Qt::WA_DeleteOnClose))
        deleteLater();
      break;
    }
    default:
      break;
  }

  return QWidget::eventFilter(o, e);
}

/*! \reimp
*/
void widget::focusInEvent(QFocusEvent *e)
{
  QWidget *candidate = this;

  switch (e->reason()) {
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
      while (!(candidate->focusPolicy() & Qt::TabFocus)) {
        candidate = candidate->nextInFocusChain();
        if (candidate == this) {
          candidate = 0;
          break;
        }
      }
      if (candidate) {
        candidate->setFocus(e->reason());
        if (e->reason() == Qt::BacktabFocusReason || e->reason() == Qt::TabFocusReason) {
          candidate->setAttribute(Qt::WA_KeyboardFocusChange);
          candidate->window()->setAttribute(Qt::WA_KeyboardFocusChange);
        }
        if (e->reason() == Qt::BacktabFocusReason)
          QWidget::focusNextPrevChild(false);
      }
      break;
    default:
      break;
  }
}

/*! \reimp
*/
bool widget::focusNextPrevChild(bool next)
{
  QWidget *curFocus = focusWidget();
  if (!next) {
    if (!curFocus->isWindow()) {
      QWidget *nextFocus = curFocus->nextInFocusChain();
      QWidget *prevFocus = 0;
      QWidget *topLevel = 0;
      while (nextFocus != curFocus) {
        if (nextFocus->focusPolicy() & Qt::TabFocus) {
          prevFocus = nextFocus;
          topLevel = 0;
        }
        nextFocus = nextFocus->nextInFocusChain();
      }

      if (!topLevel) {
        return QWidget::focusNextPrevChild(false);
      }
    }
  } else {
    QWidget *nextFocus = curFocus;
    while (1 && nextFocus != 0) {
      nextFocus = nextFocus->nextInFocusChain();
      if (nextFocus->focusPolicy() & Qt::TabFocus) {
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