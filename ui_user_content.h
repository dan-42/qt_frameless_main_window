/********************************************************************************
** Form generated from reading UI file 'user_content.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USER_CONTENT_H
#define UI_USER_CONTENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_user_content
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;

    void setupUi(QWidget *user_content)
    {
        if (user_content->objectName().isEmpty())
            user_content->setObjectName(QStringLiteral("user_content"));
        user_content->resize(400, 300);
        verticalLayout = new QVBoxLayout(user_content);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton = new QPushButton(user_content);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(user_content);

        QMetaObject::connectSlotsByName(user_content);
    } // setupUi

    void retranslateUi(QWidget *user_content)
    {
        user_content->setWindowTitle(QApplication::translate("user_content", "Form", nullptr));
        pushButton->setText(QApplication::translate("user_content", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class user_content: public Ui_user_content {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USER_CONTENT_H
