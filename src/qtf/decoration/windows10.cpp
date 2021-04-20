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

#include <qtf/decoration/windows10.hpp>

#include <QLabel>
#include <QLayout>
#include <QEvent>
#include <QResizeEvent>
#include <QPushButton>

namespace qtf
{
namespace decoration
{

///
///
static const auto config_button = [](auto& button, const auto& txt, auto make_hover_red)
{
  const QString button_style_base = {
    "*{"
    "   background-color: none;"
    "   background-color: blue;"
    "   border: none;"
    "}"};
  const QString button_style_hover = {
    "*:hover{"
    "   background-color: rgba(0,0,0,50);"
    "}"};
  const QString button_style_hover_red = {
    "*:hover{"
    "   background-color: rgb(255, 0, 0);"
    "}"};

  const auto button_style = button_style_base + (make_hover_red ? button_style_hover_red : button_style_hover );
  QFont font(QStringLiteral("Segoe MDL2 Assets"));
  font.setStyleHint(QFont::Monospace);
  button->setStyleSheet(button_style);
  button->setFont(font);
  button->setText(QString::fromUtf8(txt));
  button->setFixedSize(45, 30);
  button->setMinimumSize(45, 30);
  button->setMaximumSize(45, 30);
};

windows10::windows10(QWidget* parent)
  : QWidget(nullptr)
{
  //Create the min/max/close buttons
  button_minimzie_ = new QPushButton{this};
  button_maximize_ = new QPushButton{this};
  button_restore_ = new QPushButton{this};
  button_close_ = new QPushButton{this};

  config_button(button_minimzie_, u8"\uE921", false);
  config_button(button_maximize_, u8"\uE922", false);
  config_button(button_restore_,  u8"\uE923", false);
  config_button(button_close_,    u8"\uE8BB", true);

  connect(button_minimzie_, &QPushButton::clicked, [this](){ emit minimize(); });
  connect(button_maximize_, &QPushButton::clicked, [this](){ emit maximize(); });
  connect(button_restore_,  &QPushButton::clicked, [this](){ emit restore(); });
  connect(button_close_,    &QPushButton::clicked, [this](){ emit close(); });

  //space left of buttons, which can be used to hold and drag the application
  QWidget* left_drag_spacer = new QWidget{this};
  //left_drag_spacer->setAttribute(Qt::WA_TransparentForMouseEvents);
  left_drag_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  left_drag_spacer->setStyleSheet("background-color: none; border: none;");
  left_drag_spacer->setMinimumWidth(30);  
  drag_areas_.push_back(left_drag_spacer);

  auto layout = new QHBoxLayout{};
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(left_drag_spacer);
  layout->addWidget(button_minimzie_);
  layout->addWidget(button_maximize_);
  layout->addWidget(button_restore_);
  layout->addWidget(button_close_);
  layout->setAlignment(button_minimzie_, Qt::AlignTop);
  layout->setAlignment(button_maximize_, Qt::AlignTop);
  layout->setAlignment(button_restore_, Qt::AlignTop);
  layout->setAlignment(button_close_, Qt::AlignTop);
  setLayout(layout);
  
  setMinimumSize(45, 30);
  setMaximumSize(250, 30);  

  setStyleSheet("background-color: green; border: none;");

  set_parent_impl(parent);
}

///
///
windows10::~windows10() noexcept = default;

///
///
auto windows10::restored() -> void
{
  button_maximize_->show();
  button_restore_->hide();
}

///
///
auto windows10::maximized() -> void
{
  button_maximize_->hide();
  button_restore_->show();
}

///
///
auto windows10::drag_areas() const -> std::vector<QWidget*>
{
  return drag_areas_;
}

///
///
auto windows10::setParent(QWidget* parent) -> void
{
  set_parent_impl(parent);
}

///
///
auto windows10::set_parent_impl(QWidget* new_parent) -> void
{
  const auto current_parent = parentWidget();
  if(new_parent == current_parent)
  {
    return;
  }

  if(current_parent)
  {
    current_parent->removeEventFilter(this);
  }

  if(new_parent)
  {
    new_parent->installEventFilter(this);
    if( new_parent->isMaximized() )
    {
      maximized();
    }
    else
    {
      restored();
    }
  }

  QWidget::setParent(new_parent);
  update_position();
}

///
///
auto windows10::update_position() -> void
{
  const auto parent = parentWidget();
  if (parent)
  {
    move(parent->width() - this->width(), 0);
    raise();
  }
}

///
///
auto windows10::eventFilter(QObject *object, QEvent *event) -> bool
{
  const auto parent = parentWidget();
  const auto is_correct_object = parent && object == parent;
  const auto is_expected_event = is_correct_object && (event->type() == QEvent::Resize || event->type() == QEvent::Move);
  if (is_expected_event)
  {
    update_position();
  }  
  return false;
}

} //namespace decoration
} //namespace qtf