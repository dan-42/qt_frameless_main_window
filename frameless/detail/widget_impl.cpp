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

#include <frameless/detail/widget_impl.hpp>

#include <QLabel>
#include <QLayout>

namespace frameless
{
namespace detail
{

widget_impl::widget_impl(QWidget *parent)
  : QWidget(parent)
{
  {
    buttons_ = new QWidget{this};
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::red);
    buttons_->setAutoFillBackground(true);
    buttons_->setPalette(Pal);
    buttons_->setFixedSize(200, 30);
    buttons_->show();
    buttons_->setGeometry(this->width()-buttons_->width(), 0, 200, 30);
  }
  {
    top_drag_area_ = new QWidget{this};
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::red);
    top_drag_area_->setAutoFillBackground(true);
    top_drag_area_->setPalette(Pal);
    top_drag_area_->show();
    top_drag_area_->setGeometry(0, 0, this->width(), 18);

    QWidget* leftSpacer = new QWidget;
    leftSpacer->setAttribute(Qt::WA_TransparentForMouseEvents);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftSpacer->setStyleSheet("background-color: none; border: none;");
    leftSpacer->show();

    auto layout = new QHBoxLayout{};
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(leftSpacer);
    top_drag_area_->setLayout(layout);
  }

	//Create spacer widgets to keep the title centered
  QWidget* leftSpacer = new QWidget(buttons_);
	leftSpacer->setAttribute(Qt::WA_TransparentForMouseEvents);
	leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	leftSpacer->setStyleSheet("background-color: none; border: none;");

  const auto config_button = [](auto& button, const auto& txt)
  {
    const QString button_style = {"background-color: none; border: none;"};
    QFont font(QStringLiteral("Segoe MDL2 Assets"));
    font.setStyleHint(QFont::Monospace);
    button->setStyleSheet(button_style);
    button->setFont(font);
    button->setText(QString::fromUtf8(txt));
    button->setFixedSize(45, 30);
  };
	//Create the min/max/close buttons
  minimizeButton = new QPushButton{};
  maximizeButton = new QPushButton{};
  restoreButton = new QPushButton{};
  closeButton = new QPushButton{};

  config_button(minimizeButton, u8"\uE921");
  config_button(maximizeButton, u8"\uE922");
  config_button(restoreButton, u8"\uE923");
  config_button(closeButton, u8"\uE8BB");

  auto buttons_layout = new QHBoxLayout{};
  buttons_layout->setSpacing(0);
  buttons_layout->setMargin(0);
  buttons_layout->addWidget(leftSpacer);
  buttons_layout->addWidget(minimizeButton);
  buttons_layout->addWidget(restoreButton);
  buttons_layout->addWidget(maximizeButton);
  buttons_layout->addWidget(closeButton);
  buttons_layout->setAlignment(minimizeButton, Qt::AlignTop);
  buttons_layout->setAlignment(restoreButton, Qt::AlignTop);
  buttons_layout->setAlignment(maximizeButton, Qt::AlignTop);
  buttons_layout->setAlignment(closeButton, Qt::AlignTop);
  buttons_->setLayout(buttons_layout);
}

///
///
auto widget_impl::content(QWidget* c) -> void
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
auto widget_impl::resizeEvent(QResizeEvent* event) -> void
{
  buttons_->move(this->width()-buttons_->width(), 0);
  top_drag_area_->setGeometry(0, 0, width(), 18);
}


} //namespace detail
} //namespace frameless
