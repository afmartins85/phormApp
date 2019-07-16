#include "suppliershandler.h"

SuppliersHandler::SuppliersHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("Supplier handler Contructor");

//    localAddrHandler = new LocalAddressDef;
}
SuppliersHandler::~SuppliersHandler()
{
    qDebug("Supplier handler Destructor");
}

/**
 * @brief SuppliersHandler::setDatabase
 * @param db
 */
void SuppliersHandler::setDatabase(const QSqlDatabase &db)
{
    m_database = db;
}

/**
 * @brief SuppliersHandler::checkDependence
 * @return
 */
bool SuppliersHandler::checkDependence()
{
    return true;
}

/**
 * @brief SuppliersHandler::type
 * @return
 */
QString SuppliersHandler::type() const
{
    return m_type;
}

/**
 * @brief SuppliersHandler::socialName
 * @return
 */
QString SuppliersHandler::socialName() const
{
    return m_socialName;
}

/**
 * @brief SuppliersHandler::cnpj
 * @return
 */
QString SuppliersHandler::cnpj() const
{
    return m_cnpj;
}

/**
 * @brief SuppliersHandler::stateRegistration
 * @return
 */
QString SuppliersHandler::stateRegistration() const
{
    return m_stateRegistration;
}

/**
 * @brief SuppliersHandler::phone
 * @return
 */
QString SuppliersHandler::phone() const
{
    QString str;

    str.sprintf("%lld", m_phone);
    return str;
}

/**
 * @brief SuppliersHandler::fax
 * @return
 */
QString SuppliersHandler::fax() const
{
    QString str;

    str.sprintf("%lld", m_fax);
    return str;
}

/**
 * @brief SuppliersHandler::contactName
 * @return
 */
QString SuppliersHandler::contactName() const
{
    return m_contactName;
}

/**
 * @brief SuppliersHandler::status
 * @return
 */
QString SuppliersHandler::status() const
{
    return m_status;
}

/**
 * @brief SuppliersHandler::codeBank
 * @return
 */
QString SuppliersHandler::codeBank() const
{
    return m_codeBank;
}

/**
 * @brief SuppliersHandler::agencia
 * @return
 */
QString SuppliersHandler::agencia() const
{
    return m_agencia;
}

/**
 * @brief SuppliersHandler::cc
 * @return
 */
QString SuppliersHandler::cc() const
{
    return m_cc;
}

/**
 * @brief SuppliersHandler::setType
 * @param type
 */
void SuppliersHandler::setType(QString type)
{
    qDebug() << "type: " << type;

    int res = QString::compare(type, "FORNECEDOR", Qt::CaseInsensitive);
    if (!res) {
        m_type = "F";
    } else {
        m_type = "S";
    }
}

/**
 * @brief SuppliersHandler::setSocialName
 * @param socialName
 */
void SuppliersHandler::setSocialName(QString socialName)
{
    qDebug() << "Razão social: " << socialName;
    m_socialName = socialName;
}

/**
 * @brief SuppliersHandler::setCnpj
 * @param cnpj
 */
void SuppliersHandler::setCnpj(QString cnpj)
{
    qDebug() << "cnpj: " << cnpj;
    m_cnpj = cnpj;
}

/**
 * @brief SuppliersHandler::setStateRegistration
 * @param stateRegistration
 */
void SuppliersHandler::setStateRegistration(QString stateRegistration)
{
    qDebug() << "Inscrição estadual: " << stateRegistration;
    m_stateRegistration = stateRegistration;
}

/**
 * @brief SuppliersHandler::setPhone
 * @param phone
 */
void SuppliersHandler::setPhone(QString phone)
{
    bool ok;
    QString temp, temp2, temp3;

    phone.replace("(", ",");
    phone.replace(")", ",");
    phone.replace(" ", "\x0");

    temp2 = phone.section(',', -1, -1);
    temp = phone.section(",", 1, 1);
    temp3 = temp;
    temp3.append(temp2);

    m_phone = temp3.toULongLong(&ok);

    qDebug() << "phone: " << m_phone;
}

/**
 * @brief SuppliersHandler::setFax
 * @param fax
 */
void SuppliersHandler::setFax(QString fax)
{
    bool ok;
    QString temp, temp2, temp3;

    fax.replace("(", ",");
    fax.replace(")", ",");
    fax.replace(" ", "\x0");

    temp2 = fax.section(',', -1, -1);
    temp = fax.section(",", 1, 1);
    temp3 = temp;
    temp3.append(temp2);

    m_fax = temp3.toULongLong(&ok);

    qDebug() << "fax: " << m_fax;
}

/**
 * @brief SuppliersHandler::setContactName
 * @param contactName
 */
void SuppliersHandler::setContactName(QString contactName)
{
    qDebug() << "Representante: " << contactName;
    m_contactName = contactName;
}

/**
 * @brief SuppliersHandler::setStatus
 * @param status
 */
void SuppliersHandler::setStatus(QString status)
{
    qDebug() << "Status: " << "A";
    status = "A";
    m_status = status;
}

/**
 * @brief SuppliersHandler::setcodeBank
 * @param codeBank
 */
void SuppliersHandler::setcodeBank(QString codeBank)
{
    qDebug() << "Cód. Banco: " << codeBank;
    m_codeBank = codeBank;
}

/**
 * @brief SuppliersHandler::setAgencia
 * @param agencia
 */
void SuppliersHandler::setAgencia(QString agencia)
{
    qDebug() << "Agencia: " << agencia;
    m_agencia = agencia;
}

/**
 * @brief SuppliersHandler::setCc
 * @param cc
 */
void SuppliersHandler::setCc(QString cc)
{
    qDebug() << "Conta Corrente: " << cc;
    m_cc = cc;
}

/**
 * @brief SuppliersHandler::insert
 * @return
 */
bool SuppliersHandler::insert()
{
    bool ok;

    QString statLine = QString ("INSERT INTO tb_suppliers (type, social_name, cnpj, inscr_state, "
                                "phone, fax, contact_name, status) VALUES"
                                " (\'%1\', \'%2\', \'%3\', \'%4\', %5, %6, \'%7\', \'%8\')")
            .arg(m_type)
            .arg(m_socialName)
            .arg(m_cnpj)
            .arg(m_stateRegistration)
            .arg(m_phone)
            .arg(m_fax)
            .arg(m_contactName)
            .arg("A");

    qDebug() << statLine;

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();

    if (err.isValid()) {
        return false;
    }

    int supply_id;

    statLine = QString ("SELECT code_supplier FROM tb_suppliers WHERE social_name=\'%1\'").arg(m_socialName);

    QSqlQuery query = m_database.exec(statLine);

    err = m_database.lastError();
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    query.next();
    supply_id = query.value(0).toInt(&ok);

    statLine = QString ("INSERT INTO tb_bank (code, ag, cc, code_supplier_tb_suppliers) VALUES "
                                    " (%1, %2, %3, %4)").arg(m_codeBank).arg(m_agencia).arg(m_cc).arg(supply_id);

    qDebug() << statLine;

    m_database.exec(statLine);

    err = m_database.lastError();

    qDebug() << err.text();

    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief SuppliersHandler::supplierLoadList
 */
void SuppliersHandler::supplierLoadList()
{
    QString statLine = QString ("SELECT * FROM tb_suppliers");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_supplierListCount = query.size();

    int nameCol = query.record().indexOf("social_name");
    int cnpjCol = query.record().indexOf("cnpj");
    int phoneCol = query.record().indexOf("phone");

    m_supplierList.clear();
    m_cnpjList.clear();
    m_phoneList.clear();

    while (query.next()) {
        m_supplierList.append(query.value(nameCol).toString());
        m_cnpjList.append(query.value(cnpjCol).toString());
        m_phoneList.append(query.value(phoneCol).toString());
    }
}

/**
 * @brief SuppliersHandler::supplierListCount
 * @return
 */
int SuppliersHandler::supplierListCount()
{
    return m_supplierListCount;
}

/**
 * @brief SuppliersHandler::supplyListAt
 * @param index
 * @return
 */
QString SuppliersHandler::supplyListAt(int index)
{
    if (m_supplierListCount < 0) {
        return QString ("");
    }
    return m_supplierList.at(index);
}

/**
 * @brief SuppliersHandler::cnpjListAt
 * @param index
 * @return
 */
QString SuppliersHandler::cnpjListAt(int index)
{
    if (m_supplierListCount < 0) {
        return QString ("");
    }
    return m_cnpjList.at(index);
}

/**
 * @brief SuppliersHandler::phoneListAt
 * @param index
 * @return
 */
QString SuppliersHandler::phoneListAt(int index)
{
    if (m_supplierListCount < 0) {
        return QString ("");
    }
    return m_phoneList.at(index);
}

/**
 * @brief SuppliersHandler::remove
 * @param client
 * @return
 */
bool SuppliersHandler::remove(QString supply)
{
    QString statLine = QString ("SELECT code_supplier FROM tb_suppliers WHERE social_name=\'%1\'").arg(supply);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    query.next();

    QString supplier_Id = query.value(0).toString();

    statLine = QString ("DELETE FROM tb_address WHERE code_supplier_tb_suppliers=%1").arg(supplier_Id);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("DELETE FROM tb_bank WHERE code_supplier_tb_suppliers=%1").arg(supplier_Id);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("DELETE FROM tb_suppliers WHERE social_name=\'%1\'").arg(supply);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    return true;
}
