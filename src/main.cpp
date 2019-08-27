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

#include <QApplication>
#include <QPalette>
#include <QWidget>
#include <QVBoxLayout>
#include <qtf/widget_base.hpp>
#include <qtf/frameless/windows10.hpp>

int main(int argc, char *argv[])
{    
  // This has the app draw at HiDPI scaling on HiDPI displays,
  // usually two pixels for every one logical pixel
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  // This has QPixmap images use the @2x images when available
  // See this bug for more details on how to get this right:
  // https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(true);

  // This is your user code!
  // here you can add your used content  
  QPalette Pal;  
  QColor bg_color{8, 81, 21}; //dark green
  Pal.setColor(QPalette::Background, bg_color);

  auto widget1 =  new qtf::widget_base{};
  widget1->show();
  widget1->setAutoFillBackground(true);
  widget1->setPalette(Pal);



  QPalette Pal2;
  QColor bg_color2{21, 8, 81};
  Pal2.setColor(QPalette::Background, bg_color2);
  auto widget2 = new QWidget{};
  widget2->setAutoFillBackground(true);
  widget2->setPalette(Pal2);
  widget2->setLayout(new QVBoxLayout{});
  widget2->setGeometry(100, 100, 300, 400);
  widget2->layout()->setMargin(10);
  //widget2->layout()->addWidget(widget1);
  widget2->show();


  return app.exec();
}
