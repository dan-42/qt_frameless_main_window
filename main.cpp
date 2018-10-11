#include <QApplication>
#include <QPalette>
#include <QWidget>
#include <frameless/frameless_window.hpp>
#include <frameless/windows10.hpp>

int main(int argc, char *argv[])
{    
  //This has the app draw at HiDPI scaling on HiDPI displays, usually two pixels for every one logical pixel
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  //This has QPixmap images use the @2x images when available
  //See this bug for more details on how to get this right: https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);


  const auto start_up_size_pos = QRect{100, 100, 1024, 768};

  auto content = new QWidget{};  
  QPalette Pal;
  //QColor bg_color{55, 55, 55};
  QColor bg_color{255, 255, 255};
  Pal.setColor(QPalette::Background, bg_color);
  content->setAutoFillBackground(true);
  content->setPalette(Pal);

  auto win10_look = new frameless::windows10;
  win10_look->content(content);

  frameless::frameless_window w1{win10_look};
  QObject::connect(win10_look, &frameless::windows10::maximize, [&w1](){ w1.maximize(); });
  QObject::connect(win10_look, &frameless::windows10::minimize, [&w1](){ w1.minimize(); });
  QObject::connect(win10_look, &frameless::windows10::restore,  [&w1](){ w1.restore(); });
  QObject::connect(win10_look, &frameless::windows10::close,    [&w1]()
  {
    w1.close();
    QApplication::quit();
  });
  QObject::connect(&w1, &frameless::frameless_window::on_restore,   [&win10_look](){ win10_look->restored(); });
  QObject::connect(&w1, &frameless::frameless_window::on_maximize,  [&win10_look](){ win10_look->maximized(); });

  w1.add_draggable_areas(win10_look->draggable_widgets());
  w1.geometry(start_up_size_pos);
  w1.show();


  return app.exec();
}
