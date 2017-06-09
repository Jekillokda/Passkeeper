#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QDate>
#include <QClipboard>
#include <QApplication>
#include <QItemSelectionModel>

#include "TrayIcon.h"
#include "datamodel.h"
namespace Ui {
class MainWindow;
class AboutDialog;
class CheckPerson;
}

class MainWindow : public QMainWindow
{
 Q_OBJECT
public:
 explicit MainWindow(DataModel *model,QWidget *parent = 0);
 ~MainWindow();
 bool isUnsaved();
 int clipboardTimeout=100;
public slots:
 void saveItem(const QModelIndex &item = QModelIndex());
protected:
 void changeEvent(QEvent *e);
 void closeEvent(QCloseEvent * event);
private slots:
 void addnew();
 void removeSelectedItem();
 void showSelectedItem( const QItemSelection & selected, const QItemSelection & deselected );
 void about();
 void quit();
 void switchEchoMode();
 void copyURL();
 void copyLogin();
 void copyPassword();
 void goToURL();
 void generatePassword();
 void enableSaveButton();
 void showHideWindow();
 void showPreferencesDialog();
 void sortAscending();
 void sortDescending();
 void sortEntries(Qt::SortOrder order);
 void importDatabase();
 void exportDatabase();
 void showPasswordViewer();
 void changePassword();
 void checkPassDiff();
 void switchHideOnClose(bool checked);
 void switchAlwaysOnTop(bool alwaysOnTop);
private:
 bool hideOnClose=false;
 QClipboard* qclipboard = QApplication::clipboard();
 Ui::MainWindow *ui;
 Ui::AboutDialog *wabout;
 Ui::CheckPerson *check;
 TrayIcon *trayIcon;
 DataModel *model;
 QSortFilterProxyModel *proxyModel;
 QItemSelectionModel *selectionModel;
 bool showPasswordByDefault;
 void writeSettings();
 void writeWindowState();
 void readSettings();
 void readWindowState();
};
#endif // MAINWINDOW_H
