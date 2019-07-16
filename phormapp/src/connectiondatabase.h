#ifndef CONNECTIONDATABASE_H
#define CONNECTIONDATABASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QtQuick/QQuickItem>
#include "maintenancehandler.h"
#include "elementhandler.h"
#include "clienthandler.h"
#include "doctorhandler.h"
#include "suppliershandler.h"
#include "localhandler.h"
#include "orderhandler.h"
#include "printersetup.h"

class ConnectionDatabase : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString port READ port WRITE setPort)
    Q_PROPERTY(bool isAssertConn READ isAssertConn)
    Q_PROPERTY(bool isConfigExists READ isConfigExists)

    Q_PROPERTY(MaintenanceHandler *maintenanceHandler READ maintenanceHandler WRITE setMaintenanceHandler)
    Q_PROPERTY(ElementHandler *elementHandler READ elementHandler WRITE setElementHandler)
    Q_PROPERTY(ClientHandler *clientHandler READ clientHandler WRITE setClientHandler)
    Q_PROPERTY(DoctorHandler *doctorHandler READ doctorHandler WRITE setDoctorHandler)
    Q_PROPERTY(SuppliersHandler *suppliersHandler READ suppliersHandler WRITE setSuppliersHandler)
    Q_PROPERTY(LocalHandler *localHandler READ localHandler WRITE setLocalHandler)
    Q_PROPERTY(OrderHandler *orderHandler READ orderHandler WRITE setOrderHandler)
    Q_PROPERTY(PrinterSetup *ptrSetup READ ptrSetup WRITE setPtrSetup)

public:
    explicit ConnectionDatabase(QQuickItem *parent = 0);
    ~ConnectionDatabase();

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    QString port() const;
    int getPort() const;

    void setDriverName (QString const &);
    void setDatabaseName (QString const &);
    void setUserName (QString const &);
    void setPassword (QString const &);
    void setHostName (QString const &);
    void setPort (QString const &);

    QSqlError addConnection (const QString &driver, const QString &dbName, const QString &host,
                             const QString &user, const QString &passwd, int port);

    Q_INVOKABLE bool assertConn();

    bool isConfigExists() const;
    bool isAssertConn() const;
    bool recordConfig() const;

    MaintenanceHandler *maintenanceHandler();
    void setMaintenanceHandler(MaintenanceHandler *handler);

    ElementHandler *elementHandler();
    void setElementHandler(ElementHandler *handler);

    ClientHandler *clientHandler();
    void setClientHandler(ClientHandler *handler);

    DoctorHandler *doctorHandler() const;
    void setDoctorHandler(DoctorHandler *handler);

    SuppliersHandler *suppliersHandler() const;
    void setSuppliersHandler(SuppliersHandler *handler);

    LocalHandler *localHandler() const;
    void setLocalHandler(LocalHandler *handler);

    OrderHandler *orderHandler() const;
    void setOrderHandler(OrderHandler *handler);

    PrinterSetup *ptrSetup() const;
    void setPtrSetup (PrinterSetup *handler);

public slots:

private:
    QString m_driverName;
    QString m_databaseName;
    QString m_userName;
    QString m_password;
    QString m_hostName;
    int m_port;
    bool m_assertConn;

    bool m_isAssertConn;
    bool m_isConfigExists;

    QSqlDatabase db;
    MaintenanceHandler *m_mainHandler;
    ElementHandler *m_elementHandler;
    ClientHandler *m_clientHandler;
    DoctorHandler *m_doctorHandler;
    SuppliersHandler *m_supplierHandler;
    LocalHandler *m_localHandler;
    OrderHandler *m_orderHandler;
    PrinterSetup *m_ptrSetup;

    bool loadConfig ();
    QString parseFile(QString buff) const;

};

#endif // CONNECTIONDATABASE_H
