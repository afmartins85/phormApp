#include "maintenancehandler.h"
#include <QSqlError>
#include <QSqlResult>
#include <QDebug>

MaintenanceHandler::MaintenanceHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("MainHandler contructor 1");
}

MaintenanceHandler::~MaintenanceHandler()
{

}

/**
 * @brief MaintenanceHandler::setPhormInsert
 * @param phorm
 * @return
 */
bool MaintenanceHandler::setPhormInsert(const QString &phorm)
{
    QString statLine = QString ("INSERT INTO tb_pharma_phorm (phorm_name) VALUES (\'%1\')").arg(phorm);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::phormInsert
 * @return
 */
QString MaintenanceHandler::phormInsert() const
{
    return m_phormInsert;
}

/**
 * @brief MaintenanceHandler::setDatabase
 * @param db
 */
void MaintenanceHandler::setDatabase(QSqlDatabase const &db)
{
    m_database = db;
}

/**
 * @brief MaintenanceHandler::isOpenDB
 * @return
 */
bool MaintenanceHandler::isOpenDB()
{
    return m_database.isOpen();
}

/**
 * @brief MaintenanceHandler::phorms
 * @return
 */
QQmlListProperty<QString> MaintenanceHandler::phorms()
{
    return QQmlListProperty<QString>(this, 0, &MaintenanceHandler::append_phorms,0,0,0);
}

/**
 * @brief MaintenanceHandler::append_phorms
 * @param list
 * @param phorm
 */
void MaintenanceHandler::append_phorms(QQmlListProperty<QString> *list, QString *phorm)
{
    MaintenanceHandler *item = qobject_cast<MaintenanceHandler *>(list->object);
    if (phorm) {
        item->m_phorms.append(phorm);
    }
}

/**
 * @brief MaintenanceHandler::loadPhormList
 */
void MaintenanceHandler::loadPhormList()
{
    QString statLine = QString ("SELECT * FROM tb_pharma_phorm");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_phormListCount = query.size();

    int nameCol = query.record().indexOf("phorm_name");

    while (query.next()) {
        m_phormList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::phormListCount
 * @return
 */
int MaintenanceHandler::phormListCount()
{
    return m_phormListCount;
}

/**
 * @brief MaintenanceHandler::phormIndexOf
 * @param phorm
 * @return
 */
int MaintenanceHandler::phormIndexOf(QString phorm)
{
    return m_phormList.indexOf(phorm);
}

/**
 * @brief MaintenanceHandler::phormListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::phormListAt(int index)
{
    if (m_phormListCount < 0) {
        return QString ("");
    }

    return m_phormList.at(index);
}

/**
 * @brief MaintenanceHandler::phormListRemove
 * @param item
 * @return
 */
bool MaintenanceHandler::phormListRemove(const QString &phorm)
{
    QString statLine = QString ("DELETE FROM tb_pharma_phorm WHERE phorm_name=\'%1\'").arg(phorm);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::phormId
 * @return
 */
QString MaintenanceHandler::phormId() const
{
    return m_phormId;
}

/**
 * @brief MaintenanceHandler::setPhormId
 * @param phormName
 * @return
 */
bool MaintenanceHandler::setPhormId(const QString &phormName)
{
    QString statLine = QString ("SELECT phorm_id FROM tb_pharma_phorm WHERE phorm_name=\'%1\'").arg(phormName);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        m_phormId = query.value(0).toString();
    }

    qDebug() << "m_phormId: " << m_phormId;
    return true;
}

/**
 * @brief MaintenanceHandler::getPhormIdFromName
 * @param phorm
 * @return
 */
int MaintenanceHandler::getPhormIdFromName (QString const &phorm)
{
    int id;

    QString statLine = QString ("SELECT phorm_id FROM tb_pharma_phorm WHERE phorm_name=\'%1\'").arg(phorm);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return -1;
    }

    if (!query.size()) {
        return -2;
    }

    query.next();
    id = query.value(0).toInt();

    qDebug() << "m_phormId: " << id;
    return id;
}

/**
 * @brief MaintenanceHandler::getPhormId
 * @return
 */
int MaintenanceHandler::getPhormId()
{
    bool ok;
    return m_phormId.toInt(&ok);
}

/**
 * @brief MaintenanceHandler::setUnitInsert
 * @param unit
 * @return
 */
bool MaintenanceHandler::setUnitInsert(const QString &unit)
{
    QString statLine = QString ("INSERT INTO tb_units (unit) VALUES (\'%1\')").arg(unit);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::unitInsert
 * @return
 */
QString MaintenanceHandler::unitInsert() const
{
    return m_unitInsert;
}

/**
 * @brief MaintenanceHandler::loadUnitsList
 */
void MaintenanceHandler::loadUnitsList()
{
    QString statLine = QString ("SELECT * FROM tb_units");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_unitListCount = query.size();

    int nameCol = query.record().indexOf("unit");

    while (query.next()) {
        m_unitsList.append(query.value(nameCol).toString());
        qDebug() << "################ " << query.value(nameCol).toString();
    }
}

/**
 * @brief MaintenanceHandler::unitListCount
 * @return
 */
int MaintenanceHandler::unitListCount()
{
    return m_unitListCount;
}

/**
 * @brief MaintenanceHandler::unitsListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::unitsListAt(int index)
{
    if (m_unitListCount < 0) {
        return QString ("");
    }

    return m_unitsList.at(index);
}

/**
 * @brief MaintenanceHandler::unitIndexOf
 * @param unit
 * @return
 */
int MaintenanceHandler::unitIndexOf(QString unit)
{
    return m_unitsList.indexOf(unit);
}

/**
 * @brief MaintenanceHandler::unitsListRemove
 * @param unit
 * @return
 */
bool MaintenanceHandler::unitsListRemove(const QString &unit)
{
    QString statLine = QString ("DELETE FROM tb_units WHERE unit=\'%1\'").arg(unit);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::subGroup
 * @return
 */
QString  MaintenanceHandler::subGroup() const
{
    return m_subGroup;
}

/**
 * @brief MaintenanceHandler::setSubGroup
 * @param sub
 * @return
 */
bool  MaintenanceHandler::setSubGroup(QString const &sub)
{
    QString statLine = QString ("INSERT INTO tb_element_sub_group (sub_group_element, group_id_tb_element_group)"
                                " VALUES (\'%1\', %2)").arg(sub, m_groupId);

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::loadSubGroupList
 */
void MaintenanceHandler::loadSubGroupList(int group_id)
{
    QString statLine = QString ("SELECT * FROM tb_element_sub_group WHERE group_id_tb_element_group=%1").arg(group_id);

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_subGroupListCount = query.size();

    int nameCol = query.record().indexOf("sub_group_element");

    //Limpa a lista de sub-grupos para carregar os novos items
    m_subGroupList.clear();

    while (query.next()) {
        qDebug() << query.value(nameCol).toString();
        m_subGroupList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::subGroupListCount
 * @return
 */
int MaintenanceHandler::subGroupListCount()
{
    return m_subGroupListCount;
}

/**
 * @brief MaintenanceHandler::subGroupListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::subGroupListAt(int index)
{
    if (m_subGroupListCount < 0) {
        return QString ("");
    }
    return m_subGroupList.at(index);
}

/**
 * @brief MaintenanceHandler::subGroupIndexOf
 * @param sub
 * @return
 */
int MaintenanceHandler::subGroupIndexOf(QString sub)
{
    return m_subGroupList.indexOf(sub);
}

/**
 * @brief MaintenanceHandler::subGroupListRemove
 * @param sub
 * @return
 */
bool MaintenanceHandler::subGroupListRemove(const QString &sub)
{
    QString statLine = QString ("DELETE FROM tb_element_sub_group WHERE sub_group_element=\'%1\'").arg(sub);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::group
 * @return
 */
QString  MaintenanceHandler::group() const
{
    return m_group;
}

/**
 * @brief MaintenanceHandler::setGroup
 * @param group
 * @return
 */
bool  MaintenanceHandler::setGroup(QString const &group)
{
    QString statLine = QString ("INSERT INTO tb_element_group (group_name) VALUES (\'%1\')").arg(group);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::loadGroupList
 */
void MaintenanceHandler::loadGroupList()
{
    QString statLine = QString ("SELECT * FROM tb_element_group");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_groupListCount = query.size();

    int nameCol = query.record().indexOf("group_name");

    while (query.next()) {
        m_groupList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::groupListCount
 * @return
 */
int MaintenanceHandler::groupListCount()
{
    return m_groupListCount;
}

/**
 * @brief MaintenanceHandler::groupListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::groupListAt(int index)
{
    if (m_groupListCount < 0) {
        return QString ("");
    }
    return m_groupList.at(index);
}

/**
 * @brief MaintenanceHandler::groupIndexOf
 * @param group
 * @return
 */
int MaintenanceHandler::groupIndexOf(QString group)
{
    return m_groupList.indexOf(group);
}

/**
 * @brief MaintenanceHandler::groupListRemove
 * @param group
 * @return
 */
bool MaintenanceHandler::groupListRemove(const QString &group)
{
    QString statLine = QString ("DELETE FROM tb_element_group WHERE group_name=\'%1\'").arg(group);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::getGroupId
 * @param group
 * @return
 */
int MaintenanceHandler::getGroupId(void)
{
    bool ok;
    return m_groupId.toInt(&ok);
}

/**
 * @brief MaintenanceHandler::getFirstGroupId
 * @return
 */
int MaintenanceHandler::getFirstGroupId(void)
{
    QString statLine = QString ("SELECT MIN(group_id) FROM tb_element_group");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    int id = 0;
    while (query.next()) {
        bool result;
        id = query.value(0).toInt(&result);
    }

    qDebug() << "Min: " << id;
    return id;
}

/**
 * @brief MaintenanceHandler::setGroupId
 * @param id
 */
bool MaintenanceHandler::setGroupId(const QString &groupName)
{
    QString statLine = QString ("SELECT group_id FROM tb_element_group WHERE group_name=\'%1\'").arg(groupName);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        m_groupId = query.value(0).toString();
    }

    qDebug() << "setGroupId: " << m_groupId;
    return true;
}

/**
 * @brief MaintenanceHandler::groupId
 * @return
 */
QString MaintenanceHandler::groupId() const
{
    return m_groupId;
}

/**
 * @brief MaintenanceHandler::loadValidateList
 */
void MaintenanceHandler::loadValidateList()
{
    QString statLine = QString ("SELECT * FROM tb_validate");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_validateCount = query.size();

    int nameCol = query.record().indexOf("validate_desc");

    while (query.next()) {
        m_validateList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::validateCount
 * @return
 */
int MaintenanceHandler::validateCount()
{
    return m_validateCount;
}

/**
 * @brief MaintenanceHandler::validateListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::validateListAt(int index)
{
    if (m_validateCount < 0) {
        return QString ("");
    }
    return m_validateList.at(index);
}

/**
 * @brief MaintenanceHandler::validate
 * @return
 */
QString MaintenanceHandler::validate() const
{
    return m_validate;
}

/**
 * @brief MaintenanceHandler::setValidate
 * @param validate
 * @return
 */
bool MaintenanceHandler::setValidate(const QString &validate)
{
    QString statLine = QString ("INSERT INTO tb_validate (validate_desc) VALUES (\'%1\')").arg(validate);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::validateRemove
 * @param validate
 * @return
 */
bool MaintenanceHandler::validateRemove(const QString &validate)
{
    QString statLine = QString ("DELETE FROM tb_validate WHERE validate_desc=\'%1\'").arg(validate);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::loadExcipientList
 * @param phorm_id
 */
void MaintenanceHandler::loadExcipientList(int phorm_id)
{
    QString statLine = QString ("SELECT * FROM tb_excipient WHERE phorm_id_tb_pharma_phorm=%1").arg(phorm_id);

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_excipientCount = query.size();

    int nameCol = query.record().indexOf("element_desc");

    //Limpa a lista de excipient para carregar os novos items
    m_excipientList.clear();

    while (query.next()) {
        qDebug() << query.value(nameCol).toString();
        m_excipientList.append(query.value(nameCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::excipientCount
 * @return
 */
int MaintenanceHandler::excipientCount()
{
    return m_excipientCount;
}

/**
 * @brief MaintenanceHandler::excipientListAt
 * @param index
 */
QString MaintenanceHandler::excipientListAt(int index)
{
    if (excipientCount() < 0) {
        return QString ("");
    }
    return m_excipientList.at(index);
}

/**
 * @brief MaintenanceHandler::excipientIndexOf
 * @param excip
 * @return
 */
int MaintenanceHandler::excipientIndexOf(QString excip)
{
    qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << m_excipientList.indexOf(excip);
    qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << excip;
    return m_excipientList.indexOf(excip);
}

/**
 * @brief MaintenanceHandler::getFirstPhormId
 * @return
 */
int MaintenanceHandler::getFirstPhormId()
{
    QString statLine = QString ("SELECT MIN(phorm_id) FROM tb_pharma_phorm");

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    int id = 0;
    while (query.next()) {
        bool result;
        id = query.value(0).toInt(&result);
    }

    qDebug() << "Min: " << id;
    return id;
}

/**
 * @brief MaintenanceHandler::excipient
 * @return
 */
QString MaintenanceHandler::excipient() const
{
    return m_excipient;
}

/**
 * @brief MaintenanceHandler::setExcipient
 * @param excipient
 * @return
 */
bool MaintenanceHandler::setExcipient(const QString &excipient)
{
    QString statLine = QString ("INSERT INTO tb_excipient (element_desc, phorm_id_tb_pharma_phorm)"
                                " VALUES (\'%1\', %2)").arg(excipient, m_phormId);

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::excipientRemove
 * @param excipient
 * @return
 */
bool MaintenanceHandler::excipientRemove(const QString &excipient)
{
    QString statLine = QString ("DELETE FROM tb_excipient WHERE element_desc=\'%1\'").arg(excipient);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::loadSpecificationCapsList
 */
void MaintenanceHandler::loadSpecificationCapsList(QString const &phorm)
{
    QString statLine = QString ("SELECT * FROM tb_spec_caps WHERE phorm_id_tb_pharma_phorm=%1")
            .arg(getPhormIdFromName(phorm));

    qDebug() << statLine;

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_specCapsCount = query.size();

    qDebug() << m_specCapsCount;

    int colName = query.record().indexOf("spec_name");
    int colCapacity = query.record().indexOf("capacity");
    int colVolume = query.record().indexOf("volume");

    m_specCapsList.clear();
    m_specCapsCapacityList.clear();
    m_specCapsVolumeList.clear();

    while (query.next()) {
            m_specCapsList.append(query.value(colName).toString());
            m_specCapsCapacityList.append(query.value(colCapacity).toString());
            m_specCapsVolumeList.append(query.value(colVolume).toString());

            qDebug() << query.value(colName).toString();
            qDebug() << query.value(colCapacity).toString();
            qDebug() << query.value(colVolume).toString();
    }
}

/**
 * @brief MaintenanceHandler::loadAllSpecificationCapsList
 */
void MaintenanceHandler::loadAllSpecificationCapsList(void)
{
    QString statLine = QString ("SELECT * FROM tb_spec_caps");

    qDebug() << statLine;

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_specCapsCount = query.size();

    qDebug() << m_specCapsCount;

    int colName = query.record().indexOf("spec_name");
    int colCapacity = query.record().indexOf("capacity");
    int colVolume = query.record().indexOf("volume");

    m_specCapsList.clear();
    m_specCapsCapacityList.clear();
    m_specCapsVolumeList.clear();

    while (query.next()) {
            m_specCapsList.append(query.value(colName).toString());
            m_specCapsCapacityList.append(query.value(colCapacity).toString());
            m_specCapsVolumeList.append(query.value(colVolume).toString());

            qDebug() << query.value(colName).toString();
            qDebug() << query.value(colCapacity).toString();
            qDebug() << query.value(colVolume).toString();
    }
}

/**
 * @brief MaintenanceHandler::specificationCapsCount
 * @return
 */
int MaintenanceHandler::specificationCapsCount()
{
    return m_specCapsCount;
}

/**
 * @brief MaintenanceHandler::specCapsName
 * @return
 */
QString MaintenanceHandler::specCapsName() const
{
    return m_specCapsName;
}

/**
 * @brief MaintenanceHandler::capsCapacity
 * @return
 */
QString MaintenanceHandler::capsCapacity() const
{
    return m_capsCapacity;
}

/**
 * @brief MaintenanceHandler::capsVolume
 * @return
 */
QString MaintenanceHandler::capsVolume() const
{
    return m_capsVolume;
}

/**
 * @brief MaintenanceHandler::setCapsCapacity
 * @param capsCapacity
 */
void MaintenanceHandler::setCapsCapacity(QString capsCapacity)
{
    capsCapacity.replace(",", ".");
    m_capsCapacity = capsCapacity;
}

/**
 * @brief MaintenanceHandler::setCapsVolume
 * @param capsVolume
 */
void MaintenanceHandler::setCapsVolume(QString capsVolume)
{
    capsVolume.replace(",", ".");
    m_capsVolume = capsVolume.toFloat();
}

/**
 * @brief MaintenanceHandler::setSpecCapsName
 * @param specCapsName
 */
bool MaintenanceHandler::setSpecCapsName(QString specCapsName)
{
    m_specCapsName = specCapsName;

    QString statLine = QString ("INSERT INTO tb_spec_caps (spec_name, capacity, volume, phorm_id_tb_pharma_phorm)"
                                " VALUES (\'%1\', %2, %3, %4)").arg(m_specCapsName).arg(m_capsCapacity).arg(
                                                                    m_capsVolume).arg(getPhormId());

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::specCapsListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::specCapsListAt(int index)
{
    if (m_specCapsCount < 0) {
        return QString ("");
    }
    return m_specCapsList.at(index);
}

/**
 * @brief MaintenanceHandler::specCapacityListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::specCapacityListAt(int index)
{
    if (m_specCapsCount < 0) {
        return QString ("");
    }
    return m_specCapsCapacityList.at(index);
}

/**
 * @brief MaintenanceHandler::specVolumeListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::specVolumeListAt(int index)
{
    if (m_specCapsCount < 0) {
        return QString ("");
    }
    return m_specCapsVolumeList.at(index);
}

bool MaintenanceHandler::specCapsRemove(const QString &specCaps)
{
    QString statLine = QString ("DELETE FROM tb_spec_caps WHERE spec_name=\'%1\'").arg(specCaps);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::packageName
 * @return
 */
QString MaintenanceHandler::packageName() const
{
    return m_packageName;
}

/**
 * @brief MaintenanceHandler::VolumePack
 * @return
 */
QString MaintenanceHandler::VolumePack() const
{
    QString str;

    str.sprintf("%f", m_VolumePack);
    return str;
}

/**
 * @brief MaintenanceHandler::labelSize
 * @return
 */
int MaintenanceHandler::labelSize() const
{
    return m_labelSize;
}

/**
 * @brief MaintenanceHandler::seVolumePack
 * @param VolumePack
 */
void MaintenanceHandler::seVolumePack(QString VolumePack)
{
    bool ok;
    VolumePack.replace(",", ".");
    m_VolumePack = VolumePack.toFloat(&ok);
    qDebug() << "Volume: " << m_VolumePack << " ml";
}

/**
 * @brief MaintenanceHandler::setLabelSize
 * @param labelSize
 */
void MaintenanceHandler::setLabelSize(int labelSize)
{
    m_labelSize = labelSize;
}

/**
 * @brief MaintenanceHandler::setPackageName
 * @param packageName
 * @return
 */
bool MaintenanceHandler::setPackageName(QString packageName)
{
    m_packageName = packageName;

    int phormId = getPhormId();

    QString statLine = QString ("INSERT INTO tb_packing "
                                "(packing_name, volume, phorm_id_tb_pharma_phorm, label_size)"
                                " VALUES (\'%1\', %2, %3, %4)")
                                .arg(m_packageName)
                                .arg(m_labelSize).arg(phormId)
                                .arg(m_VolumePack);

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::loadPackList
 * @param phorm_id
 */
void MaintenanceHandler::loadPackList(int phorm_id)
{
    bool ok;
    QString statLine = QString ("SELECT * FROM tb_packing WHERE phorm_id_tb_pharma_phorm=%1").arg(phorm_id);

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_packListCount = query.size();

    int namePackCol = query.record().indexOf("packing_name");
    int volumePackCol = query.record().indexOf("volume");
    int labelSizeCol = query.record().indexOf("label_size");

    //Limpa a lista de sub-grupos para carregar os novos items
    m_packingList.clear();
    m_volumePackList.clear();
    m_labelSizeList.clear();

    while (query.next()) {
        m_packingList.append(query.value(namePackCol).toString());
        m_volumePackList.append(query.value(volumePackCol).toString());
        switch (query.value(labelSizeCol).toInt(&ok)) {
            case 0:
                m_labelSizeList.append("PEQ.");
            break;

            case 1:
                m_labelSizeList.append("MED.");
            break;

            case 2:
                m_labelSizeList.append("GR.");
            break;
        }
    }
}

/**
 * @brief MaintenanceHandler::packCount
 * @return
 */
int MaintenanceHandler::packCount()
{
    return m_packListCount;
}

/**
 * @brief MaintenanceHandler::packListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::packListAt(int index)
{
    if (m_packListCount < 0) {
        return QString ("");
    }
    return m_packingList.at(index);
}

/**
 * @brief MaintenanceHandler::volumePackListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::volumePackListAt(int index)
{
    if (m_packListCount < 0) {
        return QString ("");
    }
    return m_volumePackList.at(index);
}

/**
 * @brief MaintenanceHandler::labelSizeListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::labelSizeListAt(int index)
{
    if (m_packListCount < 0) {
        return QString ("");
    }
    return m_labelSizeList.at(index);
}

/**
 * @brief MaintenanceHandler::packRemove
 * @param pack
 * @return
 */
bool MaintenanceHandler::packRemove(const QString &pack)
{
    QString statLine = QString ("DELETE FROM tb_packing WHERE packing_name=\'%1\'").arg(pack);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::packId
 * @return
 */
QString MaintenanceHandler::packId() const
{
    return m_packId;
}

/**
 * @brief MaintenanceHandler::setPackId
 * @param packName
 * @return
 */
bool MaintenanceHandler::setPackId(const QString &packName)
{
    QString statLine = QString ("SELECT packing_id FROM tb_packing WHERE packing_name=\'%1\'").arg(packName);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        m_packId = query.value(0).toString();
    }

    return true;
}

/**
 * @brief MaintenanceHandler::loadAllPackList
 */
void MaintenanceHandler::loadAllCapsList()
{
    int phorm_id = 0;
    bool ok;

    QString statLine = QString ("SELECT phorm_id FROM tb_pharma_phorm WHERE phorm_name=\'CÁPSULAS\'");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    while (query.next()) {
        phorm_id = query.value(0).toInt(&ok);
    }

    statLine = QString ("SELECT * FROM tb_packing WHERE phorm_id_tb_pharma_phorm=%1").arg(phorm_id);

    query = m_database.exec(statLine);
    //Number of columms
    m_capsListCount = query.size();

    int namePackCol = query.record().indexOf("packing_name");

    //Limpa a lista de Embalagens para carregar os novos items
    m_packingList.clear();

    while (query.next()) {
        m_packingList.append(query.value(namePackCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::capsCount
 * @return
 */
int MaintenanceHandler::capsCount()
{
    return m_capsListCount;
}

/**
 * @brief MaintenanceHandler::capsForPack
 * @return
 */
QString MaintenanceHandler::capsForPack() const
{
    return m_capsForPack;
}

/**
 * @brief MaintenanceHandler::qtdCaps
 * @return
 */
int MaintenanceHandler::qtdCaps() const
{
    return m_qtdCaps;
}

/**
 * @brief MaintenanceHandler::setCapsForPack
 * @param capsForPack
 */
bool MaintenanceHandler::setCapsForPack(QString capsForPack)
{
    m_capsForPack = capsForPack;

    qDebug() << "m_packId: " << m_packId;
    qDebug() << "Quantidade: " << m_qtdCaps;
    qDebug() << "Capsula: " << m_capsForPack;

    QString statLine = QString ("INSERT INTO tb_caps_for_packing VALUES (\'%1\',"
                                " %2, %3)").arg(m_capsForPack).arg(m_qtdCaps).arg(m_packId);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::setQtdCaps
 * @param qtdCaps
 */
void MaintenanceHandler::setQtdCaps(int qtdCaps)
{
    m_qtdCaps = qtdCaps;
}

/**
 * @brief MaintenanceHandler::loadCapsForPackList
 * @param index
 */
void MaintenanceHandler::loadCapsForPackList(int index)
{
    QString statLine = QString ("SELECT * FROM tb_caps_for_packing WHERE packing_id_tb_packing=%1").arg(index);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_capsListCount = query.size();

    int nameCapsCol = query.record().indexOf("caps_name");
    int capacityCol = query.record().indexOf("max_capacity");

    //Limpa as listas
    m_capsForPackList.clear();
    m_capacityList.clear();

    while (query.next()) {
        m_capsForPackList.append(query.value(nameCapsCol).toString());
        m_capacityList.append(query.value(capacityCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::capsForPackCount
 * @return
 */
int MaintenanceHandler::capsForPackCount()
{
    return m_capsListCount;
}

/**
 * @brief MaintenanceHandler::capsForPackListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::capsForPackListAt(int index)
{
    if (m_capsListCount < 0) {
        return QString ("");
    }
    return m_capsForPackList.at(index);
}

/**
 * @brief MaintenanceHandler::capacityListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::capacityListAt(int index)
{
    if (m_capsListCount < 0) {
        return QString ("");
    }
    return m_capacityList.at(index);
}

/**
 * @brief MaintenanceHandler::getFirstCapsForPackId
 * @return
 */
int MaintenanceHandler::getFirstCapsForPackId()
{
    QString statLine = QString ("SELECT MIN(packing_id_tb_packing) FROM tb_caps_for_packing");

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
 * @brief MaintenanceHandler::getPackId
 * @return
 */
int MaintenanceHandler::getPackId()
{
    bool ok;
    return m_packId.toInt(&ok);
}

/**
 * @brief MaintenanceHandler::capsForPackListRemove
 * @param cap
 * @return
 */
bool MaintenanceHandler::capsForPackListRemove(const QString &cap)
{
    QString statLine = QString ("DELETE FROM tb_caps_for_packing WHERE caps_name=\'%1\'").arg(cap);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::loadPriceUpList
 * @param index
 */
void MaintenanceHandler::loadPriceUpList(int phorm_id)
{
    QString statLine = QString ("SELECT * FROM tb_composition_price WHERE phorm_id_tb_pharma_phorm=%1").arg(phorm_id);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_priceUpCount = query.size();

    int namePackCol = query.record().indexOf("comp_desc");

    //Limpa a lista de Preços para carregar os novos items
    m_priceUpList.clear();

    while (query.next()) {
        m_priceUpList.append(query.value(namePackCol).toString());
    }
}

/**
 * @brief MaintenanceHandler::priceUp
 * @return
 */
QString MaintenanceHandler::priceUp() const
{
    return m_priceUp;
}

/**
 * @brief MaintenanceHandler::setPriceUp
 * @param priceUp
 */
bool MaintenanceHandler::setPriceUp(QString const &priceUp)
{
    QString statLine = QString ("INSERT INTO tb_composition_price (comp_desc, phorm_id_tb_pharma_phorm)"
                                " VALUES (\'%1\', %2)").arg(priceUp, m_phormId);

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief MaintenanceHandler::priceUpCount
 * @return
 */
int MaintenanceHandler::priceUpCount()
{
    return m_priceUpCount;
}

/**
 * @brief MaintenanceHandler::priceUpListAt
 * @param index
 * @return
 */
QString MaintenanceHandler::priceUpListAt(int index)
{
    if (m_priceUpCount < 0) {
        return QString ("");
    }
    return m_priceUpList.at(index);
}

/**
 * @brief MaintenanceHandler::priceUpRemove
 * @param price
 * @return
 */
bool MaintenanceHandler::priceUpRemove(const QString &price)
{
    QString statLine = QString ("DELETE FROM tb_composition_price WHERE comp_desc=\'%1\'").arg(price);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::priceItemInit
 * @return
 */
QString MaintenanceHandler::priceItemInit() const
{
    QString str;

    str.sprintf("%d", m_priceItemInit);
    return str;
}

/**
 * @brief MaintenanceHandler::priceItemEnd
 * @return
 */
QString MaintenanceHandler::priceItemEnd() const
{
    QString str;

    str.sprintf("%d", m_priceItemEnd);
    return str;
}

/**
 * @brief MaintenanceHandler::costManipulation
 * @return
 */
QString MaintenanceHandler::costManipulation() const
{
    QString str;

    str.sprintf("%.2f", m_costManipulation);
    return str;
}

/**
 * @brief MaintenanceHandler::profitFactor
 * @return
 */
QString MaintenanceHandler::profitFactor() const
{
    QString str;

    str.sprintf("%.2f", m_profitFactor);
    return str;
}

/**
 * @brief MaintenanceHandler::minPrice
 * @return
 */
QString MaintenanceHandler::minPrice() const
{
    QString str;

    str.sprintf("%.2f", m_minPrice);
    return str;
}

/**
 * @brief MaintenanceHandler::setPriceItemInit
 * @param priceItemInit
 */
void MaintenanceHandler::setPriceItemInit(QString priceItemInit)
{
    bool ok;
    qDebug() << "Itens de: " << priceItemInit;
    m_priceItemInit = priceItemInit.toInt(&ok);
}

/**
 * @brief MaintenanceHandler::setPriceItemEnd
 * @param priceItemEnd
 */
void MaintenanceHandler::setPriceItemEnd(QString priceItemEnd)
{
    bool ok;
    qDebug() << "Itens até: " << priceItemEnd;
    m_priceItemEnd = priceItemEnd.toInt(&ok);
}

/**
 * @brief MaintenanceHandler::setCostManipulation
 * @param costManipulation
 */
void MaintenanceHandler::setCostManipulation(QString costManipulation)
{
    bool ok;
    costManipulation.replace(",", ".");
    m_costManipulation = costManipulation.toFloat(&ok);
    qDebug() << "Custo de manipulação: " << m_costManipulation;
}

/**
 * @brief MaintenanceHandler::setProfitFactor
 * @param profitFactor
 */
void MaintenanceHandler::setProfitFactor(QString profitFactor)
{
    bool ok;
    profitFactor.replace(",", ".");
    m_profitFactor = profitFactor.toFloat(&ok);
    qDebug() << "Fator lucro : " << m_profitFactor;
}

/**
 * @brief MaintenanceHandler::setMinPrice
 * @param minPrice
 */
void MaintenanceHandler::setMinPrice(QString minPrice)
{
    bool ok;
    minPrice.replace(",", ".");
    m_minPrice = minPrice.toFloat(&ok);
    qDebug() << "Preço minimo: " << m_minPrice;
}

/**
 * @brief MaintenanceHandler::priceUPInsert
 * @return
 */
QString MaintenanceHandler::priceUPInsert() const
{
    return m_priceUPInsert;
}

/**
 * @brief MaintenanceHandler::setPriceUPInsert
 * @param priceUPInsert
 * @return
 */
bool MaintenanceHandler::setPriceUPInsert(QString priceUPInsert)
{
    m_priceUPInsert = priceUPInsert;

    QString statLine = QString ("UPDATE tb_composition_price SET num_item_init=%1, "
                                "num_item_end=%2, cost_manipulation=%3, profit_factor=%4, "
                                "min_price=%5 WHERE comp_desc=\'%6\'").arg(m_priceItemInit).arg(
                                m_priceItemEnd).arg(m_costManipulation).arg(m_profitFactor).arg(
                                m_minPrice).arg(priceUPInsert);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::packPriceUPInsert
 * @return
 */
QString MaintenanceHandler::packPriceUPInsert() const
{
    return m_packPriceUPInsert;
}

/**
 * @brief MaintenanceHandler::setPackPriceUPInsert
 * @param priceUPInsert
 * @return
 */
bool MaintenanceHandler::setPackPriceUPInsert(QString packPriceUPInsert)
{
    m_packPriceUPInsert = packPriceUPInsert;

    QString statLine = QString ("UPDATE tb_packing SET cost_factor=%1, "
                                "cost=%2 WHERE packing_name=\'%3\'")
                            .arg(m_packFactor)
                            .arg(m_CostPack)
                            .arg(packPriceUPInsert);

    qDebug() << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();
    return true;
}

/**
 * @brief MaintenanceHandler::getPriceUpData
 * @param item
 * @return
 */
bool MaintenanceHandler::getPriceUpData (QString const &item)
{
    bool ok;
    m_costManipulation = 0;

    QString statLine = QString ("SELECT * FROM tb_composition_price WHERE comp_desc=\'%1\'").arg(item);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    int numInitCol = query.record().indexOf("num_item_init");
    int numEndCol = query.record().indexOf("num_item_end");
    int costCol = query.record().indexOf("cost_manipulation");
    int factorCol = query.record().indexOf("profit_factor");
    int minCol = query.record().indexOf("min_price");

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        qDebug("Nenhum item contrado");
        return false;
    }

    while (query.next()) {
        m_priceItemInit = query.value(numInitCol).toInt(&ok);
        m_priceItemEnd = query.value(numEndCol).toInt(&ok);
        m_costManipulation = query.value(costCol).toFloat(&ok);
        m_profitFactor = query.value(factorCol).toFloat(&ok);
        m_minPrice = query.value(minCol).toFloat(&ok);
    }

    if (!m_costManipulation) {
        return false;
    }

    return true;
}

/**
 * @brief MaintenanceHandler::getPackPriceUpData
 * @param item
 * @return
 */
bool MaintenanceHandler::getPackPriceUpData(const QString &item)
{
    bool ok;
    m_costManipulation = 0;

    QString statLine = QString ("SELECT * FROM tb_packing WHERE packing_name=\'%1\'").arg(item);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    int factorCol = query.record().indexOf("cost_factor");
    int costCol = query.record().indexOf("cost");

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        qDebug("Nenhum item contrado");
        return false;
    }

    while (query.next()) {
        m_packFactor = query.value(factorCol).toFloat(&ok);
        m_CostPack = query.value(costCol).toFloat(&ok);
    }

    return true;
}

/**
 * @brief MaintenanceHandler::packFactor
 * @return
 */
QString MaintenanceHandler::packFactor() const
{
    QString str;

    str.sprintf("%.2f", m_packFactor);
    return str;
}

/**
 * @brief MaintenanceHandler::costPack
 * @return
 */
QString MaintenanceHandler::costPack() const
{
    QString str;

    str.sprintf("%.2f", m_CostPack);
    return str;
}

/**
 * @brief MaintenanceHandler::setPackFactor
 * @param packFactor
 */
void MaintenanceHandler::setPackFactor(QString packFactor)
{
    bool ok;
    packFactor.replace(",", ".");
    m_packFactor = packFactor.toFloat(&ok);
    qDebug() << "Fator custo embalagem : " << m_packFactor;
}

/**
 * @brief MaintenanceHandler::setCostPack
 * @param CostPack
 */
void MaintenanceHandler::setCostPack(QString CostPack)
{
    bool ok;
    CostPack.replace(",", ".");
    m_CostPack = CostPack.toFloat(&ok);
    qDebug() << "Custo embalagem : " << m_CostPack;
}
