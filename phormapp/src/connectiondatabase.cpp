
#include "connectiondatabase.h"
#include <QDebug>

/**
 * @brief ConnectionDatabase::ConnectionDatabase
 * @param parent
 */
ConnectionDatabase::ConnectionDatabase(QQuickItem *parent) : QQuickItem(parent)
{

    QStringList drivers = QSqlDatabase::drivers();

    // remove compat names
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");

    setDriverName("QPSQL");

    if (loadConfig() == true) {
        qDebug("Conectando ao banco de dados");
        QSqlError err = addConnection("QPSQL", m_databaseName, m_hostName,
                      m_userName, m_password, m_port);

        if (!err.isValid()) {
            m_assertConn = true;
        } else {
            m_assertConn = false;
        }
        m_isConfigExists = true;
        qDebug() << err.text();
    } else {
        qDebug() << "Arquivo de configuração não foi encontrado.";
        m_assertConn = false;
        m_isConfigExists = false;
    }
}

/**
 * @brief ConnectionDatabase::~ConnectionDatabase
 */
ConnectionDatabase::~ConnectionDatabase()
{

}

/**
 * @brief ConnectionDatabase::driverName
 * @return
 */
QString ConnectionDatabase::driverName() const
{
    return m_driverName;
}

/**
 * @brief ConnectionDatabase::databaseName
 * @return
 */
QString ConnectionDatabase::databaseName() const
{
    return m_databaseName;
}

/**
 * @brief ConnectionDatabase::userName
 * @return
 */
QString ConnectionDatabase::userName() const
{
    return m_userName;
}

/**
 * @brief ConnectionDatabase::password
 * @return
 */
QString ConnectionDatabase::password() const
{
    return m_password;
}

/**
 * @brief ConnectionDatabase::hostName
 * @return
 */
QString ConnectionDatabase::hostName() const
{
    return m_hostName;
}

/**
 * @brief ConnectionDatabase::port
 * @return
 */
QString ConnectionDatabase::port() const
{
    QString strPort;

    strPort.sprintf("%d", m_port);
    return strPort;
}

/**
 * @brief ConnectionDatabase::getPort
 * @return
 */
int ConnectionDatabase::getPort() const
{
    return m_port;
}

/**
 * @brief ConnectionDatabase::setDriverName
 * @param driver
 */
void ConnectionDatabase::setDriverName(const QString & driver)
{
    m_driverName = driver;
}

/**
 * @brief ConnectionDatabase::setDatabaseName
 * @param db
 */
void ConnectionDatabase::setDatabaseName(const QString & db)
{
    m_databaseName = db;
}

/**
 * @brief ConnectionDatabase::setUserName
 * @param usr
 */
void ConnectionDatabase::setUserName(const QString & usr)
{
    m_userName = usr;
}

/**
 * @brief ConnectionDatabase::setPassword
 * @param pass
 */
void ConnectionDatabase::setPassword(const QString & pass)
{
    m_password = pass;
}

/**
 * @brief ConnectionDatabase::setHostName
 * @param host
 */
void ConnectionDatabase::setHostName(const QString & host)
{
    m_hostName = host;
}

/**
 * @brief ConnectionDatabase::setPort
 * @param port
 */
void ConnectionDatabase::setPort(const QString &port)
{
    bool ok;
    m_port = port.toInt(&ok, 10);
}

/**
 * @brief ConnectionDatabase::assertConn
 * @return
 */
bool ConnectionDatabase::assertConn()
{
    QSqlError err = addConnection(driverName(), databaseName(), hostName(),
                  userName(), password(), getPort());

    if (err.type() != QSqlError::NoError) {
        return false;
    }

    recordConfig();

    if (m_mainHandler->isOpenDB() == false) {
        qDebug() << "Base de dados estava fechada";
        m_mainHandler->setDatabase(db);
        m_elementHandler->setDatabase(db);
        m_clientHandler->setDatabase(db);
        m_doctorHandler->setDatabase(db);
        m_supplierHandler->setDatabase(db);
        m_localHandler->setDatabase(db);
        m_orderHandler->setDatabase(db);
        m_ptrSetup->setDatabase(db);
    }

    return true;
}

/**
 * @brief ConnectionDatabase::addConnection
 * @param driver
 * @param dbName
 * @param host
 * @param user
 * @param passwd
 * @param port
 * @return
 */
QSqlError ConnectionDatabase::addConnection(const QString &driver, const QString &dbName,
                                            const QString &host, const QString &user,
                                            const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    db = QSqlDatabase::addDatabase(driver, QString("Conn%1").arg(cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Conn%1").arg(cCount));
    }
    //connectionWidget->refresh();

    return err;

}

/**
 * @brief ConnectionDatabase::loadConfig
 * @return
 */
bool ConnectionDatabase::loadConfig()
{

    QDir pathApp;

    QString pwdFile (pathApp.currentPath());

    pwdFile.append("/database.conf");

    qDebug() << pwdFile;
//    if (pathApp.exists() == false) {
//        qDebug() << "Diretorio de configurações ainda nao foi criado";

//        pathApp.mkdir("/etc/phormApp");

//    } else {
//        qDebug() << "Diretorio encontrado";
//    }

    QString dataFile;
    QFile file(pwdFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
    }

    QTextStream in(&file);

    dataFile = in.readLine();
    setDatabaseName(parseFile(dataFile));

    dataFile = in.readLine();
    setHostName(parseFile(dataFile));

    dataFile = in.readLine();
    setPort(parseFile(dataFile));

    dataFile = in.readLine();
    setUserName(parseFile(dataFile));

    dataFile = in.readLine();
    setPassword(parseFile(dataFile));

    file.close();
    return true;
}

/**
 * @brief ConnectionDatabase::recordConfig
 * @return
 */
bool ConnectionDatabase::recordConfig() const
{

    QDir pathApp;

    QString pwdFile (pathApp.currentPath());

    pwdFile.append("/database.conf");

    QFile file(pwdFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
    }

    QTextStream out(&file);

    out << "db.name:" << databaseName() << "\n";
    out << "db.host:" << hostName() << "\n";
    out << "db.port:" << port() << "\n";
    out << "db.user:" << userName() << "\n";
    out << "db.pass:" << password() << "\n";

    file.close();

    return true;
}

/**
 * @brief ConnectionDatabase::parseFile
 * @param buff
 * @param property
 * @return
 */
QString ConnectionDatabase::parseFile(QString buff) const
{
    int i = 0;
    QString data;

    i = buff.indexOf(":");

    if (i < 0) {
        return NULL;
    }

    data = buff.mid(i+1);
    return data;
}

/**
 * @brief isConfigExists
 * @return
 */
bool ConnectionDatabase::isConfigExists() const
{
    return m_isConfigExists;
}

/**
 * @brief isAssertConn
 * @return
 */
bool ConnectionDatabase::isAssertConn() const
{
    return m_assertConn;
}

/**
 * @brief ConnectionDatabase::maintenanceHandler
 * @return
 */
MaintenanceHandler *ConnectionDatabase::maintenanceHandler()
{
    return m_mainHandler;
}

/**
 * @brief ConnectionDatabase::setMaintenanceHandler
 * @param handler
 */
void ConnectionDatabase::setMaintenanceHandler(MaintenanceHandler *handler)
{
    m_mainHandler = handler;
    m_mainHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::elementHandler
 * @return
 */
ElementHandler *ConnectionDatabase::elementHandler()
{
    return m_elementHandler;
}

/**
 * @brief ConnectionDatabase::setElementHandler
 * @param handler
 */
void ConnectionDatabase::setElementHandler(ElementHandler *handler)
{
    m_elementHandler = handler;
    m_elementHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::clientHandler
 * @return
 */
ClientHandler *ConnectionDatabase::clientHandler()
{
   return m_clientHandler;
}

/**
 * @brief ConnectionDatabase::setClientHandler
 * @param handler
 */
void ConnectionDatabase::setClientHandler(ClientHandler *handler)
{
    m_clientHandler = handler;
    m_clientHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::doctorHandler
 * @return
 */
DoctorHandler *ConnectionDatabase::doctorHandler() const
{
    return m_doctorHandler;
}

/**
 * @brief ConnectionDatabase::setDoctorHandler
 * @param doctorHandler
 */
void ConnectionDatabase::setDoctorHandler(DoctorHandler *handler)
{
    m_doctorHandler = handler;
    m_doctorHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::suppliersHandler
 * @return
 */
SuppliersHandler *ConnectionDatabase::suppliersHandler() const
{
    return m_supplierHandler;
}

/**
 * @brief ConnectionDatabase::setSuppliersHandler
 * @param handler
 */
void ConnectionDatabase::setSuppliersHandler(SuppliersHandler *handler)
{
    m_supplierHandler = handler;
    m_supplierHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::localHandler
 * @return
 */
LocalHandler *ConnectionDatabase::localHandler() const
{
    return m_localHandler;
}

/**
 * @brief ConnectionDatabase::setLocalHandler
 * @param handler
 */
void ConnectionDatabase::setLocalHandler(LocalHandler *handler)
{
    m_localHandler = handler;
    m_localHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::orderHandler
 * @return
 */
OrderHandler *ConnectionDatabase::orderHandler() const
{
    return m_orderHandler;
}

/**
 * @brief ConnectionDatabase::setOrderHandler
 * @param handler
 */
void ConnectionDatabase::setOrderHandler(OrderHandler *handler)
{
    m_orderHandler = handler;
    m_orderHandler->setDatabase(db);
}

/**
 * @brief ConnectionDatabase::ptrSetup
 * @return
 */
PrinterSetup *ConnectionDatabase::ptrSetup() const
{
    return m_ptrSetup;
}

/**
 * @brief ConnectionDatabase::setPtrSetup
 * @param handler
 */
void ConnectionDatabase::setPtrSetup(PrinterSetup *handler)
{
    m_ptrSetup = handler;
    m_ptrSetup->setDatabase(db);
}

