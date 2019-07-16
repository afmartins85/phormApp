#include "doctorhandler.h"

DoctorHandler::DoctorHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("Doctor handler Contructor");
}
DoctorHandler::~DoctorHandler()
{
    qDebug("Doctor handler Destructor");
}

/**
 * @brief DoctorHandler::setDatabase
 * @param db
 */
void DoctorHandler::setDatabase(const QSqlDatabase &db)
{
   m_database = db;
}

/**
 * @brief DoctorHandler::name
 * @return
 */
QString DoctorHandler::name() const
{
    return m_name;
}

/**
 * @brief DoctorHandler::setName
 * @param name
 */
void DoctorHandler::setName(QString name)
{
    qDebug() << "name: " << name;
    m_name = name;
}

/**
 * @brief DoctorHandler::crm
 * @return
 */
QString DoctorHandler::crm() const
{
    return m_crm;
}

/**
 * @brief DoctorHandler::setCrm
 * @param crm
 */
void DoctorHandler::setCrm(QString crm)
{
    qDebug() << "crm: " << crm;
    m_crm = crm;
}

/**
 * @brief DoctorHandler::phone
 * @return
 */
QString DoctorHandler::phone() const
{
    QString str;

    str.sprintf("%lld", m_phone);
    return str;
}

/**
 * @brief DoctorHandler::fax
 * @return
 */
QString DoctorHandler::fax() const
{
    QString str;

    str.sprintf("%lld", m_fax);
    return str;
}

/**
 * @brief DoctorHandler::speciality
 * @return
 */
QString DoctorHandler::speciality() const
{
    return m_speciality;
}

/**
 * @brief DoctorHandler::setPhone
 * @param phone
 */
void DoctorHandler::setPhone(QString phone)
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
 * @brief DoctorHandler::setFax
 * @param fax
 */
void DoctorHandler::setFax(QString fax)
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
 * @brief DoctorHandler::setSpeciality
 * @param speciality
 */
void DoctorHandler::setSpeciality(QString speciality)
{
    qDebug() << "speciality: " << speciality;
    m_speciality = speciality;
}

/**
 * @brief DoctorHandler::checkDependence
 * @return
 */
bool DoctorHandler::checkDependence()
{
    return true;
}

/**
 * @brief DoctorHandler::insert
 * @return
 */
bool DoctorHandler::insert()
{
//    QString statLine = QString ("INSERT INTO tb_doctors (crm, name, phone, fax, spec_id_tb_specialty) VALUES"
//                                " (%1, \'%2\', %3, %4, %5)").arg(m_crm).arg(m_name).arg(m_phone).arg(
//                                m_fax).arg("1");

    QString statLine = QString ("INSERT INTO tb_doctors (crm, name, phone, fax) VALUES"
                                " (%1, \'%2\', %3, %4)").arg(m_crm).arg(m_name).arg(m_phone).arg(m_fax);

    qDebug() << statLine;

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();

    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

void DoctorHandler::loadDoctorList()
{
    //####  Requisita todos os medicos presentes no banco
    QString statLine = QString ("SELECT * FROM tb_doctors");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_doctorListCount = query.size();

    int crmCol = query.record().indexOf("crm");
    int nameCol = query.record().indexOf("name");
    int phoneCol = query.record().indexOf("phone");

    m_doctorList.clear();
    m_crmList.clear();
    m_phoneList.clear();

    while (query.next()) {
        m_doctorList.append(query.value(nameCol).toString());
        m_crmList.append(query.value(crmCol).toString());
        m_phoneList.append(query.value(phoneCol).toString());
    }
}

/**
 * @brief DoctorHandler::doctorListCount
 * @return
 */
int DoctorHandler::doctorListCount()
{
    return m_doctorListCount;
}

/**
 * @brief DoctorHandler::doctorListAt
 * @param index
 * @return
 */
QString DoctorHandler::doctorListAt(int index)
{
    if (m_doctorListCount < 0) {
        return QString ("");
    }

    return m_doctorList.at(index);
}

/**
 * @brief DoctorHandler::crmListAt
 * @param index
 * @return
 */
QString DoctorHandler::crmListAt(int index)
{
    if (m_doctorListCount < 0) {
        return QString ("");
    }

    return m_crmList.at(index);
}

/**
 * @brief DoctorHandler::phoneListAt
 * @param index
 * @return
 */
QString DoctorHandler::phoneListAt(int index)
{
    if (m_doctorListCount < 0) {
        return QString ("");
    }

    return m_phoneList.at(index);
}

/**
 * @brief DoctorHandler::filterDoctorList
 * @param text
 */
void DoctorHandler::filterDoctorList(QString text)
{
    m_doctorCountSwap = 0;
    m_doctorListSwap.clear();
    m_crmListSwap.clear();
    m_phoneListSwap.clear();


    for (int i = 0; i < m_doctorListCount; i++) {
        if (!m_doctorList.at(i).lastIndexOf(text)) {

            m_doctorListSwap.append(m_doctorList.at(i));
            m_crmListSwap.append(m_crmList.at(i));
            m_phoneListSwap.append(m_phoneList.at(i));

            m_doctorCountSwap++;
        }
    }
}

/**
 * @brief DoctorHandler::doctorListCountSwap
 * @return
 */
int DoctorHandler::doctorListCountSwap()
{
    return m_doctorCountSwap;
}

/**
 * @brief DoctorHandler::doctorListSwapAt
 * @param index
 * @return
 */
QString DoctorHandler::doctorListSwapAt(int index)
{
    if (m_doctorCountSwap < 0) {
        return QString ("");
    }
    return m_doctorListSwap.at(index);
}

/**
 * @brief DoctorHandler::crmListSwapAt
 * @param index
 * @return
 */
QString DoctorHandler::crmListSwapAt(int index)
{
    if (m_doctorCountSwap < 0) {
        return QString ("");
    }
    return m_crmListSwap.at(index);
}

/**
 * @brief DoctorHandler::phoneListSwapAt
 * @param index
 * @return
 */
QString DoctorHandler::phoneListSwapAt(int index)
{
    if (m_doctorCountSwap < 0) {
        return QString ("");
    }

    return m_phoneListSwap.at(index);
}

/**
 * @brief DoctorHandler::remove
 * @param client
 * @return
 */
bool DoctorHandler::remove(QString crm)
{
    QString statLine = QString ("DELETE FROM tb_address WHERE crm_tb_doctors=%1").arg(crm);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("DELETE FROM tb_doctors WHERE crm=%1").arg(crm);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    return true;
}
