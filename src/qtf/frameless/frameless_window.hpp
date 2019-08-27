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

Copyright 2018 github.com/dan-42
*/

#ifndef FRAMELESS_WINDOW_HPP
#define FRAMELESS_WINDOW_HPP

#include <QObject>
#include <memory>

#ifdef _WIN32
namespace qtf { namespace frameless { namespace detail { namespace win {
  class widget;
}}}}
namespace qtf { namespace frameless { namespace detail {
  using widget_type = win::widget;
}}}
#else
namespace qtf { namespace frameless { namespace detail {
  class widget_impl;
  using widget_type = widget_impl;
}}}
#endif

class QWidget;

namespace qtf
{
namespace frameless
{

class frameless_window : public QObject
{
  Q_OBJECT
public:
  ///
  /// \brief frameless_window
  ///
  frameless_window(QWidget* content = nullptr);

  ///
  ///
  ///
  ~frameless_window() noexcept;

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
  auto content(QWidget* c) -> void;

  ///
  ///
  ///
  auto geometry(const QRect& g) -> void;

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
  std::unique_ptr<detail::widget_type> impl_;
};

} //namespace frameless
} //namespace qtf
#endif // FRAMELESS_WINDOW_HPP