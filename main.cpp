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
#include <frameless/frameless_window.hpp>
#include <frameless/windows10.hpp>

int main(int argc, char *argv[])
{    
  // Initial window position and size
  const auto start_up_size_pos = QRect{100, 100, 1024, 768};

  // This has the app draw at HiDPI scaling on HiDPI displays,
  // usually two pixels for every one logical pixel
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  // This has QPixmap images use the @2x images when available
  // See this bug for more details on how to get this right:
  // https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);

  // This is your user code!
  // here you can add your used content
  auto content = new QWidget{};  
  QPalette Pal;  
  QColor bg_color{8, 81, 21}; //dark green
  Pal.setColor(QPalette::Background, bg_color);
  content->setAutoFillBackground(true);
  content->setPalette(Pal);


  // We want it to look like windows10
  auto win10_look = new frameless::windows10;

  // We add the user content
  win10_look->content(content);

  // Because frameless seperates window-logic from the look
  // we need to connect both together.
  // This allows you to create a fullscreen application,
  // with no close/minimize/... buttons
  // or even with a lot of custom buttons
  frameless::frameless_window main_window{win10_look};
  QObject::connect(win10_look, &frameless::windows10::maximize, [&main_window](){ main_window.maximize(); });
  QObject::connect(win10_look, &frameless::windows10::minimize, [&main_window](){ main_window.minimize(); });
  QObject::connect(win10_look, &frameless::windows10::restore,  [&main_window](){ main_window.restore(); });
  QObject::connect(win10_look, &frameless::windows10::close,    [&main_window]()
  {
    main_window.close();
    QApplication::quit();
  });
  // These two needed by the win10_look, so we can display different buttons,
  // depending on the current state maximizes or floating
  QObject::connect(&main_window, &frameless::frameless_window::on_restore,   [&win10_look](){ win10_look->restored(); });
  QObject::connect(&main_window, &frameless::frameless_window::on_maximize,  [&win10_look](){ win10_look->maximized(); });

  // The add_draggable_areas() gives the main_window a vector of qwidgets,
  // when the user left-click-hold on this area, he can move the window around
  // like you can do on the "normal" frame on windows.
  main_window.add_draggable_areas(win10_look->draggable_widgets());

  // here we set the initial window size
  // otherwise this example window would be very tiniy
  main_window.geometry(start_up_size_pos);
  main_window.show();

  return app.exec();
}
