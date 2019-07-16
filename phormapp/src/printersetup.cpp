#include "printersetup.h"
#include <QPrinterInfo>
#include <QString>
#include <QDebug>

/**
 * @brief PrinterSetup::PrinterSetup
 */
PrinterSetup::PrinterSetup()
{

}

/**
 * @brief PrinterSetup::~PrinterSetup
 */
PrinterSetup::~PrinterSetup()
{

}

/**
 * @brief PrinterSetup::setDatabase
 * @param db
 */
void PrinterSetup::setDatabase(const QSqlDatabase &db)
{
    m_database = db;
}

/**
 * @brief PrinterSetup::printersLoadList
 */
void PrinterSetup::printersLoadList()
{
    QPrinterInfo ptrInfo;

    count = ptrInfo.availablePrinters().size();

    for (int i=0; i < count; i++) {
        printList.append(ptrInfo.availablePrinters().at(i));

        qDebug() << "################# Printer name: " << printList.at(i).printerName();
    }
}

/**
 * @brief PrinterSetup::printersCount
 * @return
 */
int PrinterSetup::printersCount()
{
    return count;
}

/**
 * @brief PrinterSetup::printerListAt
 * @param index
 * @return
 */
QString PrinterSetup::printerListAt(int index)
{
    if (count < 0) {
        return QString ("");
    }

    return printList.at(index).printerName();
}

/**
 * @brief PrinterSetup::checkPrintersRecord
 */
void PrinterSetup::checkPrintersRecord()
{
    QString statLine = QString ("SELECT * FROM tb_printer_settings");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    if ( query.size() > 0) {

        while (query.next()) {
            m_labelPrint.append(query.value("labels_printer").toString());
            m_orderPrint.append(query.value("order_printer").toString());
        }
    }
}

/**
 * @brief PrinterSetup::getLabelsPrint
 * @return
 */
QString PrinterSetup::getLabelsPrint(void)
{
    return m_labelPrint;
}

/**
 * @brief PrinterSetup::getOrderPrint
 * @return
 */
QString PrinterSetup::getOrderPrint(void)
{
    return m_orderPrint;
}

/**
 * @brief PrinterSetup::setPrinters
 * @param labels
 * @param order
 * @return
 */
bool PrinterSetup::setPrinters (const QString &labels, const QString &order)
{
    QString statLine = QString ("UPDATE tb_printer_settings SET "
                                "labels_printer='%1', "
                                "order_printer='%2' "
                                "WHERE id=1")
                                .arg(labels)
                                .arg(order);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    return true;
}

