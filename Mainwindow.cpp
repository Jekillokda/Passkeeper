#include "Mainwindow.h"
#include "ui_mainwindow.h"
#include "AboutDialog.h"
#include "TrayIcon.h"
#include "PasswordGeneratorDialog.h"
#include "PreferencesDialog.h"
#include "Databaseimportdialog.h"
#include "Databaseexportdialog.h"
#include "PasswordViewer.h"
#include "Passwordchangedialog.h"

#include <QMessageBox>
#include <QAction>
#include <QFile>
#include <QClipboard>
#include <QSettings>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QTimer>
#include <QCloseEvent>

MainWindow::MainWindow(DataModel *model,QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
 setWindowIcon(QIcon(":/icons/mainIcon"));
 ui->setupUi(this);
 this->model = model;
 proxyModel = new QSortFilterProxyModel(this);
 proxyModel->setSourceModel( model );
 proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
 connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), proxyModel, SLOT(setFilterFixedString(const QString &)));
 ui->listView->setModel(proxyModel);
 selectionModel = ui->listView->selectionModel();
 connect(selectionModel, SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), this, SLOT(showSelectedItem( const QItemSelection &, const QItemSelection &)));
 trayIcon = new TrayIcon(model, this);
 readSettings();

 connect(trayIcon, SIGNAL(clicked()), this, SLOT(showHideWindow()));
 connect(trayIcon, SIGNAL(hideOnCloseTriggered(bool)), this, SLOT(switchHideOnClose(bool)));
 connect(trayIcon, SIGNAL(alwaysOnTopTriggered(bool)), this, SLOT(switchAlwaysOnTop(bool)));
 connect(trayIcon, SIGNAL(quitClicked()), this, SLOT(quit()));
 if(trayIcon->getAlwaysOnTopState())
  setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
 this->show();
 trayIcon->setHideOnCloseChecked( hideOnClose );
 trayIcon->setVisible(true);
}

MainWindow::~MainWindow()
{
 delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
 QMainWindow::changeEvent(e);
 switch (e->type())
 {
  case QEvent::LanguageChange:
  ui->retranslateUi(this);
  break;
 default:
  break;
 }
}

void MainWindow::addnew()
{
    selectionModel->clearSelection();
    bool isEmpty = true;
    int row = proxyModel->rowCount()-1;
    for(int i = 0; i < 4; i++)
    {
     if(!proxyModel->data( proxyModel->index( row, i ) ).toString().isEmpty())
     isEmpty = false;
    }

     if(!isEmpty || row == -1)
    {
      if( !proxyModel->insertRows(proxyModel->rowCount(), 1) )
       {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        box.setText( tr("Error adding entry.") );
        box.setIcon( QMessageBox::Critical );
        box.exec();
       }
    }
     selectionModel->setCurrentIndex( proxyModel->index( proxyModel->rowCount()-1, 0 ), QItemSelectionModel::SelectCurrent);
}

void MainWindow::about()
{
    AboutDialog about(this);
        about.exec();
}

bool MainWindow::isUnsaved()
{
    return ui->saveButton->isEnabled();
}

void MainWindow::saveItem(const QModelIndex &item)
{
    int row;
    if(item.isValid())
    {
        row = item.row();
        if(row >= proxyModel->rowCount())
            return;
    }
    else
        row = selectionModel->selection().indexes()[0].row();
    if( !proxyModel->setData( proxyModel->index( row, 0), ui->nameEdit->text()) )
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        box.setText("Error writing entry!");
        box.setIcon( QMessageBox::Critical );
        box.exec();
    }
    proxyModel->setData( proxyModel->index( row, 1), ui->urlEdit->text());
    proxyModel->setData( proxyModel->index( row, 2), ui->loginEdit->text());
    proxyModel->setData( proxyModel->index( row, 3), ui->passwordEdit->text());

    errorCode err = model->saveDatabase();
    if(err == FILE_ERROR)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        box.setText("Error writing to database!");
        box.setIcon( QMessageBox::Critical );
        box.exec();
    }
    ui->saveButton->setEnabled(false);
}
void MainWindow::quit()
{
    if(ui->saveButton->isEnabled())
    {
        show();
        QMessageBox box(this);
        box.setWindowTitle( tr("Unsaved entry - PassKeeper") );
        box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
        box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box.setIcon( QMessageBox::Warning );
        int res = box.exec();
        if(res == QMessageBox::Save)
            saveItem();
        else if(res == QMessageBox::Cancel)
            return;
    }
    if(isVisible())
        writeWindowState();
    writeSettings();
    qApp->quit();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(hideOnClose)
    {
        writeWindowState();
        setVisible(false);
        event->ignore();
    }
    else
    {
        if(ui->saveButton->isEnabled())
        {
            QMessageBox box(this);
            box.setWindowTitle( tr("Unsaved entry - QPass") );
            box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
            box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            box.setIcon( QMessageBox::Warning );
            int res = box.exec();
            if(res == QMessageBox::Save)
                saveItem();
            else if(res == QMessageBox::Cancel)
            {
                event->ignore();
                return;
            }
        }
        if(isVisible())
            writeWindowState();

        writeSettings();
        event->accept();
    }
}

void MainWindow::switchEchoMode()
{
    if(ui->passwordEdit->echoMode() == QLineEdit::Password)
    {
        ui->passwordEdit->setEchoMode( QLineEdit::Normal );
        ui->showPasswordButton->setChecked(true);
    }
    else
    {
        ui->passwordEdit->setEchoMode( QLineEdit::Password );
        ui->showPasswordButton->setChecked(false);
    }
}
void MainWindow::copyLogin()
{
    QApplication::clipboard()->setText(ui->loginEdit->text());
        QTimer::singleShot(clipboardTimeout*1000, this, SLOT(clearClipboard()));
}

void MainWindow::copyPassword()
{
    QApplication::clipboard()->setText(ui->passwordEdit->text());
    if(clipboardTimeout > 0)
        QTimer::singleShot(clipboardTimeout*1000, this, SLOT(clearClipboard()));
}
void MainWindow::copyURL()
{
    QApplication::clipboard()->setText(ui->urlEdit->text());
}
void MainWindow::goToURL()
{
    QString url = ui->urlEdit->text();
    if( !(url.indexOf("http://") == 0 || url.indexOf("https://") == 0) )
        url.insert(0, "http://");
    if( !QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode)) )
    {
        QMessageBox box(this);
        box.setWindowTitle("PassKeeper");
        box.setText( tr("Failed to open URL") );
        box.setIcon( QMessageBox::Warning );
        box.exec();
    }
}

void MainWindow::generatePassword()
{
    PasswordGeneratorDialog dialog(this);
    dialog.setSetAsPasswordEnabled(true);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog.getResult() != QString::Null())
        {
            if(!ui->passwordEdit->text().isEmpty())
            {
                QMessageBox box(this);
                box.setWindowTitle( tr("PassKeeper") );
                box.setText( tr("Current password is not empty. Are you sure you want to overwrite it?") );
                box.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
                box.setIcon( QMessageBox::Question );
                if(box.exec() == QMessageBox::No)
                    return;
            }
            ui->passwordEdit->setText( dialog.getResult() );
            enableSaveButton();
        }
    }
}

void MainWindow::removeSelectedItem()
{
    QModelIndexList list = selectionModel->selection().indexes();
    if(list.count() == 1)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("Item remove - PassKeeper") );
        box.setIcon( QMessageBox::Warning );
        box.setStandardButtons( QMessageBox::Yes | QMessageBox::No);
        box.setText( tr("Are you sure to delete selected entry?") );
        if(box.exec() == QMessageBox::Yes)
        {
            ui->saveButton->setEnabled(false);
            QModelIndex modelIndex = list[0];
            if( !this->proxyModel->removeRows(modelIndex.row(), 1) )
            {
                QMessageBox box1(this);
                box1.setWindowTitle( tr("PassKeeper") );
                box1.setText( tr("Error removing entry.") );
                box1.setIcon( QMessageBox::Critical );
                box1.exec();
            }

            errorCode err = model->saveDatabase();
            if(err == FILE_ERROR)
            {
                QMessageBox box(this);
                box.setWindowTitle( tr("PassKeeper") );
                box.setText("Error writing to database!");
                box.setIcon( QMessageBox::Critical );
                box.exec();
            }
        }
    }
}

void MainWindow::showSelectedItem( const QItemSelection & selected, const QItemSelection & deselected )
{
    if(ui->saveButton->isEnabled())
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("Unsaved entry - PassKeeper") );
        box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
        box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard);
        box.setIcon( QMessageBox::Warning );
        int res = box.exec();
        if(res == QMessageBox::Save)
            saveItem(deselected.indexes()[0]);
    }
    QModelIndexList list = selected.indexes();
    if(list.count() == 1)
    {
        int row = list[0].row();
        ui->itemWidget->setEnabled(true);
        ui->deleteButton->setEnabled(true);
        ui->actionShowNumberedCharacters->setEnabled(true);

        ui->nameEdit->setText( proxyModel->data( proxyModel->index( row, 0 ) ).toString() );
        ui->urlEdit->setText( proxyModel->data( proxyModel->index( row, 1 ) ).toString() );
        ui->loginEdit->setText( proxyModel->data( proxyModel->index( row, 2 ) ).toString() );
        ui->passwordEdit->setText( proxyModel->data( proxyModel->index( row, 3 ) ).toString() );
    }
    else
    {
        ui->itemWidget->setEnabled(false);
        ui->nameEdit->setText(QString());
        ui->urlEdit->setText(QString());
        ui->loginEdit->setText(QString());
        ui->passwordEdit->setText(QString());
        ui->deleteButton->setEnabled(false);
        ui->actionShowNumberedCharacters->setEnabled(false);
    }

    if(!showPasswordByDefault)
    {
        ui->passwordEdit->setEchoMode( QLineEdit::Password );
        ui->showPasswordButton->setChecked(false);
    }
    else
    {
        ui->passwordEdit->setEchoMode( QLineEdit::Normal );
        ui->showPasswordButton->setChecked(true);
    }

    ui->saveButton->setEnabled(false);
}
void MainWindow::enableSaveButton()
{
    ui->saveButton->setEnabled(true);
}

void MainWindow::showHideWindow()
{
    if(isVisible())
    {
        hide();
    }
    else
    {
        show();
        activateWindow();
    }
}
void MainWindow::writeWindowState()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
void MainWindow::readWindowState()
{
    QSettings settings;
    QVariant pos = settings.value("pos");
    if(pos.isValid())
        move(pos.toPoint());
    QSize size = settings.value("size", QSize(652, 425)).toSize();
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("hideOnClose", hideOnClose);
    settings.setValue("alwaysOnTop", trayIcon->getAlwaysOnTopState());
    settings.setValue("visibleElementsAmount", trayIcon->getVisibleElementsAmount());
    settings.setValue("clipboardTimeout", clipboardTimeout);
    settings.setValue("showPasswordByDefault", showPasswordByDefault);
}

void MainWindow::readSettings()
{
    QSettings settings;
    clipboardTimeout = settings.value("clipboardTimeout", 0).toInt();
    hideOnClose = settings.value("hideOnClose", false).toBool();
    trayIcon->setAlwaysOnTopState( settings.value("alwaysOnTop", false).toBool() );
    trayIcon->setVisibleElementsAmount( settings.value("visibleElementsAmount", 15).toInt() );
    showPasswordByDefault = settings.value("showPasswordByDefault", false).toBool();
}
void MainWindow::showPreferencesDialog()
{
    PreferencesDialog preferences(this);

    preferences.setVisibleElementsAmount( trayIcon->getVisibleElementsAmount() );
    preferences.setClipboardTimeout( clipboardTimeout );
    preferences.setShowPassword( showPasswordByDefault );
    if( preferences.exec() == QDialog::Accepted )
    {
        trayIcon->setVisibleElementsAmount( preferences.getVisibleElementsAmount() );
        clipboardTimeout = preferences.getClipboardTimeout();
        showPasswordByDefault = preferences.getShowPassword();
    }
}
void MainWindow::sortAscending()
{
    sortEntries(Qt::AscendingOrder);
}

void MainWindow::sortDescending()
{
    sortEntries(Qt::DescendingOrder);
}
void MainWindow::sortEntries(Qt::SortOrder order)
{
    selectionModel->clearSelection();
    model->sort(0, order);
    errorCode err = model->saveDatabase();
    if(err == FILE_ERROR)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        box.setText("Error writing to database!");
        box.setIcon( QMessageBox::Critical );
        box.exec();
    }
}

void MainWindow::exportDatabase()
{
    selectionModel->clearSelection();
    DatabaseExportDialog exportDialog(this);
    if(exportDialog.exec() == QDialog::Accepted)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        errorCode err = model->exportDatabase( exportDialog.getPath(), exportDialog.getPassword(), exportDialog.getFormat(), exportDialog.getColumnOrganization());
        if(err == SUCCESS)
        {
            box.setText( tr("Database has been exported successfully.") );
            box.setIcon(QMessageBox::Information);
        }
        else
        {
            box.setText( tr("Error exporting database.") );
            box.setIcon(QMessageBox::Critical);
        }
        box.exec();
    }
}

void MainWindow::importDatabase()
{
    selectionModel->clearSelection();
    DatabaseImportDialog importDialog(this);
    if(importDialog.exec() == QDialog::Accepted)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        bool replace = false;
        if(importDialog.getMode() == DatabaseImportDialog::Replace)
        {
            replace = true;
            QMessageBox askBox(this);
            askBox.setWindowTitle( tr("PassKeeper") );
            askBox.setText( tr("All your existing enries will be replaced. Are you sure you want to continue?") );
            askBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
            askBox.setIcon( QMessageBox::Question );
            if( askBox.exec() == QMessageBox::No )
                return;
        }

        int format = importDialog.getFormat();
        int res = model->importDatabase(importDialog.getPath(), importDialog.getPassword(), replace, format, importDialog.getColumnOrganization());
        if( res == SUCCESS )
        {
            box.setText( tr("Database has been imported successfully.") );
            box.setIcon(QMessageBox::Information);
        }
        else
        {
            box.setText( tr("Error importing database. Probably file is corrupted or has incorrect format") );
            box.setIcon(QMessageBox::Critical);
        }
        box.exec();
    }
}
void MainWindow:: showPasswordViewer()
{
    PasswordViewer viewer(this, ui->passwordEdit->text());
    viewer.exec();
}

void MainWindow::changePassword()
{
    PasswordChangeDialog passwordDialog(model->getPassword(), this);
    if(passwordDialog.exec() == QDialog::Accepted)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        if( model->changePassword( passwordDialog.getNewPassword() ) == SUCCESS )
        {
            box.setText( tr("Password has been changed successfully.") );
            box.setIcon( QMessageBox::Information );
        }
        else
        {
            box.setText( tr("Error changing password.") );
            box.setIcon( QMessageBox::Critical );
        }
        box.exec();
    }
}

void MainWindow:: checkPassDiff()
{
    int smalldigitals=0;
    int bigdigitals=0;
    int numbers=0;
    int symbols=0;
    int code;
    QString pass=ui->passwordEdit->text();
    for(int i=0;i<pass.length();i++)
    {
        bool num=false;
        QChar c=pass.at(i);
        code=c.unicode();
        if (c.isDigit()){ numbers++;num=true;}
        if ((c.isLower())&&(num==false)) smalldigitals++;
        if (c.isUpper()) bigdigitals++;
        if ((code>='!')&&(code<='@')&&(num==false)) symbols++;
    }
    int diff=smalldigitals+bigdigitals*2+numbers*3+symbols*4;
    ui->PassDiff->display (diff);
}
void MainWindow::switchHideOnClose(bool checked)
{
    hideOnClose = checked;
    writeSettings();
}

void MainWindow::switchAlwaysOnTop(bool alwaysOnTop)
{
    if(alwaysOnTop)
    {
        if( isVisible() )
            writeWindowState();
        setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
        readWindowState();
        show();
    }
    else
    {
        bool visible = isVisible();
        if( visible )
            writeWindowState();
        readWindowState();
        setVisible(visible);
    }
    writeSettings();
}
