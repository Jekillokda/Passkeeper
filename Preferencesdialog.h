#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "ui_PreferencesDialog.h"
class PreferencesDialog : public QDialog, private Ui::PreferencesDialog
{
Q_OBJECT
public:
 PreferencesDialog(QWidget *parent);
 void setVisibleElementsAmount(int amount);
 int getVisibleElementsAmount();
 void setClipboardTimeout(int timeout);
 int getClipboardTimeout();
 void setShowPassword(bool show);
 bool getShowPassword();
};
#endif
