QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    src

SOURCES += \
    src/tree_view/file_rect.cpp \
    src/tree_view/tree_view.cpp \
    src/main_window/main_window.cpp \
    src/tree_generator/tree_generator.cpp \
    src/tree_generator/console_dialog.cpp \
    src/main.cpp

HEADERS += \
    src/tree_view/file_rect.h \
    src/tree_view/tree_view.h \
    src/main_window/main_window.h \
    src/tree_generator/tree_generator.h \
    src/tree_generator/console_dialog.h

FORMS += \
    src/tree_view/tree_view.ui \
    src/main_window/main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    directory2tree.rc

RC_FILE = directory2tree.rc

#ICON = icon_macos.icns
