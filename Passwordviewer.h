#ifndef PASSWORDVIEWER_H
#define PASSWORDVIEWER_H

#include "ui_PasswordViewer.h"

#include <QDialog>
class PasswordViewer : public QDialog, private Ui::PasswordViewer
{
Q_OBJECT
public:
 PasswordViewer(QWidget *parent, QString password);
};
#endif
