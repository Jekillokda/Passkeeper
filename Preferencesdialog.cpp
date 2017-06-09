#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{
 setupUi(this);
}

void PreferencesDialog::setVisibleElementsAmount(int amount)
{
 visibleElementsAmountBox->setValue(amount);
}

int PreferencesDialog::getVisibleElementsAmount()
{
 return visibleElementsAmountBox->value();
}

void PreferencesDialog::setClipboardTimeout(int timeout)
{
 if(timeout == 0)
  clipboardBox->setChecked(false);
 else
  clipboardBox->setChecked(true);
  clipboardTimeoutBox->setValue(timeout);
}

int PreferencesDialog::getClipboardTimeout()
{
 if(!clipboardBox->isChecked())
  return 0;
 else
 return clipboardTimeoutBox->value();
}

void PreferencesDialog::setShowPassword(bool show)
{
 showPasswordBox->setChecked(show);
}

bool PreferencesDialog::getShowPassword()
{
 return showPasswordBox->isChecked();
}
