#ifndef PRINTERSETUP_H
#define PRINTERSETUP_H

#include <QQuickItem>
#include <QPrinterInfo>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

class PrinterSetup : public QQuickItem
{
    Q_OBJECT

public:
    PrinterSetup();
    ~PrinterSetup();

    Q_INVOKABLE void printersLoadList();
    Q_INVOKABLE int printersCount();
    Q_INVOKABLE QString printerListAt(int index);

    void setDatabase(const QSqlDatabase &db);
    void checkPrintersRecord();

    QString getOrderPrint();
    QString getLabelsPrint();

    Q_INVOKABLE bool setPrinters(const QString &labels, const QString &order);
signals:

public slots:

private:

    QSqlDatabase m_database;
    int count;
    QList<QPrinterInfo> printList;
    QString m_labelPrint;
    QString m_orderPrint;

};

#endif // PRINTERSETUP_H
