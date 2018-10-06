#include <QApplication>
#include <QPalette>
#include <QWidget>
#include <frameless/frameless_window.hpp>

int main(int argc, char *argv[])
{    
  //This has the app draw at HiDPI scaling on HiDPI displays, usually two pixels for every one logical pixel
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  //This has QPixmap images use the @2x images when available
  //See this bug for more details on how to get this right: https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);

  //A common feature is to save your app's geometry on close such that you can draw in the same place on relaunch
  //Thus this project supports specifying the X/Y/Width/Height in a cross-platform manner
  int windowXPos, windowYPos, windowWidth, windowHeight;
  windowXPos = 100;
  windowYPos = 100;
  windowWidth = 1024;
  windowHeight = 768;

  frameless::frameless_window w1{};
  w1.geometry(windowXPos, windowYPos, windowWidth, windowHeight);
  w1.show();

  {
  auto content = new QWidget{};
  QPalette Pal;
  Pal.setColor(QPalette::Background, Qt::green);
  content->setAutoFillBackground(true);
  content->setPalette(Pal);
  content->show();
  w1.content(content);
  }


//    frameless::frameless_window w2{};
//    w2.geometry({windowXPos, windowYPos, windowWidth, windowHeight});
//    w2.show();

  return app.exec();
}
