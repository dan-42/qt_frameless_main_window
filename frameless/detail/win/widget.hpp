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

Copyright 2013 Digia Plc and/or its subsidiary(-ies). BSD
Copyright 2017 github.com/dfct
Copyright 2018 github.com/dan-42
*/

#pragma once
#ifndef FRAMELESS_widget_H
#define FRAMELESS_widget_H

#include <QWidget>
#include <QVBoxLayout>

#include <frameless/detail/widget_impl.hpp>
#include <frameless/detail/win/native_window.hpp>

namespace frameless
{
namespace detail
{
namespace win
{

class widget : public QWidget
{
    Q_OBJECT
public:
    widget();
    ~widget() override;

    void show();
    void center();
    void showCentered();
    void setGeometry(int x, int y, int w, int h);

    HWND getParentWindow() const;

public slots:
    void onMaximizeButtonClicked();
    void onMinimizeButtonClicked();
    void onCloseButtonClicked();

protected:
    void childEvent( QChildEvent *e ) override;
    bool eventFilter( QObject *o, QEvent *e ) override;

    bool focusNextPrevChild(bool next) override;
    void focusInEvent(QFocusEvent *e) override;

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
private:
    QVBoxLayout m_Layout;

    widget_impl* p_Widget;

    native_window* p_ParentWinNativeWindow;
    HWND m_ParentNativeWindowHandle;

    HWND _prevFocus;
    bool _reenableParent;

    int BORDERWIDTH = 6;		//Adjust this as you wish for # of pixels on the edges to show resize handles
    int TOOLBARHEIGHT = 40; //Adjust this as you wish for # of pixels from the top to allow dragging the window

    void saveFocus();
    void resetFocus();
};

} //namespace win
} //namespace detail
} //namespace frameless


#endif // FRAMELESS_widget_H