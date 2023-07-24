#ifndef WIDGETUTILSNEW_H
#define WIDGETUTILSNEW_H
#include <QWidget>
#include <functional>
#include <QDebug>

namespace WidgetUtils {

    template<typename Func>
    void Execute(QObject *widget,Func func,bool showExMsg=true)
    {
        QWidget* control=dynamic_cast<QWidget*>(widget);
        try
        {
            if(control!=nullptr)
            {
                control->setEnabled(false);
            }
            func();
        }
        catch(QString ex)
        {
            if(showExMsg)
            {
                WidgetUtilsNew::ShowWarning(ex);
            }
            qDebug()<<ex;
        }

        if(control!=nullptr)
        {
            control->setEnabled(true);
        }
    }

    class WidgetUtilsNew
    {
    public:
        WidgetUtilsNew();
        static void ShowWarning(const QString &warningText);
    };

}


#endif // WIDGETUTILSNEW_H
