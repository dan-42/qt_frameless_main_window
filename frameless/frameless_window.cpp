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

#ifdef _WIN32
#  include <Windows.h>
#  include <frameless/detail/win/widget.hpp>
#else
#  include <frameless/detail/widget.hpp>
#endif

#ifdef __APPLE__
#  include <frameless/detail/osx/osx_hide_title_bar.hpp>
#endif

#include "frameless_window.hpp"

namespace frameless
{
///
///
frameless_window::frameless_window()
  : impl_{std::make_unique<detail::widget_type>()}
{
#ifdef __APPLE__
  //Then, hide the OS X title bar
  detail::osx::osx_hide_title_bar::hide(impl_->winId());
#endif
}

///
///
frameless_window::~frameless_window() noexcept = default;

///
///
auto frameless_window::show() -> void
{
  impl_->show();
}

///
///
auto frameless_window::geometry(int x, int y, int w, int h) -> void
{
  impl_->setGeometry(x, y, w, h);
}

///
///
auto frameless_window::content(QWidget* c) -> void
{
  impl_->content(c);
}

} //namespace frameless