QT += core gui widgets concurrent svg

contains(DEFINES, WITH_CHECK_UPDATES) {
    QT += network
}

equals(QT_MAJOR_VERSION, 4) {
    error("SVG Cleaner depend on Qt >= 5.6")
}

equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 6) {
    error("SVG Cleaner depend on Qt >= 5.6")
}

TARGET = SVGCleaner
unix:!mac:TARGET = svgcleaner-gui

DEFINES += QT_NO_FOREACH

TEMPLATE = app
CONFIG += C++11

SOURCES += \
    src/aboutdialog.cpp \
    src/cleaner.cpp \
    src/compressor.cpp \
    src/detailsdialog.cpp \
    src/doc.cpp \
    src/enums.cpp \
    src/filesview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/preferences/attributespage.cpp \
    src/preferences/basepreferencespage.cpp \
    src/preferences/cleaneroptions.cpp \
    src/preferences/elementspage.cpp \
    src/preferences/mainpage.cpp \
    src/preferences/outputpage.cpp \
    src/preferences/pathspage.cpp \
    src/preferences/preferencesdialog.cpp \
    src/preferences/widgets/dotwidget.cpp \
    src/preferences/widgets/iconlistview.cpp \
    src/preferences/widgets/warningcheckbox.cpp \
    src/process.cpp \
    src/settings.cpp \
    src/treemodel.cpp

HEADERS += \
    src/aboutdialog.h \
    src/cleaner.h \
    src/compressor.h \
    src/detailsdialog.h \
    src/doc.h \
    src/enums.h \
    src/filesview.h \
    src/mainwindow.h \
    src/preferences/attributespage.h \
    src/preferences/basepreferencespage.h \
    src/preferences/cleaneroptions.h \
    src/preferences/elementspage.h \
    src/preferences/mainpage.h \
    src/preferences/outputpage.h \
    src/preferences/pathspage.h \
    src/preferences/preferencesdialog.h \
    src/preferences/widgets/dotwidget.h \
    src/preferences/widgets/iconlistview.h \
    src/preferences/widgets/warningcheckbox.h \
    src/process.h \
    src/settings.h \
    src/treemodel.h \
    src/utils.h

FORMS += \
    src/aboutdialog.ui \
    src/detailsdialog.ui \
    src/mainwindow.ui \
    src/preferences/attributespage.ui \
    src/preferences/elementspage.ui \
    src/preferences/mainpage.ui \
    src/preferences/outputpage.ui \
    src/preferences/pathspage.ui

contains(DEFINES, WITH_CHECK_UPDATES) {
    SOURCES += src/updater.cpp
    HEADERS += src/updater.h
}

RESOURCES += icons/icons.qrc data/data.qrc

win32:RC_FILE = icons/icon.rc
mac:ICON      = icons/svgcleaner.icns

unix:!mac {
    isEmpty (PREFIX):PREFIX = /usr

    INSTALLS           += desktop logo bin

    desktop.path        = $$PREFIX/share/applications
    desktop.files      += svgcleaner.desktop

    logo.path           = $$PREFIX/share/icons/hicolor/scalable/apps
    logo.files         += icons/svgcleaner.svg

    bin.path            = $$PREFIX/bin
    bin.files          += $$TARGET
}
