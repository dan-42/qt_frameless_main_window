# SPDX-License-Identifier:  MIT
# Licensed under the MIT License <http://opensource.org/licenses/MIT>.
#
# Permission is hereby  granted, free of charge, to any  person obtaining a copy
# of this software and associated  documentation files (the "Software"), to deal
# in the Software  without restriction, including without  limitation the rights
# to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
# copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
# furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
# IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
# FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
# AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
# LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Copyright 2017 github.com/dfct
# Copyright 2018 github.com/dan-42

QT += core gui widgets

TARGET = frameless_window
TEMPLATE = app

SOURCES += \
  $$files(src/*.cpp) \
  $$files(src/qtf/*.cpp) \
  $$files(src/qtf/frameless/*.cpp) \
  $$files(src/qtf/frameless/detail/*.cpp)

HEADERS += \
  $$files(src/*.hpp) \
  $$files(src/qtf/*.hpp) \
  $$files(src/qtf/frameless/*.hpp) \
  $$files(src/qtf/frameless/detail/*.hpp)

INCLUDEPATH += src
win32 {
  # Only include / compile these files on Windows
  SOURCES += $$files(src/qtf/frameless/detail/win/*.cpp)
  HEADERS += $$files(src/qtf/frameless/detail/win/*.hpp)
}

mac {
  # Only include / compile these files on OS X
  OBJECTIVE_SOURCES += $$files(src/qtf/frameless/detail/osx/*.mm)
  HEADERS  += $$files(src/qtf/frameless/detail/osx/*.hpp)

  # Additionally include Cocoa for OS X code
  LIBS += -framework Foundation -framework Cocoa
  INCLUDEPATH += /System/Library/Frameworks/Foundation.framework/Versions/C/Headers
}

FORMS += \
  $$files(src/qtf/*.ui)
