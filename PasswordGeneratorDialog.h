#ifndef PASSWORDGENERATORDIALOG_H
#define PASSWORDGENERATORDIALOG_H

#define IMAGE_WIDTH 255
#define IMAGE_HEIGHT 192

#include <QDialog>
#include <QAction>
#include <QStackedWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QProgressBar>
#include <QPixmap>

#include "ui_PasswordGeneratorDialogPage1.h"
#include "ui_PasswordGeneratorDialogPage3.h"

class GraphicGenerator;

enum characterSetValues
{
 smallCharacters = 1,
 bigCharacters = 2,
 digits = 4,
 specialCharacters = 8
};

class PasswordGeneratorDialog : public QDialog
{
Q_OBJECT
public:
 PasswordGeneratorDialog(QWidget *parent);
 QString getResult();
 void setSetAsPasswordEnabled(bool enabled);
private:
 Ui::PasswordGeneratorDialogPage1 uiSettings;
 Ui::PasswordGeneratorDialogPage3 uiResult;
 QStackedWidget *stackedWidget;
 QWidget *settingsWidget;
 QWidget *resultWidget;
 QWidget *generatorWidget;
 QWidget *buttonsWidget;
 QPushButton *nextButton;
 QPushButton *backButton;
 GraphicGenerator *generator;
 QProgressBar *progressBar;
private slots:
 void back();
 void next();
};

class GraphicGenerator : public QLabel
{
 Q_OBJECT
public:
 GraphicGenerator(QProgressBar *progressBar, QWidget *parent);
 void initGenerator(int size, int characterSet);
 QString getResult();
protected:
 void mouseMoveEvent( QMouseEvent *event);
private:
 QProgressBar *progressBar;
 int count;
 int size;
 int value;
 int table[IMAGE_WIDTH][IMAGE_HEIGHT];
 char characterMap[128];
 int characterMapCount;
 QString result;
 signals:
 void generatingEnd();
};

#endif //PASSWORDGENERATORDIALOG_H
