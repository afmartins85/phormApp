#include "elementhandler.h"
#include <QSqlError>
#include <QSqlResult>
#include <QDebug>

ElementHandler::ElementHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("ElementHandler contructor");
}

ElementHandler::~ElementHandler()
{

}

/**
 * @brief ElementHandler::setDatabase
 * @param db
 */
void ElementHandler::setDatabase(const QSqlDatabase &db)
{
   m_database = db;
}

/**
 * @brief ElementHandler::isOpenDB
 * @return
 */
bool ElementHandler::isOpenDB()
{
    return m_database.isOpen();
}

/**
 * @brief ElementHandler::subGroupId
 * @return
 */
QString ElementHandler::subGroupId() const
{
    QString str;

    str.sprintf("%d", m_subGroupId);
    return str;
}

/**
 * @brief ElementHandler::setSubGroupId
 * @param subGroupId
 */
void ElementHandler::setSubGroupId(QString subGroupId)
{
    bool ok;

    QString statLine = QString ("SELECT sub_group_id FROM tb_element_sub_group WHERE sub_group_element=\'%1\'").arg(subGroupId);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_subGroupId = query.value(0).toInt(&ok);
    }

    qDebug() << "sub Grupo id: " << m_subGroupId;
}

/**
 * @brief ElementHandler::element
 * @return
 */
QString ElementHandler::element() const
{
    return m_element;
}

/**
 * @brief ElementHandler::setElement
 * @param element
 */
void ElementHandler::setElement(QString const &element)
{
    qDebug() << "Element Name: " << element;
    m_element = element;
}

/**
 * @brief ElementHandler::nickname
 * @return
 */
QString ElementHandler::nickname() const
{
    return m_nickname;
}

/**
 * @brief ElementHandler::setNickname
 * @param arg
 */
void ElementHandler::setNickname(QString const &nickname)
{
    qDebug() << "Apelido: " << nickname;
    m_nickname = nickname;
}

/**
 * @brief ElementHandler::unitmanip
 * @return
 */
QString ElementHandler::unitmanip() const
{
    QString str;

    str.sprintf("%d", m_unitmanip);
    return str;
}

/**
 * @brief ElementHandler::setUnitmanip
 * @param arg
 */
void ElementHandler::setUnitmanip(QString const &unit)
{
    bool ok;
    qDebug() << "Unidade de Manip.: " << unit;

    QString statLine = QString ("SELECT unit_id FROM tb_units WHERE unit=\'%1\'").arg(unit);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_unitmanip = query.value(0).toInt(&ok);
    }

    qDebug() << "id: " << m_unitmanip;
}

/**
 * @brief ElementHandler::unitstock
 * @return
 */
QString ElementHandler::unitstock() const
{
    QString str;
    str.sprintf("%d", m_unitstock);
    return str;
}


/**
 * @brief ElementHandler::setUnitstock
 * @param arg
 */
void ElementHandler::setUnitstock(QString const &unit)
{
    bool ok;
    qDebug() << "Unidade de estoque: " << unit;

    QString statLine = QString ("SELECT unit_id FROM tb_units WHERE unit=\'%1\'").arg(unit);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_unitstock = query.value(0).toInt(&ok);
    }

    qDebug() << "id: " << m_unitstock;
}

/**
 * @brief ElementHandler::getUnitMan
 * @return
 */
QString ElementHandler::getUnitMan()
{
   return m_unitManipulationText;
}

/**
 * @brief ElementHandler::getUnitStock
 * @return
 */
QString ElementHandler::getUnitStock()
{
    return m_unitStockText;
}

/**
 * @brief ElementHandler::concentration
 * @return
 */
QString ElementHandler::concentration() const
{
    QString str = QString ("%1").arg(m_concentration);

    return str;
}

/**
 * @brief ElementHandler::setConcentration
 * @param arg
 */
void ElementHandler::setConcentration(QString &c)
{
    bool ok;
    c.replace(",", ".");
    m_concentration = c.toFloat(&ok);
    qDebug() << "Concentração: " << m_concentration << "%";
}


/**
 * @brief ElementHandler::factor
 * @return
 */
QString ElementHandler::factor() const
{
    QString str = QString ("%1").arg(m_factor);

    return str;
}


/**
 * @brief ElementHandler::setFactor
 * @param arg
 */
void ElementHandler::setFactor(QString &factor)
{
    bool ok;
    factor.replace(",", ".");
    m_factor = factor.toFloat(&ok);
    qDebug() << "Fator de correção: " << m_factor;
}

/**
 * @brief ElementHandler::doseMin
 * @return
 */
QString ElementHandler::doseMin() const
{
    QString str = QString ("%1").arg(m_doseMin);

    return str;
}

/**
 * @brief ElementHandler::doseMax
 * @return
 */
QString ElementHandler::doseMax() const
{
    QString str = QString ("%1").arg(m_doseMax);

    return str;
}

/**
 * @brief ElementHandler::setDoseMin
 * @param arg
 */
void ElementHandler::setDoseMin(QString &dose)
{
    bool ok;
    dose.replace(",", ".");
    m_doseMin = dose.toFloat(&ok);
    qDebug() << "Dose Mínima: " << m_doseMin;
}

void ElementHandler::setDoseMax(QString &dose)
{
    bool ok;
    dose.replace(",", ".");
    m_doseMax = dose.toFloat(&ok);
    qDebug() << "Dose Máxima: " << m_doseMax;
}

/**
 * @brief ElementHandler::isControlled
 * @return
 */
bool ElementHandler::isControlled() const
{
    return m_isControlled;
}

/**
 * @brief ElementHandler::setIsControlled
 * @param arg
 */
void ElementHandler::setIsControlled(bool const &arg)
{
    m_isControlled = arg;
}

/**
 * @brief ElementHandler::lote
 * @return
 */
QString ElementHandler::lote() const
{
    return m_lote;
}

/**
 * @brief ElementHandler::setLote
 * @param lote
 */
void ElementHandler::setLote(QString &lote)
{
    m_lote = lote;
    qDebug() << "Nº Lote: " << m_lote;
}

/**
 * @brief ElementHandler::description
 * @return
 */
QString ElementHandler::description() const
{
    return m_description;
}

/**
 * @brief ElementHandler::setDescription
 * @param arg
 */
void ElementHandler::setDescription(QString arg)
{
    m_description = arg;
    qDebug() << "Observação: " << m_description;
}

/**
 * @brief ElementHandler::uiBase
 * @return
 */
QString ElementHandler::uiBase() const
{
    QString str = QString ("%1").arg(m_uiBase);

    return str;
}

/**
 * @brief ElementHandler::uiEquivalent
 * @return
 */
QString ElementHandler::uiEquivalent() const
{
    QString str = QString ("%1").arg(m_uiEquivalent);

    return str;
}

/**
 * @brief ElementHandler::uiUnit
 * @return
 */
QString ElementHandler::uiUnit() const
{
    QString str;

    str.sprintf("%d", m_uiUnit);
    return str;
}

/**
 * @brief ElementHandler::utrBase
 * @return
 */
QString ElementHandler::utrBase() const
{
    QString str = QString ("%1").arg(m_utrBase);

    return str;
}

/**
 * @brief ElementHandler::utrEquivalent
 * @return
 */
QString ElementHandler::utrEquivalent() const
{
    QString str = QString ("%1").arg(m_utrEquivalent);

    return str;
}

/**
 * @brief ElementHandler::utrUnit
 * @return
 */
QString ElementHandler::utrUnit() const
{
    QString str;

    str.sprintf("%d", m_utrUnit);
    return str;
}

/**
 * @brief ElementHandler::setUiBase
 * @param arg
 */
void ElementHandler::setUiBase(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_uiBase = arg.toFloat(&ok);
    qDebug() << "Qtd. Ui base: " << m_uiBase;
}

/**
 * @brief ElementHandler::setUiEquivalent
 * @param arg
 */
void ElementHandler::setUiEquivalent(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_uiEquivalent = arg.toFloat(&ok);
    qDebug() << "Qtd. Ui Equivalente: " << m_uiEquivalent;
}

/**
 * @brief ElementHandler::setUiUnit
 * @param arg
 */
void ElementHandler::setUiUnit(const QString &arg)
{
    bool ok;

    QString statLine = QString ("SELECT unit_id FROM tb_units WHERE unit=\'%1\'").arg(arg);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_uiUnit = query.value(0).toInt(&ok);
    }

    qDebug() << "Unidade de Manip.: " << m_uiUnit;
}

/**
 * @brief ElementHandler::setUtrBase
 * @param arg
 */
void ElementHandler::setUtrBase(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_utrBase = arg.toFloat(&ok);
    qDebug() << "Qtd. Utr base: " << m_utrBase;
}

/**
 * @brief ElementHandler::setUtrEquivalent
 * @param arg
 */
void ElementHandler::setUtrEquivalent(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_utrEquivalent = arg.toFloat(&ok);
    qDebug() << "Qtd. Utr Equivalente: " << m_utrEquivalent;
}

/**
 * @brief ElementHandler::setUtrUnit
 * @param arg
 */
void ElementHandler::setUtrUnit(const QString &arg)
{
    bool ok;

    QString statLine = QString ("SELECT unit_id FROM tb_units WHERE unit=\'%1\'").arg(arg);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return;
    }

    while (query.next()) {
        m_utrUnit = query.value(0).toInt(&ok);
    }

    qDebug() << "UTR Unidade de equivalencia: " << m_utrUnit;
}

/**
 * @brief ElementHandler::cost
 * @return
 */
QString ElementHandler::cost() const
{
    QString str = QString ("%1").arg(m_cost);

    return str;
}

/**
 * @brief ElementHandler::costFactor
 * @return
 */
QString ElementHandler::costFactor() const
{
    QString str = QString ("%1").arg(m_costFactor);

    return str;
}

/**
 * @brief ElementHandler::stockMin
 * @return
 */
QString ElementHandler::stockMin() const
{
    QString str = QString ("%1").arg(m_stockMin);

    return str;
}

/**
 * @brief ElementHandler::stockMax
 * @return
 */
QString ElementHandler::stockMax() const
{
    QString str = QString ("%1").arg(m_stockMax);

    return str;
}

/**
 * @brief ElementHandler::density
 * @return
 */
QString ElementHandler::density() const
{
    QString str = QString ("%1").arg(m_density);

    return str;
}

/**
 * @brief ElementHandler::setCost
 * @param arg
 */
void ElementHandler::setCost(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_cost = arg.toFloat(&ok);
    qDebug() << "Custo: " << m_cost;
}

/**
 * @brief ElementHandler::setCostFactor
 * @param arg
 */
void ElementHandler::setCostFactor(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_costFactor = arg.toFloat(&ok);
    qDebug() << "Fator de Custo: " << m_costFactor;
}

/**
 * @brief ElementHandler::setStockMin
 * @param arg
 */
void ElementHandler::setStockMin(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_stockMin = arg.toFloat(&ok);
    qDebug() << "Estoque mínimo: " << m_stockMin;
}

/**
 * @brief ElementHandler::setStockMax
 * @param arg
 */
void ElementHandler::setStockMax(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_stockMax = arg.toFloat(&ok);
    qDebug() << "Estoque máximo: " << m_stockMax;
}

/**
 * @brief ElementHandler::setDensity
 * @param arg
 */
void ElementHandler::setDensity(QString arg)
{
    bool ok;
    arg.replace(",", ".");
    m_density = arg.toFloat(&ok);
    qDebug() << "Densidade: " << m_density;
}

/**
 * @brief ElementHandler::loteSupplier
 * @return
 */
QString ElementHandler::loteSupplier() const
{
    return m_loteSupplier;
}

/**
 * @brief ElementHandler::setLoteSupplier
 * @param arg
 */
void ElementHandler::setLoteSupplier(QString arg)
{
    m_loteSupplier = arg;
    qDebug() << "Lote do fornecendor: " << m_loteSupplier;
}

/**
 * @brief ElementHandler::check
 * @return
 */
bool ElementHandler::checkDependence (void)
{

//    if ((!m_concentration) || (!m_factor) || (!m_doseMin) ||
//            (!m_doseMax) || (!m_lote) || (!m_uiBase) || (!m_uiEquivalent) ||
//            (!m_utrBase) || (!m_utrEquivalent) || (!m_cost) || (!m_costFactor) ||
//            (!m_stockMin) || (!m_stockMax) || (!m_density)) {

//        return false;
//    }

    if (!m_element.compare("")) {
        return false;
    }
    if ((!m_lote.size()) || (!m_concentration) || (!m_cost) || (!m_costFactor) ) {
        return false;
    }

    return true;
}

/**
 * @brief ElementHandler::insert
 * @return
 */
bool ElementHandler::insert(void)
{
    bool ok;
    int m_code_element_id;

    QString statLine = QString ("INSERT INTO tb_element_base "
                                "(name, nickname, unit_manip, unit_stock, concentration, correction_factor,"
                                "dose_min, dose_max, observation, internal_lote, sub_group_id_tb_element_sub_group)"
                                " VALUES (\'%1\', \'%2\', %3, %4, %5, %6, %7, %8, \'%9\', \'%10\', %11)").arg(m_element).arg(
                m_nickname).arg(m_unitmanip).arg(m_unitstock).arg(m_concentration).arg(m_factor).arg(m_doseMin).arg(m_doseMax).arg(
                m_description).arg(m_lote).arg(m_subGroupId);

    qDebug() << "Insert Elements:\n" << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();
    qDebug() << err;
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("SELECT code_element FROM tb_element_base WHERE name=\'%1\'").arg(m_element);

    QSqlQuery query = m_database.exec(statLine);

    err = m_database.lastError();
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        m_code_element_id = query.value(0).toInt(&ok);
    }

    qDebug() << "Id do elemento: " << m_code_element_id;

    statLine = QString ("INSERT INTO tb_finance (cost, cost_factor, stock_min, stock_max, density, lote_supplier,"
                        "code_element_tb_element_base) VALUES (%1, %2, %3, %4, %5, \'%6\', %7)").arg(m_cost).arg(
                m_costFactor).arg(m_stockMin).arg(m_stockMax).arg(m_density).arg(m_loteSupplier).arg(m_code_element_id);

    qDebug() << "Insert Finance:\n" << statLine;

    m_database.exec(statLine);

    err = m_database.lastError();

    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("INSERT INTO tb_conversion (ui_base, ui_equivalent, unit_conv_ui, utr_base, utr_equivalent,"
                        " unit_conv_utr, code_element_tb_element_base) VALUES (%1, %2, %3, %4, %5, %6, %7)").arg(m_uiBase).arg(
                m_uiEquivalent).arg(m_uiUnit).arg(m_utrBase).arg(m_utrEquivalent).arg(m_utrUnit).arg(m_code_element_id);

    qDebug() << "Insert Conversion:\n" << statLine;

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
 * @brief ElementHandler::update
 * @return
 */
bool ElementHandler::update(void)
{
    QString statLine = QString ("UPDATE tb_element_base "
                "SET name=\'%1\', nickname=\'%2\', unit_manip= %3, unit_stock=%4, "
                "concentration=%5, correction_factor=%6, dose_min=%7, dose_max=%8, "
                "observation=\'%9\', internal_lote=\'%10\', sub_group_id_tb_element_sub_group=%11 "
                "WHERE code_element=%12").arg(m_element).arg(m_nickname).arg(m_unitmanip)
            .arg(m_unitstock).arg(m_concentration).arg(m_factor).arg(m_doseMin).arg(m_doseMax)
            .arg(m_description).arg(m_lote).arg(m_subGroupId).arg(m_codeElement);

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    qDebug() << "Id do elemento: " << m_codeElement;

    statLine = QString ("UPDATE tb_finance SET cost=%1, cost_factor=%2, stock_min=%3, stock_max=%4, "
                        "density=%5, lote_supplier=\'%6\' WHERE code_element_tb_element_base=%7")
                        .arg(m_cost).arg(m_costFactor).arg(m_stockMin).arg(m_stockMax).arg(m_density)
                        .arg(m_loteSupplier).arg(m_codeElement);

    m_database.exec(statLine);

    err = m_database.lastError();

    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("UPDATE tb_conversion SET ui_base=%1, ui_equivalent=%2, unit_conv_ui=%3, "
                        "utr_base=%4, utr_equivalent=%5, unit_conv_utr=%6 WHERE code_element_tb_element_base=%7")
                        .arg(m_uiBase).arg(m_uiEquivalent).arg(m_uiUnit).arg(m_utrBase)
                        .arg(m_utrEquivalent).arg(m_utrUnit).arg(m_codeElement);

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
 * @brief ElementHandler::loadElementList
 */
void ElementHandler::loadElementList()
{
    QSqlQuery sec_query;

    //####  Requisita todos os elementos presentes no banco
    QString statLine = QString ("SELECT name FROM tb_element_base");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_elementListCount = query.size();

//    int nameCol = query.record().indexOf("group_name");

    int i = 0;
    while (query.next()) {
        m_elementList.append(query.value(0).toString());
        i++;
    }

    //##### Requisita todos os sub grupos registrados em ordem por elemento

    statLine = QString ("SELECT sub_group_id_tb_element_sub_group FROM tb_element_base");

    query = m_database.exec(statLine);
    m_subGroupListCount = query.size();

    i = 0;
    bool ok;
    int j = 0;
    int subGroupId;
    int groupId;

    while (query.next()) {

        subGroupId = query.value(0).toInt(&ok);

        //##### Requisita o nome do sub grupo
        statLine = QString ("SELECT sub_group_element FROM tb_element_sub_group WHERE sub_group_id=%1").arg(subGroupId);
        sec_query = m_database.exec(statLine);

        while (sec_query.next()) {
            m_subGroupList.append(sec_query.value(0).toString());

            //##### Requisita o nome do grupo o qual sub grupo pertence
            statLine = QString ("SELECT group_id_tb_element_group FROM tb_element_sub_group"
                                " WHERE sub_group_element=\'%1\'").arg(m_subGroupList.at(i));

            sec_query = m_database.exec(statLine);

            while (sec_query.next()) {

                groupId = sec_query.value(0).toInt(&ok);

                statLine = QString ("SELECT group_name FROM tb_element_group WHERE group_id=%1").arg(groupId);
                sec_query = m_database.exec(statLine);

                while (sec_query.next()) {
                    m_groupList.append(sec_query.value(0).toString());
                    j++;
                }
            }
            i++;
        }
    }
}

/**
 * @brief ElementHandler::elementListCount
 * @return
 */
int ElementHandler::elementListCount()
{
    return m_elementListCount;
}

/**
 * @brief ElementHandler::elementListAt
 * @param index
 * @return
 */
QString ElementHandler::elementListAt(int index)
{
    if (m_elementListCount < 0) {
        return QString ("");
    }

    return m_elementList.at(index);
}

/**
 * @brief ElementHandler::elementSubGroupAt
 * @param index
 * @return
 */
QString ElementHandler::elementSubGroupAt(int index)
{
    if (m_elementListCount < 0) {
        return QString ("");
    }

    return m_subGroupList.at(index);
}

/**
 * @brief ElementHandler::elementGroupAt
 * @param index
 * @return
 */
QString ElementHandler::elementGroupAt(int index)
{
    if (m_elementListCount < 0) {
        return QString ("");
    }

    return m_groupList.at(index);
}

/**
 * @brief ElementHandler::remove
 * @param item
 * @return
 */
bool ElementHandler::remove(QString item)
{
    bool ok;
    int element_id;
    int conversion_id;
    int finance_id;

    QString statLine = QString ("SELECT code_element FROM tb_element_base WHERE name=\'%1\'").arg(item);

    qDebug() << statLine;
    QSqlQuery query = m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();

    err = m_database.lastError();
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        element_id = query.value(0).toInt(&ok);
    }

    qDebug() << "Id do elemento: " << element_id;

    //#### REMOVE ITEM DA TABELA DE CONVERSÃO
    statLine = QString ("SELECT conversion_id FROM tb_conversion WHERE code_element_tb_element_base=%1").arg(element_id);

    query = m_database.exec(statLine);
    qDebug() << err.text();

    err = m_database.lastError();
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        conversion_id = query.value(0).toInt(&ok);
    }
    statLine = QString ("DELETE FROM tb_conversion WHERE conversion_id=%1").arg(conversion_id);

    query = m_database.exec(statLine);
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    //#### REMOVE ITEM DA TABELA DE FINANÇAS
    statLine = QString ("SELECT cost_id FROM tb_finance WHERE code_element_tb_element_base=%1").arg(element_id);

    query = m_database.exec(statLine);
    qDebug() << err.text();

    err = m_database.lastError();
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    while (query.next()) {
        finance_id = query.value(0).toInt(&ok);
    }
    statLine = QString ("DELETE FROM tb_finance WHERE cost_id=%1").arg(finance_id);

    query = m_database.exec(statLine);
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    //#### REMOVE ITEM DA TABELA DE ELEMENTOS
    statLine = QString ("DELETE FROM tb_element_base WHERE code_element=%1").arg(element_id);

    query = m_database.exec(statLine);
    qDebug() << err.text();
    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    loadElementList();

    return true;
}

/**
 * @brief ElementHandler::setSelect
 * @param select
 */
bool ElementHandler::setSelect(QString select)
{
    bool ok;

    //OBTEM OS DADOS DA TABELA DE INSUMO
    QString statLine = QString ("SELECT * FROM tb_element_base WHERE name=\'%1\'").arg(select);

    QSqlQuery query = m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    int count = query.size();

    if (!count) {
        return false;
    }

    int idCol = query.record().indexOf("code_element");
    int nameCol = query.record().indexOf("name");
    int nickCol = query.record().indexOf("nickname");
    int unitManCol = query.record().indexOf("unit_manip");
    int unitStockCol = query.record().indexOf("unit_stock");
    int concentrationCol = query.record().indexOf("concentration");
    int corFactorCol = query.record().indexOf("correction_factor");
    int minCol = query.record().indexOf("dose_min");
    int maxCol = query.record().indexOf("dose_max");
    int controlCol = query.record().indexOf("is_controlled");
    int obsCol = query.record().indexOf("observation");
    int intCol = query.record().indexOf("internal_lote");
    int subCol = query.record().indexOf("sub_group_id_tb_element_sub_group");

    query.next();

    m_codeElement = query.value(idCol).toInt(&ok);
    m_element = query.value(nameCol).toString();
    m_nickname = query.value(nickCol).toString();

    m_unitmanip = query.value(unitManCol).toInt(&ok);
    m_unitManipulationText = getUnitString(m_unitmanip);

    m_unitstock = query.value(unitStockCol).toInt(&ok);
    m_unitStockText = getUnitString(m_unitstock);

    m_concentration = query.value(concentrationCol).toFloat(&ok);
    m_factor = query.value(corFactorCol).toFloat(&ok);
    m_doseMin = query.value(minCol).toFloat(&ok);
    m_doseMax = query.value(maxCol).toFloat(&ok);
    m_isControlled = query.value(controlCol).toBool();
    m_description = query.value(obsCol).toString();
    m_lote = query.value(intCol).toString();

    //IDENTIFICA O SUB GRUPO DO INSUMO
    m_subGroupId = query.value(subCol).toInt((&ok));
    statLine = QString ("SELECT * FROM tb_element_sub_group WHERE sub_group_id=%1").arg(m_subGroupId);

    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    count = query.size();
    if (!count) {
        return false;
    }
    query.next();

    int subElementCol = query.record().indexOf("sub_group_element");
    int groupIdCol = query.record().indexOf("group_id_tb_element_group");

    m_subGroupElement = query.value(subElementCol).toString();
    int groupId = query.value(groupIdCol).toInt(&ok);

    //IDENTIFICA O GRUPO DO INSUMO
    statLine = QString ("SELECT group_name FROM tb_element_group WHERE group_id=%1").arg(groupId);

    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    count = query.size();
    if (!count) {
        return false;
    }
    query.next();

    m_groupName = query.value(0).toString();

    //IDENTIFICA OS DADOS DE CONVERSAO
    statLine = QString ("SELECT * FROM tb_conversion WHERE code_element_tb_element_base=%1").arg(m_codeElement);

    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    count = query.size();
    if (!count) {
        return false;
    }
    query.next();

    int uiBaseCol = query.record().indexOf("ui_base");
    int uiEquivalentCol = query.record().indexOf("ui_equivalent");
    int unitUiCol = query.record().indexOf("unit_conv_ui");
    int utrBaseCol = query.record().indexOf("utr_base");
    int utrEquivalentCol = query.record().indexOf("utr_equivalent");
    int unitUtrCol = query.record().indexOf("unit_conv_utr");

    m_uiBase = query.value(uiBaseCol).toFloat(&ok);
    m_uiEquivalent = query.value(uiEquivalentCol).toFloat(&ok);
    m_uiUnit = query.value(unitUiCol).toInt(&ok);
    m_uiConv = getUnitString(m_uiUnit);

    m_utrBase = query.value(utrBaseCol).toFloat(&ok);
    m_utrEquivalent = query.value(utrEquivalentCol).toFloat(&ok);
    m_utrUnit = query.value(unitUtrCol).toInt(&ok);
    m_utrConv = getUnitString(m_utrUnit);

    //IDENTIFICA OS DADOS DE CONVERSAO
    statLine = QString ("SELECT * FROM tb_finance WHERE code_element_tb_element_base=%1").arg(m_codeElement);

    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    count = query.size();
    if (!count) {
        return false;
    }
    query.next();

    int costCol = query.record().indexOf("cost");
    int costFactorCol = query.record().indexOf("cost_factor");
    int minStockCol = query.record().indexOf("stock_min");
    int maxStockCol = query.record().indexOf("stock_max");
    int densityCol = query.record().indexOf("density");
    int loteSupCol = query.record().indexOf("lote_supplier");

    m_cost = query.value(costCol).toFloat(&ok);
    m_costFactor = query.value(costFactorCol).toFloat(&ok);
    m_stockMin = query.value(minStockCol).toFloat(&ok);
    m_stockMax = query.value(maxStockCol).toFloat(&ok);
    m_density = query.value(densityCol).toFloat(&ok);
    m_loteSupplier = query.value(loteSupCol).toString();

    return true;
}

/**
 * @brief ElementHandler::select
 * @return
 */
QString ElementHandler::select() const
{
    return m_select;
}

/**
 * @brief ElementHandler::getSubGroupElement
 * @return
 */
QString ElementHandler::getSubGroupElement()
{
    return m_subGroupElement;
}

/**
 * @brief ElementHandler::getGroupName
 * @return
 */
QString ElementHandler::getGroupName()
{
    return m_groupName;
}

/**
 * @brief ElementHandler::getUiConv
 * @return
 */
QString ElementHandler::getUiConv()
{
    return m_uiConv;
}

/**
 * @brief ElementHandler::getUtrConv
 * @return
 */
QString ElementHandler::getUtrConv()
{
    return m_utrConv;
}

/**
 * @brief ElementHandler::getUnitString
 * @param index
 * @return
 */
QString ElementHandler::getUnitString(int index)
{
    QString statLine = QString ("SELECT unit FROM tb_units WHERE unit_id=%1").arg(index);

    QSqlQuery query = m_database.exec(statLine);

    query.next();

    QString unit = query.value(0).toString();

    if (unit == NULL) {
        unit = "mg   ";
    }
    return unit;
}

