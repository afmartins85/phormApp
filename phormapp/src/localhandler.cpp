#include "localhandler.h"


LocalHandler::LocalHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("Local handler Contructor");
}

LocalHandler::~LocalHandler()
{
    qDebug("Local handler Destructor");
}

/**
 * @brief LocalHandler::setDatabase
 * @param db
 */
void LocalHandler::setDatabase(const QSqlDatabase &db)
{
    m_database = db;
}

/**
 * @brief LocalHandler::address
 * @return
 */
QString LocalHandler::address () const
{
    return m_addr;
}

/**
 * @brief LocalHandler::tdistrict
 * @return
 */
QString LocalHandler::district () const
{
    return m_district;
}


/**
 * @brief LocalHandler::city
 * @return
 */
QString LocalHandler::city () const
{
    return m_city;
}


/**
 * @brief LocalHandler::state
 * @return
 */
QString LocalHandler::state () const
{
    return m_state;
}

/**
 * @brief LocalHandler::complement
 * @return
 */
QString LocalHandler::complement () const
{
    return m_compl;
}

/**
 * @brief LocalHandler::setAddress
 * @param address
 */
void LocalHandler::setAddress(const QString &address)
{
    qDebug() << "addr " << address;
    m_addr = address;
}


/**
 * @brief LocalHandler::setDistrict
 * @param district
 */
void LocalHandler::setDistrict(const QString & district)
{
    qDebug() << "district " << district;
    m_district = district;
}

/**
 * @brief LocalHandler::setCity
 * @param city
 */
void LocalHandler::setCity(const QString &city)
{
    QString statLine = QString ("SELECT city_id FROM tb_citys WHERE city_name=\'%1\'").arg(city);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_city = query.value(0).toString();
    }

    qDebug() << "city " << m_city;
}

/**
 * @brief LocalHandler::setState
 * @param state
 */
void LocalHandler::setState(const QString &state)
{
    QString statLine = QString ("SELECT states_id FROM tb_states WHERE observation=\'%1\'").arg(state);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_state = query.value(0).toString();
    }

    qDebug() << "state " << m_state;
}

/**
 * @brief LocalHandler::setComplement
 * @param complement
 */
void LocalHandler::setComplement(const QString & complement)
{
    qDebug() << "complement " << complement;
    m_compl = complement;
}

/**
 * @brief LocalHandler::stateListAt
 * @param index
 * @return
 */
QString LocalHandler::stateListAt(int index)
{
    if (m_statesListCount < 0) {
        return QString ("");
    }
    return m_statesList.at(index);
}

/**
 * @brief LocalHandler::loadStateList
 */
void LocalHandler::loadStateList()
{
    QString statLine = QString ("SELECT * FROM tb_states");

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_statesListCount = query.size();

    int stateCol = query.record().indexOf("observation");

    m_statesList.clear();

    while (query.next()) {
        qDebug() << query.value(stateCol).toString();
        m_statesList.append(query.value(stateCol).toString());
    }
}

/**
 * @brief LocalHandler::statesCount
 * @return
 */
int LocalHandler::statesCount()
{
//    return localAddrHandler->getStateListCount();
    return m_statesListCount;
}

/**
 * @brief LocalHandler::getFirstStateId
 * @return
 */
int LocalHandler::getFirstStateId()
{
    QString statLine = QString ("SELECT MIN(states_id) FROM tb_states");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    int id = 0;
    while (query.next()) {
        bool result;
        id = query.value(0).toInt(&result);
    }

    return id;
}

/**
 * @brief LocalHandler::loadCityList
 */
void LocalHandler::loadCityList(int state_id)
{
    QString statLine = QString ("SELECT * FROM tb_citys WHERE states_id_tb_states=%1").arg(state_id);

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_cityListCount = query.size();

    int nameCol = query.record().indexOf("city_name");

    m_cityList.clear();

    while (query.next()) {
        qDebug() << query.value(nameCol).toString();
        m_cityList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief LocalHandler::cityListCount
 * @return
 */
int LocalHandler::cityListCount()
{
   return m_cityListCount;
}

/**
 * @brief LocalHandler::cityListAt
 * @param index
 * @return
 */
QString LocalHandler::cityListAt(int index)
{
    if (m_cityListCount <= 0) {
        return QString ("");
    }

    return m_cityList.at(index);
}

/**
 * @brief LocalHandler::stateId
 * @return
 */
QString LocalHandler::stateId() const
{
    return m_stateId;
}

/**
 * @brief LocalHandler::setStateId
 * @param stateId
 * @return
 */
bool LocalHandler::setStateId(const QString &stateId)
{
    QString statLine = QString ("SELECT states_id FROM tb_states WHERE observation=\'%1\'").arg(stateId);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        m_stateId = query.value(0).toString();
    }

    qDebug() << "m_stateId: " << m_stateId;
    return true;
}

/**
 * @brief LocalHandler::getStateId
 * @return
 */
int LocalHandler::getStateId()
{
    bool ok;
    return m_stateId.toInt(&ok);
}

/**
 * @brief LocalHandler::insertCity
 * @return
 */
QString LocalHandler::insertCity() const
{
    return m_insertCity;
}

/**
 * @brief LocalHandler::setInsertCity
 * @param insertCity
 */
bool LocalHandler::setInsertCity(QString insertCity)
{
    QString statLine = QString ("INSERT INTO tb_citys (city_name, states_id_tb_states) VALUES (\'%1\', %2)").arg(
                                insertCity, m_stateId);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief LocalHandler::insert
 * @param type
 * @param str
 * @return
 */
bool LocalHandler::insert(int type, QString str)
{

    switch (type) {

    case 1:
        {
            QString statLine = QString ("SELECT code_client FROM tb_clients WHERE name=\'%1\'").arg(str);

            QSqlQuery query = m_database.exec(statLine);
            //Number of columms

            QSqlError err = m_database.lastError();
            if (err.isValid()) {
                return false;
            }

            query.next();

            QString clientId = query.value(0).toString();

            statLine = QString ("INSERT INTO tb_address (address, addrs_comp, district, city, state, "
                                "code_client_tb_clients) VALUES (\'%1\', \'%2\', \'%3\', %4, %5, %6)").arg(
                                m_addr).arg(m_compl).arg(m_district).arg(m_city).arg(m_state).arg(clientId);

            qDebug() << statLine;

            m_database.exec(statLine);

            err = m_database.lastError();
            if (err.isValid()) {
                return false;
            }

            m_database.commit();
        }
        break;

    case 2:
        {
            QString statLine = QString ("INSERT INTO tb_address (address, addrs_comp, district, city, state, "
                                "crm_tb_doctors) VALUES (\'%1\', \'%2\', \'%3\', %4, %5, %6)").arg(
                                m_addr).arg(m_compl).arg(m_district).arg(m_city).arg(m_state).arg(str);

            qDebug() << statLine;

            m_database.exec(statLine);

            QSqlError err = m_database.lastError();
            if (err.isValid()) {
                return false;
            }

            m_database.commit();
        }
        break;

    case 3:
        {
            QString statLine = QString ("SELECT code_supplier FROM tb_suppliers WHERE social_name=\'%1\'").arg(str);

            QSqlQuery query = m_database.exec(statLine);
            //Number of columms

            QSqlError err = m_database.lastError();
            if (err.isValid()) {
                return false;
            }

            query.next();

            QString supplierId = query.value(0).toString();

            statLine = QString ("INSERT INTO tb_address (address, addrs_comp, district, city, state, "
                                "code_supplier_tb_suppliers) VALUES (\'%1\', \'%2\', \'%3\', %4, %5, %6)").arg(
                                m_addr).arg(m_compl).arg(m_district).arg(m_city).arg(m_state).arg(supplierId);

            qDebug() << statLine;

            m_database.exec(statLine);

            err = m_database.lastError();
            if (err.isValid()) {
                return false;
            }

            m_database.commit();
        }
    }

    return true;
}
