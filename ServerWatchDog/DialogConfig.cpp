#include "DialogConfig.h"
#include "ui_DialogConfig.h"

#include <QMessageBox>
#include "widgetutils.h"

DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfig)
{
    ui->setupUi(this);
    // 设置选择模式为整行选择
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    SetCommHeader(ui->tableWidget);
    LoadConfig();
}

void DialogConfig::SetCommHeader(QTableWidget *table)
{
    QStringList headerText;
    headerText<<"程序名"<<"程序路径";
    table->setColumnCount(headerText.size());      //设置表格列数
    for (int i=0;i<table->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setPointSize(10);           //字体大小
        headerItem->setFont(font);       //设置字体
        table->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }

    SetTableColunWidth(table);
}

void DialogConfig::SetTableColunWidth(QTableWidget *table)
{
    QList<int> columnWidths;
    columnWidths << 200 << 240;
    for(int i=0;i<columnWidths.count();i++)
    {
        table->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Interactive);
        table->setColumnWidth(i,columnWidths.at(i));
    }

    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    table->setAlternatingRowColors(true);
}

DialogConfig::~DialogConfig()
{
    delete ui;
}

void DialogConfig::on_btnInsert_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void DialogConfig::on_btnDelete_clicked()
{
    QModelIndexList rows=ui->tableWidget->selectionModel()->selectedRows();
    if(rows.count()==1)
    {
        ui->tableWidget->removeRow(rows[0].row());
    }
}

void DialogConfig::on_btnSaveCfg_clicked()
{
    SaveConfig();
}

void DialogConfig::on_btnLoadCfg_clicked()
{
    LoadConfig();
}

void DialogConfig::LoadConfig()
{
    WatchDogParam p=GlobalVar::Instance().GetWatchDogParam();
    ui->chbAutoRun->setChecked(p.IsAutoRun);
    ui->spinCycle->setValue(p.CycleInterval);
    SetItems(p.WatchDogItems,ui->tableWidget);
}



void DialogConfig::SaveConfig()
{
    WidgetUtils::Execute(sender(),[this]{
        WatchDogParam p;
        p.IsAutoRun=ui->chbAutoRun->isChecked();
        p.CycleInterval=ui->spinCycle->value();
        p.WatchDogItems=GetItems(ui->tableWidget);
        ui->chbAutoRun->setChecked(p.IsAutoRun);
        ui->spinCycle->setValue(p.CycleInterval);
        GlobalVar::Instance().SetWatchDogParam(p);

        QMessageBox::information(this,"提示","配置保存成功，重启后生效!");
    });
}

void DialogConfig::SetItems(QList<WatchDogConfigItem> items, QTableWidget *table)
{
    table->setRowCount(0);
    QList<WatchDogConfigItem> items1=items;
    table->setRowCount(items1.count());

    for(int i=0;i<items1.count();i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setData(Qt::DisplayRole, items1[i].Name);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        table->setItem(i, 0, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setData(Qt::DisplayRole, items1[i].APPPath);
        item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        table->setItem(i, 1, item2);
    }
}

QList<WatchDogConfigItem> DialogConfig::GetItems(QTableWidget *table)
{
    QList<WatchDogConfigItem> ret;
    int row=ui->tableWidget->rowCount();
    for(int i=0;i<row;i++)
    {
        if(table->item(i,0)->data(Qt::DisplayRole).toString().isEmpty())
        {
            continue;
        }
        WatchDogConfigItem tmp;
        tmp.Name=table->item(i,0)->data(Qt::DisplayRole).toString();
        tmp.APPPath=table->item(i,1)->data(Qt::DisplayRole).toString();
        if(!QFile::exists(tmp.APPPath))
        {
            throw QString("%1不存在").arg(tmp.APPPath);
        }
        ret.append(tmp);
    }
    return ret;
}
