#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QAbstractItemModel>
#include <QAction>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
Q_OBJECT
public:
 TrayIcon(QAbstractItemModel *model, QObject *parent);
 ~TrayIcon();
 void setHideOnCloseChecked(bool checked);
 void setAlwaysOnTopState(bool checked);
 bool getAlwaysOnTopState();
 int getVisibleElementsAmount();
 void setVisibleElementsAmount(int amount);
private:
 QMenu *menu;
 QAction *hideOnCloseAction;
 QAction *alwaysOnTopAction;
 QAbstractItemModel *model;
 int visibleElementsAmount;
 void changeVisibleElementsAmount(int amount);
private slots:
 void handleActivated(QSystemTrayIcon::ActivationReason reason);
 void changeData(const QModelIndex &topLeft, const QModelIndex &bottomRight);
 void copyDataToClipboard(QAction *action);
 void insertRows(const QModelIndex &parent, int start, int end);
 void removeRows(const QModelIndex &parent, int start, int end);
signals:
 void clicked();
 void hideOnCloseTriggered(bool checked);
 void alwaysOnTopTriggered(bool checked);
 void quitClicked();
};
#endif //TRAYICON_H
