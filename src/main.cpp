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

#include <QApplication>
#include <QPalette>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <qtf/widget.hpp>
#include <qtf/decoration/windows10.hpp>

static constexpr auto dummy_text = 
R"(This PC\Downloads
This PC\Downloads
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut 
labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo 
dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor
sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod
tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et
accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata
sanctus est Lorem ipsum dolor sit amet.
)";


static const auto make_demo = [](auto* widget, const auto& bg_color) -> void
{
  QPalette Pal;    
  Pal.setColor(QPalette::Background, bg_color);  
  widget->setGeometry(100,100, 400, 300);
  auto layout = new QVBoxLayout{widget};
  widget->setLayout(layout);
  widget->show();
  widget->setAutoFillBackground(true);
  widget->setPalette(Pal);
  widget->setContentsMargins(20,20,20,20);
  auto l1 = new QLabel{widget};
  l1->setContentsMargins(0,0,0,0);
  l1->setText(dummy_text);
  l1->setWordWrap(true);
  l1->show();
  l1->setStyleSheet("font-size: 9pt; color: #d4d4d4; background-color: #c1c1c1; border: 1px solid red;");

  auto b1 = new QPushButton{widget};
  b1->setText("click");
  b1->show();
  b1->connect(b1, &QPushButton::clicked, [l1, b1]()
  { 
    static auto b = false;
    if(b)
    {
      l1->setStyleSheet("font-size: 9pt; color: #d4d4d4; background-color: #c1c1c1; border: 1px solid red;");
      b = false;
    }
    else
    {
      l1->setStyleSheet("font-size: 12pt; color: #d4d4d4; background-color: #c1c1c1; border: 1px solid red;");
      b = true;
    }    
    b1->raise();
  });
  layout->addWidget(l1);
  layout->addWidget(b1);
  b1->raise();
};

int main(int argc, char *argv[])
{    
  // SetProcessDpiAwareness(PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE); 
  // // This has the app draw at HiDPI scaling on HiDPI displays,
  // // usually two pixels for every one logical pixel
  // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  // //QApplication::setAttribute(Qt::AA_NativeWindows);
  // //QApplication::setAttribute(Qt::AA_MSWindowsUseDirect3DByDefault);

  // // This has QPixmap images use the @2x images when available
  // // See this bug for more details on how to get this right:
  // // https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
  // QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(true);

  
  // {
  //   auto widget1 =  new qtf::widget_base{};
  //   QColor bg_color{147, 204, 124}; //light green
  //   make_demo(widget1, bg_color);    
  // }
  {
    QColor bg_color{32, 32, 32};
    auto widget1 =  new QWidget{};
    make_demo(widget1, bg_color);
  } 
  {
  auto widget1 =  new qtf::widget{};
  QColor bg_color{32, 32, 32};
  make_demo(widget1, bg_color);    

  auto decoration = new qtf::decoration::windows10{};
  widget1->setDecoration(decoration);
  
  }

  


  return app.exec();
}
