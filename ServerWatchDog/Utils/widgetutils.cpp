#include "widgetutils.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

namespace WidgetUtils {
    WidgetUtilsNew::WidgetUtilsNew()
    {

    }

    void WidgetUtils::WidgetUtilsNew::ShowWarning(const QString &warningText)
    {
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setText(warningText);

        int width=200;
        int height=160;
        // 获取屏幕分辨率
        const QRect screenGeometry = QApplication::desktop()->screenGeometry();

        // 计算对话框位置
        const int x = screenGeometry.width() / 2 - width / 2;
        const int y = screenGeometry.height() / 2 - height / 2;

        // 设置对话框位置
        msgBox->setGeometry(x,y,width,height);
        msgBox->exec();

        delete msgBox;
    }
}
