#include "NewDatabaseDialog.h"

#include <QMessageBox>
NewDatabaseDialog::NewDatabaseDialog(QWidget *parent) : QDialog(parent)
{
 setWindowTitle( tr("New database - PassKeeper") );
 setupUi(this);
}
