#include "user_content.hpp"
#include "ui_user_content.h"

user_content::user_content(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::user_content)
{
  ui->setupUi(this);
}

user_content::~user_content()
{
  delete ui;
}
