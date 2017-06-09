#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T19:12:37
#
#-------------------------------------------------

QT += core gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PassKeeper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \ 
    main.cpp \
    PasswordGeneratorDialog.cpp \
    AboutDialog.cpp \
    TrayIcon.cpp \
    DataAccess.cpp \
    CsvFormat.cpp \
    DataModel.cpp \
    PredefinedSettings.cpp \
    PreviousPasswordDialog.cpp \
    OtherDatabaseDialog.cpp \
    NewDatabaseDialog.cpp \
    Crypt.cpp \
    Passwordviewer.cpp \
    App.cpp \
    Columnorganizationdialog.cpp \
    Databaseexportdialog.cpp \
    Databaseimportdialog.cpp \
    Preferencesdialog.cpp \
    Passwordchangedialog.cpp \
    Mainwindow.cpp

HEADERS  += \
    PasswordGeneratorDialog.h \
    AboutDialog.h \
    TrayIcon.h \
    DataAccess.h \
    DataModel.h \
    CsvFormat.h \
    PredefinedSettings.h \
    PreviousPasswordDialog.h \
    OtherDatabaseDialog.h \
    NewDatabaseDialog.h \
    Crypt.h \
    App.h \
    Columnorganizationdialog.h \
    Databaseexportdialog.h \
    Databaseimportdialog.h \
    Mainwindow.h \
    Preferencesdialog.h \
    Passwordviewer.h \
    Passwordchangedialog.h

FORMS    += \
    mainwindow.ui \
    PasswordGeneratorDialogPage1.ui \
    PasswordGeneratorDialogPage3.ui \
    AboutDialog.ui \
    PreviousPasswordDialog.ui \
    OtherDatabaseDialog.ui \
    NewDatabaseDialog.ui \
    preferencesdialog.ui \
    databaseimportdialog.ui \
    columnorganizationdialog.ui \
    databaseexportdialog.ui \
    Passwordviewer.ui \
    passwordchangedialog.ui

DISTFILES += \
    icons/PassKeeper.ico

RESOURCES += \
    resources/resources.qrc
