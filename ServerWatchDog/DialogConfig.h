#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include "Head.h"
#include <QDialog>
#include <QTableWidget>
#include "GlobalVar.h"

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();

private slots:
    void on_btnLoadCfg_clicked();

    void on_btnInsert_clicked();

    void on_btnDelete_clicked();

    void on_btnSaveCfg_clicked();

private:
    Ui::DialogConfig *ui;
    void SetCommHeader(QTableWidget *table);
    void SetTableColunWidth(QTableWidget *table);

    void LoadConfig();
    void SaveConfig();

    void SetItems(QList<WatchDogConfigItem> items, QTableWidget *table);
    QList<WatchDogConfigItem> GetItems(QTableWidget *table);
};

#endif // DIALOGCONFIG_H
