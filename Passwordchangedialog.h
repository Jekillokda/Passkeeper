#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include "ui_PasswordChangeDialog.h"

#include <QDialog>
#include <QString>

class PasswordChangeDialog : public QDialog, private Ui::PasswordChangeDialog
{
Q_OBJECT
public:
 PasswordChangeDialog(const QString &currentPassword, QWidget *parent);
 QString getNewPassword();
private:
 QString currentPassword;
 private slots:
 void accept();
};

#endif //PASSWORDCHANGEDIALOG_H
