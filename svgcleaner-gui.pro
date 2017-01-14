QT += core gui widgets concurrent svg

contains(DEFINES, WITH_CHECK_UPDATES) {
    QT += network
}

TARGET = SVGCleaner
unix:!mac:TARGET = svgcleaner-gui

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/treemodel.cpp \
    src/settings.cpp \
    src/preferences/preferencesdialog.cpp \
    src/preferences/elementspage.cpp \
    src/preferences/attributespage.cpp \
    src/preferences/outputpage.cpp \
    src/aboutdialog.cpp \
    src/preferences/pathspage.cpp \
    src/preferences/cleaneroptions.cpp \
    src/preferences/basepreferencespage.cpp \
    src/preferences/mainpage.cpp \
    src/cleaner.cpp \
    src/doc.cpp \
    src/preferences/widgets/warningcheckbox.cpp \
    src/enums.cpp \
    src/process.cpp \
    src/compressor.cpp \
    src/filesview.cpp \
    src/preferences/widgets/iconlistview.cpp \
    src/detailsdialog.cpp \
    src/exceptions.cpp

HEADERS  += \
    src/mainwindow.h \
    src/treemodel.h \
    src/settings.h \
    src/preferences/preferencesdialog.h \
    src/preferences/elementspage.h \
    src/preferences/attributespage.h \
    src/preferences/outputpage.h \
    src/aboutdialog.h \
    src/preferences/pathspage.h \
    src/preferences/cleaneroptions.h \
    src/preferences/basepreferencespage.h \
    src/preferences/mainpage.h \
    src/cleaner.h \
    src/enums.h \
    src/utils.h \
    src/doc.h \
    src/preferences/widgets/warningcheckbox.h \
    src/process.h \
    src/compressor.h \
    src/filesview.h \
    src/preferences/widgets/iconlistview.h \
    src/detailsdialog.h \
    src/exceptions.h

FORMS    += \
    src/mainwindow.ui \
    src/preferences/elementspage.ui \
    src/preferences/attributespage.ui \
    src/preferences/outputpage.ui \
    src/aboutdialog.ui \
    src/preferences/pathspage.ui \
    src/preferences/mainpage.ui \
    src/detailsdialog.ui

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
    bin.files          += svgcleaner-gui
}
