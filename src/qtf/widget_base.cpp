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


#include <qtf/widget_base.hpp>

#include <qtf/frameless/frameless_window.hpp>
#include <qtf/frameless/windows10.hpp>

#include <QRect>
#include <QApplication>

namespace qtf
{

///
///
widget_base::widget_base(QWidget* parent)
  : window_{nullptr}
  , decoration_{nullptr}
{
  set_parent_impl(parent);
}

///
///
widget_base::~widget_base()
{
  if(window_)
  {
    window_->deleteLater();
  }
  if(decoration_)
  {
    decoration_->deleteLater();
  }
}

///
///
auto widget_base::setParent(QWidget* parent) -> void
{
  const auto current_parent = this->parentWidget();
  if(parent == nullptr && current_parent == nullptr)
  {
    set_parent_impl(parent);
    QWidget::setParent(parent);
  }
  else if(parent == nullptr && current_parent != nullptr)
  {
    QWidget::setParent(parent);
    set_parent_impl(parent);
  }
  else if(parent != nullptr && current_parent == nullptr)
  {
    QWidget::setParent(parent);
    this->close();
  }
  else if(parent != nullptr && current_parent != nullptr)
  {
    QWidget::setParent(parent);

    if(window_)
    {
      window_->hide();
      window_->close();
      window_->deleteLater();
    }
  }
}

///
///
auto widget_base::setParent(QWidget* parent, Qt::WindowFlags f) -> void
{
  set_parent_impl(parent);
  QWidget::setParent(parent, f);
}

///
///
auto widget_base::set_parent_impl(QWidget* parent) ->  void
{
  const auto start_up_size_pos = QRect{100, 100, 1024, 768};

  if(parent == nullptr)
  {    
    // We want it to look like windows10
    decoration_ = new qtf::frameless::windows10;

    // We add the user content
    decoration_->content(this);

    // Because frameless seperates window-logic from the look
    // we need to connect both together.
    // This allows you to create a fullscreen application,
    // with no close/minimize/... buttons
    // or even with a lot of custom buttons
    window_ = new qtf::frameless::frameless_window{decoration_};
    QObject::connect(decoration_, &qtf::frameless::windows10::maximize, [this](){ window_->maximize(); });
    QObject::connect(decoration_, &qtf::frameless::windows10::minimize, [this](){ window_->minimize(); });
    QObject::connect(decoration_, &qtf::frameless::windows10::restore,  [this](){ window_->restore(); });
    QObject::connect(decoration_, &qtf::frameless::windows10::close,    [this]()
    {
      if(window_)
      {
        window_->close();
        window_->deleteLater();
      }
      this->close();
    });
    // These two needed by the win10_look, so we can display different buttons,
    // depending on the current state maximizes or floating
    QObject::connect(window_, &qtf::frameless::frameless_window::on_restore,   [this](){ decoration_->restored(); });
    QObject::connect(window_, &qtf::frameless::frameless_window::on_maximize,  [this](){ decoration_->maximized(); });

    // The add_draggable_areas() gives the main_window a vector of qwidgets,
    // when the user left-click-hold on this area, he can move the window around
    // like you can do on the "normal" frame on windows.
    window_->add_draggable_areas(decoration_->draggable_widgets());

    // here we set the initial window size
    // otherwise this example window would be very tiniy
    window_->geometry(start_up_size_pos);
    window_->show();
    window_->maximize();
  }
}

///
///
auto widget_base::closeEvent(QCloseEvent *event) -> void
{
  QWidget::closeEvent(event);
}

///
///
auto widget_base::event(QEvent *e) -> bool
{
  const auto t = e->type();

  if(t == QEvent::ParentChange)
  {
    if(window_ && decoration_)
    {
      decoration_->deleteLater();
      decoration_ = nullptr;
      window_->close();
      window_->deleteLater();
      window_ = nullptr;
    }
  }

  Q_UNUSED(t);
  return QWidget::event(e);
}

} // namespace qtf