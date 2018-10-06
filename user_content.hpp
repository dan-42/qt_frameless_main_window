#ifndef USER_CONTENT_HPP
#define USER_CONTENT_HPP

#include <QWidget>

namespace Ui {
class user_content;
}

class user_content : public QWidget
{
  Q_OBJECT

public:
  explicit user_content(QWidget *parent = nullptr);
  ~user_content();

private:
  Ui::user_content *ui;
};

#endif // USER_CONTENT_HPP
