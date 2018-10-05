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
	: QMainWindow(parent)
{

	//Set a black background for funsies
	QPalette Pal(palette());
	Pal.setColor(QPalette::Background, Qt::blue);
	setAutoFillBackground(true);
	setPalette(Pal);

	//Windows example of adding a toolbar + min/max/close buttons
#ifdef _WIN32

	//Add the toolbar
	toolBar = new QToolBar(this);
	toolBar->setMovable(false);
	toolBar->setFloatable(false);
	addToolBar(toolBar);

	//Create a transparent-to-mouse-events widget that pads right for a fixed width equivalent to min/max/close buttons
	QWidget* btnSpacer = new QWidget(toolBar);
	btnSpacer->setAttribute(Qt::WA_TransparentForMouseEvents);
	btnSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	btnSpacer->setStyleSheet("background-color: none; border: none;");
	btnSpacer->setFixedWidth(135 /* rough width of close/min/max buttons */);
	toolBar->addWidget(btnSpacer);


	//Create a title label just because
	QLabel* titleLabel = new QLabel("TrueFramelessWindow");
	titleLabel->setFixedWidth(160);

	//Set it transparent to mouse events such that you can click and drag when moused over the label
	titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);


	//Create spacer widgets to keep the title centered
	QWidget* leftSpacer = new QWidget(toolBar);
	QWidget* rightSpacer = new QWidget(toolBar);

	//Set them transparent to mouse events + auto-expanding in size
	leftSpacer->setAttribute(Qt::WA_TransparentForMouseEvents);
	leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	leftSpacer->setStyleSheet("background-color: none; border: none;");
	rightSpacer->setAttribute(Qt::WA_TransparentForMouseEvents);
	rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	rightSpacer->setStyleSheet("background-color: none; border: none;");

	//Add spacers & title label
	toolBar->addWidget(leftSpacer);
	toolBar->addWidget(titleLabel);
	toolBar->addWidget(rightSpacer);


	//Create the min/max/close buttons
	minimizeButton = new QPushButton("-");
	maximizeButton = new QPushButton("O");
	closeButton = new QPushButton("X");

	maximizeButton->setCheckable(true);

	minimizeButton->setFixedSize(45, 28);
	maximizeButton->setFixedSize(45, 28);
	closeButton->setFixedSize(45, 28);

	toolBar->addWidget(minimizeButton);
	toolBar->addWidget(maximizeButton);
	toolBar->addWidget(closeButton);
	toolBar->layout()->setAlignment(minimizeButton, Qt::AlignTop);
	toolBar->layout()->setAlignment(maximizeButton, Qt::AlignTop);
	toolBar->layout()->setAlignment(closeButton, Qt::AlignTop);

	//An actual app should use icons for the buttons instead of text
	//and style the different button states / widget margins in css

#endif

}

} //namespace detail
} //namespace frameless
