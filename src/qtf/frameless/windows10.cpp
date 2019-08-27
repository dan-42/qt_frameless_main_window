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

#include <qtf/frameless/windows10.hpp>

#include <QLabel>
#include <QLayout>
#include <QPushButton>

namespace qtf
{
namespace frameless
{

windows10::windows10()
  : QWidget(nullptr)
{
  {
    buttons_ = new QWidget{this};
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::transparent);
    buttons_->setAutoFillBackground(true);
    buttons_->setPalette(Pal);
    buttons_->setFixedSize(200, 30);    
    buttons_->setGeometry(this->width()-buttons_->width(), 0, 200, 30);
  }
  {
    top_drag_area_ = new QWidget{this};
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::transparent);
    top_drag_area_->setAutoFillBackground(true);
    top_drag_area_->setPalette(Pal);    
    top_drag_area_->setGeometry(0, 0, this->width(), 18);


    QWidget* left_drag_spacer = new QWidget;
    left_drag_spacer->setAttribute(Qt::WA_TransparentForMouseEvents);
    left_drag_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    left_drag_spacer->setStyleSheet("background-color: none; border: none;");

    auto layout = new QHBoxLayout{};
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(left_drag_spacer);
    top_drag_area_->setLayout(layout);
  }

  const auto config_button = [](auto& button, const auto& txt, auto make_hover_red)
  {
    const QString button_style_base = {
      "*{"
      "   background-color: none;"
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
  };

	//Create the min/max/close buttons
  button_minimzie_ = new QPushButton;
  button_maximize_ = new QPushButton;
  button_restore_ = new QPushButton;
  button_close_ = new QPushButton;

  config_button(button_minimzie_, u8"\uE921", false);
  config_button(button_maximize_, u8"\uE922", false);
  config_button(button_restore_,  u8"\uE923", false);
  config_button(button_close_,    u8"\uE8BB", true);

  //space left of buttons, which can be used
  // to hold and drag the application
  QWidget* left_drag_spacer = new QWidget(buttons_);
  left_drag_spacer->setAttribute(Qt::WA_TransparentForMouseEvents);
  left_drag_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  left_drag_spacer->setStyleSheet("background-color: none; border: none;");

  auto buttons_layout = new QHBoxLayout{};
  buttons_layout->setSpacing(0);
  buttons_layout->setMargin(0);
  buttons_layout->addWidget(left_drag_spacer);
  buttons_layout->addWidget(button_minimzie_);
  buttons_layout->addWidget(button_maximize_);
  buttons_layout->addWidget(button_restore_);
  buttons_layout->addWidget(button_close_);
  buttons_layout->setAlignment(button_minimzie_, Qt::AlignTop);
  buttons_layout->setAlignment(button_maximize_, Qt::AlignTop);
  buttons_layout->setAlignment(button_restore_, Qt::AlignTop);
  buttons_layout->setAlignment(button_close_, Qt::AlignTop);
  buttons_->setLayout(buttons_layout);

  connect(button_minimzie_, &QPushButton::clicked, [this](){ emit minimize(); });
  connect(button_maximize_, &QPushButton::clicked, [this](){ emit maximize(); });
  connect(button_restore_,  &QPushButton::clicked, [this](){ emit restore(); });
  connect(button_close_,    &QPushButton::clicked, [this](){ emit close(); });
}

///
///
auto windows10::content(QWidget* c) -> void
{
  if(!layout())
  {
    auto l = new QHBoxLayout;
    l->setMargin(0);
    setLayout(l);
  }
  layout()->addWidget(c);
  top_drag_area_->raise();
  buttons_->raise();
}

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
auto windows10::draggable_widgets() const -> std::vector<QWidget*>
{
  return std::vector<QWidget*>{buttons_, top_drag_area_};
}

///
///
auto windows10::resizeEvent(QResizeEvent* /*event*/) -> void
{
  buttons_->move(this->width()-buttons_->width(), 0);
  top_drag_area_->setGeometry(0, 0, width(), 18);
}


} //namespace frameless
} //namespace qtf