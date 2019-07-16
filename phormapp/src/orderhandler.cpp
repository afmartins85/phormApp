#define _XOPEN_SOURCE

#include "orderhandler.h"
#include <QAbstractItemModel>
#include <QDateTime>
#include <QDebug>
#include <QGuiApplication>
#include <QHeaderView>
#include <QPainter>
#include <QPrinterInfo>
#include <QSizeF>
#include <QSqlError>
#include <QSqlResult>
#include <QStandardItem>
#include <QTableView>
#include <QTableWidget>
#include <QTimer>
#include "printersetup.h"
#include "time.h"

OrderHandler::OrderHandler(QQuickItem *parent) : QQuickItem(parent) {
  m_printer = new QPrinter;
  qDebug("OrderHandler contructor");

  m_countPage = 0;
  m_countlabels = 0;
}

OrderHandler::~OrderHandler() {}

/**
 * @brief OrderHandler::setDatabase
 * @param db
 */
void OrderHandler::setDatabase(const QSqlDatabase &db) { m_database = db; }

/**
 * @brief OrderHandler::loadPrintConfig
 */
void OrderHandler::loadPrintConfig(void) {
  PrinterSetup ptrSet;

  ptrSet.setDatabase(m_database);
  ptrSet.checkPrintersRecord();
  m_labelsPrinter = ptrSet.getLabelsPrint();
  m_orderPrinter = ptrSet.getOrderPrint();

  qDebug() << "IMPRESSORA PARA ORDENS: " << m_orderPrinter;
  qDebug() << "IMPRESSORA PARA ETIQUETAS: " << m_labelsPrinter;
}

/**
 * @brief OrderHandler::phorm
 * @return
 */
QString OrderHandler::phorm() const { return m_phorm; }

/**
 * @brief OrderHandler::setPhorm
 * @param phorm
 */
void OrderHandler::setPhorm(QString const &phorm) {
  qDebug() << "[ORDER] phorm: " << phorm;
  // Carrega o valor de preço mínimo para a phormula
  setMinPrice(phorm);
  m_phorm = phorm;
}

/**
 * @brief OrderHandler::setMinPrice
 * @param phorm
 */
bool OrderHandler::setMinPrice(QString const &phorm) {
  QString statLine =
      QString(
          "SELECT price.min_price "
          "FROM tb_pharma_phorm AS phorm, tb_composition_price AS price "
          "WHERE phorm.phorm_name='%1' AND price.phorm_id_tb_pharma_phorm "
          "= phorm.phorm_id")
          .arg(phorm);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  while (query.next()) {
    m_minCostPhorm = query.value(0).toFloat();
  }
  return true;
}

/**
 * @brief OrderHandler::getMinPrice
 * @return
 */
float OrderHandler::getMinPrice() { return m_minCostPhorm; }

/**
 * @brief OrderHandler::phormQtd
 * @return
 */
QString OrderHandler::phormQtd() const { return m_phormQtd; }

/**
 * @brief OrderHandler::setPhormQtd
 * @param phormQtd
 */
void OrderHandler::setPhormQtd(QString const &phormQtd) {
  qDebug() << "[ORDER] phorm qtd.: " << phormQtd;
  m_phormQtd = phormQtd;
}

/**
 * @brief OrderHandler::phormQtdUnit
 * @return
 */
QString OrderHandler::phormQtdUnit() const { return m_phormQtdUnit; }

/**
 * @brief OrderHandler::setPhormQtdUnit
 * @param phormQtdUnit
 */
void OrderHandler::setPhormQtdUnit(QString const &phormQtdUnit) {
  qDebug() << "[ORDER] phorm qtd. unit: " << phormQtdUnit;
  m_phormQtdUnit = phormQtdUnit;
}

/**
 * @brief OrderHandler::Element
 * @return
 */
QString OrderHandler::element() const { return m_element; }

/**
 * @brief OrderHandler::setElement
 * @param Element
 */
void OrderHandler::setElement(QString const &element) {
  qDebug() << "[ORDER] element: " << element;
  m_element = element;
}

/**
 * @brief OrderHandler::elementAt
 * @param index
 * @return
 */
QString OrderHandler::elementAt(int index) { return m_elementList.at(index); }

/**
 * @brief OrderHandler::elementQtd
 * @return
 */
QString OrderHandler::elementQtd() const { return m_elementQtd; }

/**
 * @brief OrderHandler::setElementQtd
 * @param elementQtd
 */
void OrderHandler::setElementQtd(QString const &elementQtd) {
  qDebug() << "[ORDER] element qtd.: " << elementQtd;
  m_elementQtd = elementQtd;
}

/**
 * @brief OrderHandler::elementQtdAt
 * @param index
 * @return
 */
QString OrderHandler::elementQtdAt(int index) {
  QString str = QString("%1").arg(m_qtdBaseList.at(index));
  qDebug() << "loaded qtd: " << str;

  return str;
}

/**
 * @brief OrderHandler::elementQtdUnit
 * @return
 */
QString OrderHandler::elementQtdUnit() const { return m_elementQtdUnit; }

/**
 * @brief OrderHandler::setElementQtdUnit
 * @param elementQtdUnit
 */
void OrderHandler::setElementQtdUnit(QString const &elementQtdUnit) {
  qDebug() << "[ORDER] element qtd. unit: " << elementQtdUnit;
  m_elementQtdUnit = elementQtdUnit;
  m_elementQtdUnitList.append(elementQtdUnit);
}

/**
 * @brief OrderHandler::getElementQtdUnit
 * @param index
 * @return
 */
QString OrderHandler::getElementQtdUnit(int index) {
  return m_elementQtdUnitList.at(index);
}

/**
 * @brief OrderHandler::excip
 * @return
 */
QString OrderHandler::excip() {
  m_excipient = getExcipientName(m_excipient_id);

  return m_excipient;
}

/**
 * @brief OrderHandler::setExcip
 * @param excip
 */
void OrderHandler::setExcip(QString const &excip) {
  qDebug() << "excip. escolhido: " << excip;
  if (!excip.compare("...")) {
    m_excipient_id = 0;
    m_excipient = "";
  } else {
    m_excipient_id = getIdExcipient(excip);
    m_excipient = excip;
  }
}

/**
 * @brief OrderHandler::validate
 * @return
 */
QString OrderHandler::validate() {
  m_validate = getValidateDesc(m_validate_id);

  return m_validate;
}

/**
 * @brief OrderHandler::setValidate
 * @param validate
 */
void OrderHandler::setValidate(QString const &validate) {
  m_validate_id = getIdValidate(validate);
}

/**
 * @brief OrderHandler::usage
 * @return
 */
QString OrderHandler::usage() {
  char usage = getUsageDesc(static_cast<int>(m_element_composed_id));

  if (usage == 'I') {
    m_usage = "INTERNO";
  } else {
    m_usage = "EXTERNO";
  }

  return m_usage;
}

/**
 * @brief OrderHandler::setUsage
 * @param usage
 */
void OrderHandler::setUsage(QString usage) {
  if (!usage.compare("INTERNO")) {
    m_usage = 'I';
  } else {
    m_usage = 'E';
  }
}

/**
 * @brief OrderHandler::getIdPhorm
 * @param phorm
 * @return
 */
int OrderHandler::getIdPhorm(QString const &phorm) {
  bool ok;
  int id;

  QString statLine =
      QString("SELECT phorm_id FROM tb_pharma_phorm WHERE phorm_name=\'%1\'")
          .arg(phorm);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getPhormFromId
 * @param id
 * @return
 */
QString OrderHandler::getPhormFromId(const int id) {
  QString phorm;

  QString statLine =
      QString("SELECT phorm_name FROM tb_pharma_phorm WHERE phorm_id=%1")
          .arg(id);

  QSqlQuery query = m_database.exec(statLine);

  while (query.next()) {
    phorm = query.value(0).toString();
  }

  return phorm;
}

/**
 * @brief OrderHandler::getIdElement
 * @param element
 * @return
 */
int OrderHandler::getIdElement(const QString &element) {
  bool ok;
  int id;

  QString statLine =
      QString("SELECT code_element FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getElementFromId
 * @param id
 * @return
 */
QString OrderHandler::getElementFromId(const int id) {
  QString element;

  QString statLine =
      QString("SELECT name FROM tb_element_base WHERE code_element=%1").arg(id);

  QSqlQuery query = m_database.exec(statLine);

  while (query.next()) {
    element = query.value(0).toString();
  }

  return element;
}

/**
 * @brief OrderHandler::getUnitPrepareFromElement
 * @param element
 * @return
 */
QString OrderHandler::getUnitPrepareFromElement(const QString &element) {
  int unitId;
  QString str = "";

  QString statLine =
      QString("SELECT unit_manip FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  qDebug() << statLine;
  QSqlQuery query = m_database.exec(statLine);

  if (query.size() > 0) {
    query.next();

    unitId = query.value(0).toInt();

    statLine =
        QString("SELECT unit FROM tb_units WHERE unit_id=%1").arg(unitId);

    query = m_database.exec(statLine);
    query.next();

    str = query.value(0).toString();
  }

  return str;
}

/**
 * @brief OrderHandler::getIdUnit
 * @param unitBaseConvert
 * @return
 */
int OrderHandler::getIdUnit(const QString &unitBaseConvert) {
  bool ok;
  int id;

  QString statLine = QString("SELECT unit_id FROM tb_units WHERE unit=\'%1\'")
                         .arg(unitBaseConvert);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getUnitFromId
 * @param id
 * @return
 */
QString OrderHandler::getUnitFromId(const int id) {
  QString unit;

  QString statLine =
      QString("SELECT unit FROM tb_units WHERE unit_id=%1").arg(id);

  QSqlQuery query = m_database.exec(statLine);

  while (query.next()) {
    unit = query.value(0).toString();
  }

  return unit;
}

/**
 * @brief OrderHandler::getIdExcipient
 * @param excipient
 * @return
 */
int OrderHandler::getIdExcipient(const QString &excipient) {
  bool ok;
  int id;

  QString statLine =
      QString("SELECT element_id FROM tb_excipient WHERE element_desc=\'%1\'")
          .arg(excipient);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getExcipientName
 * @param index
 * @return
 */
QString OrderHandler::getExcipientName(int id) {
  QString excip;

  QString statLine =
      QString("SELECT element_desc FROM tb_excipient WHERE element_id=%1")
          .arg(id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return NULL;
  }

  while (query.next()) {
    excip = query.value(0).toString();
  }

  return excip;
}

/**
 * @brief OrderHandler::getIdValidate
 * @param validate
 * @return
 */
int OrderHandler::getIdValidate(const QString &validate) {
  bool ok;
  int id;

  QString statLine =
      QString("SELECT validate_id FROM tb_validate WHERE validate_desc=\'%1\'")
          .arg(validate);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << statLine;
  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getValidateDesc
 * @param id
 * @return
 */
QString OrderHandler::getValidateDesc(int id) {
  QString validate;

  QString statLine =
      QString("SELECT validate_desc FROM tb_validate WHERE validate_id=%1")
          .arg(id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return NULL;
  }

  while (query.next()) {
    validate = query.value(0).toString();
  }

  return validate;
}

/**
 * @brief OrderHandler::getUsageDesc
 * @param id
 * @return
 */
char OrderHandler::getUsageDesc(int id) {
  char usage;

  QString statLine =
      QString("SELECT use FROM tb_composition_order WHERE composition_id=%1")
          .arg(id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    usage = query.value(0).toChar().toLatin1();
  }

  return usage;
}

/**
 * @brief OrderHandler::getDensity
 * @param element
 * @return
 */
float OrderHandler::getDensity(QString const &element) {
  bool ok;
  int id;
  float density;

  QString statLine =
      QString("SELECT code_element FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  statLine = QString(
                 "SELECT density FROM tb_finance WHERE "
                 "code_element_tb_element_base=\'%1\'")
                 .arg(id);

  query = m_database.exec(statLine);
  // Number of columms

  err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    density = query.value(0).toFloat(&ok);
  }

  return density;
}

/**
 * @brief OrderHandler::getCorrectionFactor
 * @param element
 * @return
 */
float OrderHandler::getCorrectionFactor(QString const &element) {
  bool ok;
  float factor;

  QString statLine =
      QString("SELECT correction_factor FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    factor = query.value(0).toFloat(&ok);
  }

  return factor;
}

/**
 * @brief OrderHandler::getUnitStock
 * @param element
 * @return
 */
QString OrderHandler::getUnitStock(QString const &element) {
  bool ok;
  int unit_stock;
  QString un_Stock;

  QString statLine =
      QString("SELECT unit_stock FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return NULL;
  }

  query.next();

  unit_stock = query.value(0).toInt(&ok);

  statLine =
      QString("SELECT unit FROM tb_units WHERE unit_id=%1").arg(unit_stock);
  query = m_database.exec(statLine);

  err = m_database.lastError();
  if (err.isValid()) {
    return NULL;
  }

  query.next();
  un_Stock = query.value(0).toString();

  qDebug() << __FUNCTION__ << " | " << __LINE__
           << " : Unidade de estoque encontrada - " << un_Stock;

  return un_Stock;
}

/**
 * @brief OrderHandler::getDoctor
 * @param doctor
 * @return
 */
unsigned int OrderHandler::getDoctor(const QString &doctor) {
  bool ok;
  unsigned int crm;

  QString statLine =
      QString("SELECT crm FROM tb_doctors WHERE name=\'%1\'").arg(doctor);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    crm = query.value(0).toInt(&ok);
  }

  return crm;
}

/**
 * @brief OrderHandler::getDoctorFromCRM
 * @param crm
 * @return
 */
QString OrderHandler::getDoctorFromCRM(const int crm) {
  QString doctor = "";

  QString statLine =
      QString("SELECT name FROM tb_doctors WHERE crm=%1").arg(crm);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  while (query.next()) {
    doctor = query.value(0).toString();
  }

  return doctor;
}

/**
 * @brief OrderHandler::getClient
 * @param client
 * @return
 */
unsigned int OrderHandler::getClient(const QString &client) {
  bool ok;
  unsigned int code_client;

  QString statLine =
      QString("SELECT code_client FROM tb_clients WHERE name=\'%1\'")
          .arg(client);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    code_client = query.value(0).toInt(&ok);
  }

  return code_client;
}

/**
 * @brief OrderHandler::getInternalLote
 * @param element
 * @return
 */
QString OrderHandler::getInternalLote(const QString &element) {
  QString str = "0";

  QString statLine =
      QString("SELECT internal_lote FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return str;
  }

  if (query.size() <= 0) {
    return str;
  }

  query.next();

  str = query.value(0).toString();

  return str;
}

/**
 * @brief OrderHandler::getSpecCaps
 * @param spec
 * @return
 */
unsigned int OrderHandler::getSpecCaps(const QString &spec) {
  bool ok;
  unsigned int id;

  if (!spec.compare("...")) {
    qDebug() << "getSpecCaps: " << spec;
    return 0;
  }

  QString statLine =
      QString("SELECT cap_spec_id FROM tb_spec_caps WHERE spec_name=\'%1\'")
          .arg(spec);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getPacking
 * @param packing
 * @return
 */
unsigned int OrderHandler::getPacking(const QString &packing) {
  bool ok;
  unsigned int id;

  QString statLine =
      QString("SELECT packing_id FROM tb_packing WHERE packing_name=\'%1\'")
          .arg(packing);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    id = query.value(0).toInt(&ok);
  }

  return id;
}

/**
 * @brief OrderHandler::getCostFactor
 * @param id
 * @return
 */
float OrderHandler::getCostFactor(int id) {
  bool ok;
  float factor;

  QString statLine = QString(
                         "SELECT cost_factor FROM tb_finance WHERE "
                         "code_element_tb_element_base=%1")
                         .arg(id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    factor = query.value(0).toFloat(&ok);
  }

  return factor;
}

/**
 * @brief OrderHandler::getCost
 * @param id
 * @return
 */
float OrderHandler::getCost(int id) {
  bool ok;
  float cost;

  QString statLine =
      QString(
          "SELECT cost FROM tb_finance WHERE code_element_tb_element_base=%1")
          .arg(id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return -1;
  }

  while (query.next()) {
    cost = query.value(0).toFloat(&ok);
  }

  return cost;
}

/**
 * @brief OrderHandler::loadOrderCost
 * @param index
 */
void OrderHandler::loadOrderCost(int index) {
  bool ok;

  QString statLine =
      QString("SELECT cost FROM tb_order WHERE order_id=%1").arg(index);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    m_orderCost = 0;
    return;
  }

  while (query.next()) {
    m_orderCost = query.value(0).toFloat(&ok);
  }
  qDebug() << "CUSTO DA ORDEM " << m_orderCost;
}

/**
 * @brief OrderHandler::getOrderCost
 * @return
 */
float OrderHandler::getOrderCost(void) { return m_orderCost; }

/**
* @brief OrderHandler::loadCapsSpecification
*/
void OrderHandler::loadCapsSpecification(int phorm_id) {
  bool ok;
  QString statLine =
      QString(
          "SELECT * FROM tb_spec_caps WHERE phorm_id_tb_pharma_phorm=%1 "
          "ORDER BY volume ASC")
          .arg(phorm_id);

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return;
  }

  m_volumeCount = query.size();

  m_capsVolume.clear();
  m_capsSpecName.clear();

  int nameCol = query.record().indexOf("spec_name");
  int volumeCol = query.record().indexOf("volume");

  while (query.next()) {
    m_capsSpecName.append(query.value(nameCol).toString());
    m_capsVolume.append(query.value(volumeCol).toFloat(&ok));
  }

  for (int i = 0; i < m_volumeCount; i++) {
    qDebug() << "[ORDER] caps spec name: " << m_capsSpecName.at(i);
    qDebug() << "[ORDER] volume: " << m_capsVolume.at(i);
  }
}

/**
 * @brief OrderHandler::unitBaseConvert
 * @param unit
 * @return
 */
float OrderHandler::unitBaseConvert(QString unit) {
  unit.replace(" ", "\0");

  if (!unit.compare("mcg", Qt::CaseInsensitive)) {
    qDebug() << "[ORDER] unit: " << unit;
    return 0.000001;
  } else if (!unit.compare("g", Qt::CaseInsensitive)) {
    qDebug() << "[ORDER] unit: " << unit;
    return 1;
  } else if (!unit.compare("mg", Qt::CaseInsensitive)) {
    qDebug() << "[ORDER] unit: " << unit;
    return 0.001;
  } else if (!unit.compare("kg", Qt::CaseInsensitive)) {
    qDebug() << "[ORDER] unit: " << unit;
    return 1000;
  }

  return 0;
}

/**
 * @brief OrderHandler::endPurchaseProcess
 * @return
 */
bool OrderHandler::endPurchaseProcess() {
  int i;
  bool ok;
  float qtdPrepare = 0;

  if (!m_phorm.compare("CÁPSULAS")) {
    // Carrega as especificações de capsulas
    loadCapsSpecification(m_phorm_id);

    //#### Massa total para manipular
    for (i = 0; i < m_qtdPrepareList.count(); i++) {
      qtdPrepare += m_qtdPrepareList.at(i);
    }

    qDebug() << "[END ORDER] qtd. manipule: " << qtdPrepare << " g";

    // CALCULA O VOLUME
    float density = 0;
    float volume = 0;
    for (i = 0; i < m_qtdPrepareList.count(); i++) {
      density = getDensity(m_elementList.at(i));

      qDebug() << "[ORDER] denisity for calc: " << density;
      // Massa a ser manipuado X densidade
      volume += m_qtdPrepareList.at(i) / density;
    }
    qDebug() << "[ORDER] volume: " << volume << " ml";

    int phormQtd = m_phormQtd.toInt(&ok);
    float volumePerCaps = volume / phormQtd;
    qDebug() << "[ORDER] volume per Caps: " << volumePerCaps << " ml";

    //#### Identifica em qual capsula deverá ser manipulado
    int index;
    for (index = 0; index < m_volumeCount; index++) {
      if (volumePerCaps < m_capsVolume.at(index)) {
        m_capsSpecDetected = m_capsSpecName.at(index);
        qDebug() << "CAPSULA ENCONTRADA" << m_capsSpecDetected;
        break;
      }
    }

    m_dose = 1;
    if (index >= m_volumeCount) {
      index = m_volumeCount - 1;
      qDebug() << "NÃO FOI POSSIVEL ENCONTRAR UMA CAPSULA PARA ESSE VOLUME";
      qDebug() << "MANILUPAR NA CAPSULA " << m_capsSpecName.at(index);
      m_capsSpecDetected = m_capsSpecName.at(index);
      m_dose = volumePerCaps / m_capsVolume.at(index);

      int rest = (volumePerCaps / m_capsVolume.at(index)) / m_dose;
      if (rest > 0) {
        m_dose++;
      }
    }
    qDebug() << m_dose << " caps/dose";

    float maxVolumeDetected = 0;
    float remainder = 0;

    //#### Verifica se deve calcular excipiente
    if (volumePerCaps != m_capsVolume.at(index)) {
      qDebug() << "[ORDER] EXCIPIENTE DEVE SER ADICIONADO";

      m_isExcipient = true;
      maxVolumeDetected = m_capsVolume.at(index) * phormQtd;
      qDebug() << "[ORDER] Maximo volume que a capsula selecionada suporta "
               << maxVolumeDetected;

      remainder = volume - maxVolumeDetected;
      if (remainder < 0) {
        remainder = maxVolumeDetected - volume;
      }
      qDebug() << "[ORDER] Preencher com " << remainder << "ml de excipiente";

      float excip_density = getDensity(m_excipient);
      m_excip_bulk = remainder / excip_density;
      qDebug() << "[ORDER] " << m_excip_bulk << "g de excipiente";

      //#### CALCULA O CUSTO DO EXCIPIENT
      QString elementUnitStock = getUnitStock(m_excipient);
      float baseCost = unitBaseConvert(elementUnitStock);
      float excip_element_id = getIdElement(m_excipient);
      qDebug() << "excip id: " << excip_element_id;
      float costExcip = getCost(excip_element_id);
      costExcip = costExcip / baseCost;
      qDebug() << "[ORDER] Valor do excipiente R$ " << costExcip;
      float factorCostExcip = getCostFactor(excip_element_id);
      qDebug() << "[ORDER] Fator de custo do excipiente R$ " << factorCostExcip;
      cost_excip = costExcip * factorCostExcip * m_excip_bulk;

      qDebug() << "[ORDER] Custo do excipiente R$ " << cost_excip;
    } else {
      m_isExcipient = false;
    }
  } else if ((!m_phorm.compare("GEL")) || (!m_phorm.compare("CREMES")) ||
             (!m_phorm.compare("LOÇÃO")) || (!m_phorm.compare("POMADA")) ||
             (!m_phorm.compare("SABONETE LIQUIDO")) ||
             (!m_phorm.compare("SHAMPOO")) || (!m_phorm.compare("SOLUÇÃO")) ||
             (!m_phorm.compare("TALCO"))) {
    //#### Verifica se deve calcular excipiente
    int sumPrepare = 0;

    for (int i = 0; i < m_qtdPrepareList.count(); i++) {
      sumPrepare += m_qtdPrepareList.at(i);
    }

    qDebug() << "SumPrepare " << sumPrepare;
    qDebug() << "phormQtd " << m_phormQtd.toInt(&ok);
    if (sumPrepare < m_phormQtd.toInt(&ok)) {
      //#### CALCULA O CUSTO DO EXCIPIENT
      QString elementUnitStock = getUnitStock(m_excipient);
      float baseCost = unitBaseConvert(elementUnitStock);
      float excip_element_id = getIdElement(m_excipient);
      qDebug() << "excip id: " << excip_element_id;
      float costExcip = getCost(excip_element_id);
      costExcip = costExcip / baseCost;
      qDebug() << "[ORDER] Valor do excipiente R$ " << costExcip;
      float factorCostExcip = getCostFactor(excip_element_id);
      qDebug() << "[ORDER] Fator de custo do excipiente R$ " << factorCostExcip;
      cost_excip = costExcip * factorCostExcip;
      qDebug() << "[ORDER] Custo do excipiente R$ " << cost_excip;
      m_excip_bulk = m_phormQtd.toInt(&ok) - sumPrepare;
      qDebug() << "[ORDER] Qtd. Excipiente " << m_excip_bulk << " "
               << getUnitPrepareFromElement(m_excipient);
      m_isExcipient = true;
    } else {
      m_isExcipient = false;
    }
  }

  return true;
}

/**
 * @brief OrderHandler::removeExcip
 * @return
 */
float OrderHandler::removeExcip(void) {
  if (m_isExcipient == false) {
    return 0;
  }

  return cost_excip;
}

/**
 * @brief OrderHandler::addElementToCart
 * @param isRecording
 * @return
 */
bool OrderHandler::addElementToCart(bool isRecording) {
  bool ok;
  float prepare;

  m_momentQtdPrepare = "";
  m_momentQtdBase = 0;
  m_momentCost = "";

  m_phorm_id = getIdPhorm(m_phorm);
  int phormQtd = m_phormQtd.toInt(&ok);
  m_momentQtdBase = m_elementQtd.toFloat(&ok);
  float factor = getCorrectionFactor(m_element);

  if ((!m_phorm.compare("CÁPSULAS")) || (!m_phorm.compare("SACHÊS"))) {
    qDebug() << "[ORDER] id phorm: " << m_phorm_id;
    float base = unitBaseConvert(m_elementQtdUnit);
    qDebug() << "[ORDER] calc. base: " << base;
    //#### Calculo de massa a ser manipulado
    // Quantidade de Caps. X Quantidade de Insumo X base X fator de correção
    prepare = phormQtd * m_momentQtdBase * base * factor;
    m_momentQtdPrepare.sprintf("%.5f", prepare);
    qDebug() << "[ORDER] qtd. manipule: " << m_momentQtdPrepare
             << getUnitPrepareFromElement(m_element);

  } else if (!m_phorm.compare("FLORAIS")) {
  } else if ((!m_phorm.compare("GEL")) || (!m_phorm.compare("CREMES")) ||
             (!m_phorm.compare("LOÇÃO")) || (!m_phorm.compare("POMADA")) ||
             (!m_phorm.compare("SABONETE LIQUIDO")) ||
             (!m_phorm.compare("SHAMPOO")) || (!m_phorm.compare("SOLUÇÃO")) ||
             (!m_phorm.compare("TALCO"))) {
    //#### Calculo de massa a ser manipulado
    // Quantidade de Caps. X Quantidade de Insumo X base X fator de correção
    prepare = (phormQtd * (m_momentQtdBase / 100)) * factor;
    m_momentQtdPrepare.sprintf("%.5f", prepare);
    qDebug() << "[ORDER] qtd. manipule: " << m_momentQtdPrepare << " g";
  }

  // Parse das informacoes
  m_qtdPrepareList.append(prepare);
  m_qtdPrepareUnitList.append(getUnitPrepareFromElement(m_element));
  m_qtdBaseList.append(m_momentQtdBase);

  m_elementList.append(m_element);
  m_element_id = getIdElement(m_element);

  //#### CALCULA O CUSTO
  QString elementUnitStock = getUnitStock(m_element);
  float baseCost = unitBaseConvert(elementUnitStock);
  float costTemp = getCost(m_element_id);
  costTemp = costTemp / baseCost;
  float factorCostTemp = getCostFactor(m_element_id);
  float cost = costTemp * factorCostTemp * prepare;

  m_costList.append(cost);
  m_momentCost.sprintf("%.02f", cost);

  // Verifica se deve gravar ou nao os dados
  if (isRecording == true) {
    if (composedInsert() != true) {
      return false;
    }
  }

  return true;
}

/**
 * @brief OrderHandler::composedInsert
 * @return
 */
bool OrderHandler::composedInsert() {
  bool ok;

  QString statLine =
      QString(
          "INSERT INTO tb_composition_order "
          " VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11)")
          .arg(m_element_composed_id)
          .arg(m_phorm_id)
          .arg(m_phormQtd.toInt(&ok))
          .arg(getIdUnit(m_phormQtdUnit))
          .arg(m_element_id)
          .arg(getInternalLote(m_element))
          .arg(m_elementQtd.toInt(&ok))
          .arg(getIdUnit(m_elementQtdUnit))
          .arg(m_excipient_id)
          .arg(m_momentQtdPrepare)
          .arg(m_momentCost);

  qDebug() << "Insert Composition:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  qDebug() << err;
  if (err.isValid()) {
    return false;
  }

  return true;
}

/**
 * @brief OrderHandler::composedDelete
 * @return
 */
bool OrderHandler::composedDelete(int elementId) {
  QString statLine =
      QString(
          "DELETE FROM tb_composition_order "
          " WHERE composed=%1 AND code_element_tb_element_base=%2")
          .arg(m_element_composed_id)
          .arg(elementId);

  qDebug() << "Update Composition:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  if (err.isValid()) {
    return false;
  }

  return true;
}

/**
 * @brief OrderHandler::composedUpdate
 * @param oldElement
 * @param newElement
 * @param qtdElement
 * @param unit
 * @return
 */
bool OrderHandler::composedUpdate(int oldElement, int newElement,
                                  int qtdElement, QString unit) {
  QString statLine =
      QString(
          "UPDATE tb_composition_order SET "
          "code_element_tb_element_base=%1,"
          "qtd_element=%2,"
          "qtd_element_unit=%3,"
          "handle=%4,"
          "cost=%5 "
          "WHERE composed=%6 AND code_element_tb_element_base=%7")

          .arg(newElement)
          .arg(qtdElement)
          .arg(getIdUnit(unit))
          .arg(m_momentQtdPrepare)
          .arg(m_momentCost)
          .arg(m_element_composed_id)
          .arg(oldElement);

  qDebug() << "Update Composition:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  if (err.isValid()) {
    return false;
  }

  return true;
}

/**
 * @brief OrderHandler::getObsElement
 * @param element
 * @return
 */
QString OrderHandler::getObsElement(QString element) {
  QString str("");

  QString statLine =
      QString("SELECT observation FROM tb_element_base WHERE name=\'%1\'")
          .arg(element);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Obs. Element:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  if (err.isValid()) {
    return str;
  }

  query.next();

  str = query.value(0).toString();
  qDebug() << __FUNCTION__ << " [" << __LINE__ << str;

  return str;
}

/**
 * @brief OrderHandler::qtdPrepare
 * @return
 */
QString OrderHandler::qtdPrepare() { return m_momentQtdPrepare; }

/**
 * @brief OrderHandler::qtdBase
 * @return
 */
float OrderHandler::qtdBase() { return m_momentQtdBase; }

/**
 * @brief OrderHandler::cost
 * @return
 */
QString OrderHandler::cost() { return m_momentCost; }

/**
 * @brief OrderHandler::costListAt
 * @param index
 * @return
 */
float OrderHandler::costListAt(int index) { return m_costList.at(index); }

/**
 * @brief OrderHandler::getCapsSpecDetected
 * @return
 */
QString OrderHandler::getCapsSpecDetected() { return m_capsSpecDetected; }

/**
 * @brief OrderHandler::clearState
 */
void OrderHandler::clearState() {
  qDebug() << "Operação cancelada\n";
  m_capsVolume.clear();
  m_costList.clear();
  m_qtdPrepareList.clear();
  m_qtdPrepareUnitList.clear();
  m_qtdBaseList.clear();
  m_baseList.clear();
  m_elementList.clear();
  m_momentQtdBase = 0;
  m_volumeCount = 0;
  cost_excip = 0;
  m_obs.clear();
  m_elementQtdUnitList.clear();
}

/**
 * @brief OrderHandler::getCostExcip
 * @return
 */
float OrderHandler::getCostExcip() { return cost_excip; }

/**
 * @brief setSpecCaps
 * @param specCaps
 */
void OrderHandler::setSpecCaps(QString const &specCaps) {
  qDebug() << "specCaps";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << specCaps;
  m_specCaps = specCaps;
}

/**
 * @brief OrderHandler::specCaps
 * @return
 */
QString OrderHandler::specCaps() const { return m_specCaps; }

/**
 * @brief OrderHandler::setPackage
 * @param package
 */
void OrderHandler::setPackage(QString const &package) {
  qDebug() << "embalagem: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << package;
  m_package = package;
}

/**
 * @brief OrderHandler::package
 * @return
 */
QString OrderHandler::package() const { return m_package; }

/**
 * @brief OrderHandler::setPosologia
 * @param posologia
 */
void OrderHandler::setPosologia(QString const &posologia) {
  qDebug() << "posologia: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << posologia;
  m_posologia = posologia;
}

/**
 * @brief OrderHandler::posologia
 * @return
 */
QString OrderHandler::posologia() const { return m_posologia; }

/**
 * @brief OrderHandler::setObs
 * @param obs
 */
void OrderHandler::setObs(QString const &obs) {
  qDebug() << "observação: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << obs;
  m_obs = obs;
}

/**
 * @brief OrderHandler::obs
 * @return
 */
QString OrderHandler::obs() const { return m_obs; }

/**
 * @brief OrderHandler::setDoctor
 * @param doctor
 */
void OrderHandler::setDoctor(QString const &doctor) {
  qDebug() << "doutor: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << doctor;
  m_doctor = doctor;
}
/**
 * @brief OrderHandler::doctor
 * @return
 */
QString OrderHandler::doctor() const { return m_doctor; }

/**
 * @brief OrderHandler::sum
 * @return
 */
QString OrderHandler::sum() const { return m_sum; }

/**
 * @brief OrderHandler::setSum
 * @param sum
 */
void OrderHandler::setSum(QString const &sum) {
  qDebug() << "addition: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << sum;
  m_sum = sum;
}

/**
 * @brief OrderHandler::sub
 * @return
 */
QString OrderHandler::sub() const { return m_sub; }

/**
 * @brief OrderHandler::setSub
 * @param sub
 */
void OrderHandler::setSub(QString const &sub) {
  qDebug() << "discount: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << sub;
  m_sub = sub;
}

/**
 * @brief OrderHandler::finalCost
 * @return
 */
QString OrderHandler::finalCost() const {
  QString str = QString("%1").arg(m_finalCost);

  qDebug() << "Custo final: " << str;
  return str;
}

/**
 * @brief OrderHandler::setFinalCost
 * @param finalCost
 */
void OrderHandler::setFinalCost(QString finalCost) {
  bool ok;
  finalCost.replace(",", ".");

  float convCost = finalCost.toFloat(&ok);
  QString str;

  str.sprintf("%.02f", convCost);
  m_finalCost = str.toFloat(&ok);

  qDebug() << "custo final: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << finalCost;
}

/**
 * @brief OrderHandler::isDelivery
 * @return
 */
bool OrderHandler::isDelivery() const { return m_isDelivery; }

/**
 * @brief OrderHandler::setIsDelivery
 * @param isDelivery
 */
void OrderHandler::setIsDelivery(bool const &isDelivery) {
  qDebug() << "tele-entrega? : ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << isDelivery;
  m_isDelivery = isDelivery;
}

/**
 * @brief OrderHandler::costDelivery
 * @return
 */
QString OrderHandler::costDelivery() const {
  QString str = QString("%1").arg(m_costDelivery);

  return str;
}

/**
 * @brief OrderHandler::setCostDelivery
 * @param costDelivery
 */
void OrderHandler::setCostDelivery(QString costDelivery) {
  bool ok;
  costDelivery.replace(",", ".");
  m_costDelivery = costDelivery.toFloat(&ok);

  qDebug() << "custo da tele-entrega: ";
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << m_costDelivery;
}

/**
 * @brief OrderHandler::updateComposedToCart
 * @return
 */
bool OrderHandler::updateComposedToCart() {
  int i;
  QString statLine;
  m_composedName.clear();

  m_composedName.append(m_elementList.at(0));
  for (i = 1; i < m_elementList.count(); i++) {
    m_composedName.append(" + ");
    m_composedName.append(m_elementList.at(i));
  }

  qDebug() << "Composed name: " << m_composedName;

  if (m_specCaps.compare("...")) {
    statLine = QString(
                   "UPDATE tb_order SET "
                   "composed_name=\'%1\', "
                   "dosage=\'%2\', "
                   "packing_id_tb_packing=%3, "
                   "cap_spec_id_tb_spec_caps=%4, "
                   "crm_tb_doctors=%5, "
                   "obs=\'%6\', "
                   "addition=%7, "
                   "discount=%8, "
                   "cost=%9, "
                   "use=\'%10\', "
                   "validate_id_tb_validate=%11, "
                   "composed_tb_composition_order=%12 "
                   "WHERE order_id=%13")
                   .arg(m_composedName)
                   .arg(m_posologia)
                   .arg(getPacking(m_package))
                   .arg(getSpecCaps(m_specCaps))
                   .arg(getDoctor(m_doctor))
                   .arg(m_obs)
                   .arg(m_sum)
                   .arg(m_sub)
                   .arg(m_finalCost)
                   .arg(m_usage)
                   .arg(m_validate_id)
                   .arg(m_element_composed_id)
                   .arg(m_order_id);
  } else {
    statLine = QString(
                   "UPDATE tb_order SET "
                   "composed_name=\'%1\', "
                   "dosage=\'%2\', "
                   "packing_id_tb_packing=%3, "
                   "crm_tb_doctors=%4, "
                   "obs=\'%5\', "
                   "addition=%6, "
                   "discount=%7, "
                   "cost=%8, "
                   "use=\'%9\'', "
                   "validate_id_tb_validate=%10, "
                   "composed_tb_composition_order=%11 "
                   "WHERE order_id=%12")
                   .arg(m_composedName)
                   .arg(m_posologia)
                   .arg(getPacking(m_package))
                   .arg(getDoctor(m_doctor))
                   .arg(m_obs)
                   .arg(m_sum)
                   .arg(m_sub)
                   .arg(m_finalCost)
                   .arg(m_usage)
                   .arg(m_validate_id)
                   .arg(m_element_composed_id)
                   .arg(m_order_id);
  }

  qDebug() << "Update Order:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  if (err.isValid()) {
    return false;
  }

  currentPageMap(m_order_index);
  return true;
}

/**
 * @brief OrderHandler::addComposedToCart
 * @param index
 * @return
 */
bool OrderHandler::addComposedToCart(int index) {
  int i;
  QString statLine;
  m_composedName.clear();

  m_composedName.append(m_elementList.at(0));
  for (i = 1; i < m_elementList.count(); i++) {
    m_composedName.append(" + ");
    m_composedName.append(m_elementList.at(i));
  }

  qDebug() << "Composed name: " << m_composedName;

  if (m_specCaps.compare("...")) {
    statLine =
        QString(
            "INSERT INTO tb_order (composed_name, dosage, "
            "packing_id_tb_packing, "
            "cap_spec_id_tb_spec_caps, crm_tb_doctors, obs, "
            "addition, discount, cost, use, validate_id_tb_validate, "
            "composed_tb_composition_order)"
            " VALUES (\'%1\', \'%2\', %3, %4, %5, \'%6\', %7, %8, %9, "
            "\'%10\', %11, %12)")
            .arg(m_composedName)
            .arg(m_posologia)
            .arg(getPacking(m_package))
            .arg(getSpecCaps(m_specCaps))
            .arg(getDoctor(m_doctor))
            .arg(m_obs)
            .arg(m_sum)
            .arg(m_sub)
            .arg(m_finalCost > m_minCostPhorm ? m_finalCost : m_minCostPhorm)
            .arg(m_usage)
            .arg(m_validate_id)
            .arg(m_element_composed_id);
  } else {
    statLine =
        QString(
            "INSERT INTO tb_order (composed_name, dosage, "
            "packing_id_tb_packing, "
            " crm_tb_doctors, obs, "
            "addition, discount, cost, use, validate_id_tb_validate, "
            "composed_tb_composition_order)"
            " VALUES (\'%1\', \'%2\', %3, %4, \'%5\', %6, %7, %8, \'%9\', "
            "%10, %11)")
            .arg(m_composedName)
            .arg(m_posologia)
            .arg(getPacking(m_package))
            .arg(getDoctor(m_doctor))
            .arg(m_obs)
            .arg(m_sum)
            .arg(m_sub)
            .arg(m_finalCost)
            .arg(m_usage)
            .arg(m_validate_id)
            .arg(m_element_composed_id);
  }

  qDebug() << "Insert Composition:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  // Cria uma lista de ordens para a compra atual
  m_order_id_list.append(getOrderIdFromComposed(m_element_composed_id));

  currentPageMap(m_countPage);
  currentLabelMap(m_countlabels);

  m_elementMap.insert(index, static_cast<int>(m_element_composed_id));
  m_element_composed_id++;
  m_countPage++;
  m_countlabels++;

  return true;
}

/**
 * @brief OrderHandler::composedName
 * @return
 */
QString OrderHandler::composedName() {
  qDebug() << "ComposedName: " << m_composedName;
  return m_composedName;
}

/**
 * @brief OrderHandler::client
 * @return
 */
QString OrderHandler::client() const { return m_costumer; }

/**
 * @brief OrderHandler::description
 * @return
 */
QString OrderHandler::description() const { return m_description; }

/**
 * @brief OrderHandler::totalCost
 * @return
 */
float OrderHandler::totalCost() const { return m_totalCost; }

/**
 * @brief OrderHandler::setClient
 * @param client
 */
void OrderHandler::setClient(QString client) {
  m_costumer = client;
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] : " << m_costumer;
}

/**
 * @brief OrderHandler::setDescription
 * @param description
 */
void OrderHandler::setDescription(QString description) {
  m_description = description;
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << m_description;
}

/**
 * @brief OrderHandler::setTotalCost
 * @param totalCost
 */
void OrderHandler::setTotalCost(float totalCost) {
  m_totalCost = totalCost;
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << m_totalCost;
}

/**
 * @brief OrderHandler::orderStatus
 * @return
 */
QString OrderHandler::orderStatus() const { return m_orderStatus; }

/**
 * @brief OrderHandler::setOrderStatus
 * @param orderStatus
 */
void OrderHandler::setOrderStatus(QString orderStatus) {
  m_orderStatus = orderStatus;
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << m_orderStatus;
}

/**
 * @brief OrderHandler::getPreComposedId
 * @param c_name
 * @return
 */
unsigned int OrderHandler::getPreComposedId(const QString &c_name,
                                            unsigned int order_id) {
  unsigned int id;
  bool result;

  QString statLine =
      QString(
          "SELECT order_id FROM tb_order "
          "WHERE composed_name=\'%1\' AND composed_tb_composition_order=%2")
          .arg(c_name)
          .arg(order_id);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Composed order id:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  id = query.value(0).toInt(&result);
  qDebug() << __FUNCTION__ << " [" << __LINE__ << id;

  return id;
}

/**
 * @brief OrderHandler::getPurchaseId
 * @return
 */
unsigned int OrderHandler::getPurchaseId() {
  unsigned int purchase_id;
  bool result;

  QString statLine = QString(
                         "SELECT purchase_id FROM tb_purchase WHERE "
                         "order_id_tb_order=%1")
                         .arg(m_element_composed_id);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Purchase:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  purchase_id = query.value(0).toInt(&result);
  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :" << purchase_id;

  return purchase_id;
}

/**
 * @brief OrderHandler::getDatePurchase
 * @param order_id
 * @return
 */
QDateTime OrderHandler::getDatePurchase(unsigned int purchase_id) {
  QString str;
  QDateTime date;

  QString statLine =
      QString("SELECT order_date FROM tb_purchase WHERE order_id_tb_order=%1")
          .arg(purchase_id);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Purchase:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  str = query.value(0).toString();
  qDebug() << __FUNCTION__ << " [" << __LINE__ << str;

  date = QDateTime::fromString(str, "yyyy-MM-ddTHH:mm:ss");

  qDebug() << __FUNCTION__ << " [" << __LINE__ << date.toLocalTime();

  return date;
}

/**
 * @brief OrderHandler::getPhoneClient
 * @param client
 * @return
 */
QString OrderHandler::getPhoneClient(QString client) {
  QString str;

  QString statLine =
      QString("SELECT phone_1 FROM tb_clients WHERE name=\'%1\'").arg(client);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Phone Client:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  str = query.value(0).toString();
  qDebug() << __FUNCTION__ << " [" << __LINE__ << str;

  return str;
}

/**
 * @brief OrderHandler::getCRM
 * @param doctor
 * @return
 */
QString OrderHandler::getCRM(QString doctor) {
  QString str;

  QString statLine =
      QString("SELECT crm FROM tb_doctors WHERE name=\'%1\'").arg(doctor);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get CRM:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  str = query.value(0).toString();
  qDebug() << __FUNCTION__ << " [" << __LINE__ << str;

  return str;
}

/**
 * @brief OrderHandler::getCostumerId
 * @param name
 * @return
 */
int OrderHandler::getCostumerId(QString name) {
  int id;

  QString statLine =
      QString("SELECT code_client FROM tb_clients WHERE name=\'%1\'").arg(name);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Costumer Id:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms
  query.next();

  id = query.value(0).toInt();
  qDebug() << __FUNCTION__ << " [" << __LINE__ << id;

  return id;
}

/**
 * @brief OrderHandler::finishPurchase
 * @return
 */
bool OrderHandler::finishPurchase() {
  QDateTime date(QDate(2015, 10, 7), QTime(8, 0, 0));

  QString statLine;

  for (int i = 0; i < m_order_id_list.count(); i++) {
    statLine = QString(
                   "INSERT INTO tb_purchase ("
                   "order_status,"
                   "order_obs,"
                   "code_client_tb_clients,"
                   "order_cost,"
                   "delivery,"
                   "cost_delivery,"
                   "data_scheduled,"
                   "order_id_tb_order) VALUES (\'%1\', \'%2\', %3, %4, %5, "
                   "%6, \'%7\', %8)")
                   .arg(orderStatus().toLatin1().at(0))
                   .arg(m_description)
                   .arg(getClient(m_costumer))
                   .arg(m_finalCost)
                   .arg(m_isDelivery ? "true" : "false")
                   .arg(m_costDelivery)
                   .arg(date.toString("yyyy-dd-MM hh:mm:ss"))
                   .arg(m_order_id_list.at(i));

    qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
             << "Create Purchase:\n"
             << statLine;
    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();
    if (err.isValid()) {
      return false;
    }
  }

  m_elementMap.clear();
  return true;
}

/**
 * @brief OrderHandler::remove
 * @param index
 * @return
 */
bool OrderHandler::remove(int index) {
  if (composedDelete(getIdElement(m_elementList.at(index))) != true) {
    return false;
  }

  m_qtdPrepareList.removeAt(index);
  m_qtdPrepareUnitList.removeAt(index);
  m_qtdBaseList.removeAt(index);
  m_elementList.removeAt(index);
  m_elementQtdUnitList.removeAt(index);
  m_costList.removeAt(index);

  return true;
}

/**
 * @brief OrderHandler::update
 * @param index
 * @param phormQtd
 * @param element
 * @param qtd
 * @param unit
 * @return
 */
bool OrderHandler::update(int index, QString phormQtd, QString element,
                          QString qtd, QString unit) {
  bool ok;
  int oldElementId;
  float prepare;
  m_momentQtdPrepare = "";
  m_momentQtdBase = 0;
  m_momentCost = "";

  int phorm_qtd = phormQtd.toInt(&ok);
  float factor = getCorrectionFactor(element);
  m_momentQtdBase = qtd.toFloat(&ok);

  if ((!m_phorm.compare("CÁPSULAS")) || (!m_phorm.compare("SACHÊS"))) {
    float base = unitBaseConvert(unit);
    qDebug() << "[ORDER] calc. base: " << base;

    //#### Calculo de massa a ser manipulado
    // Quantidade de Caps. X Quantidade de Insumo X base X fator de correção
    prepare = phorm_qtd * m_momentQtdBase * base * factor;
    m_momentQtdPrepare.sprintf("%.02f", prepare);
    qDebug() << "[ORDER] qtd. manipule: " << m_momentQtdPrepare << " g";

    // Parse das informacoes
    m_qtdPrepareList.replace(index, prepare);
    m_qtdBaseList.replace(index, m_momentQtdBase);

    oldElementId = getIdElement(m_elementList.at(index));

    m_elementList.replace(index, element);
    m_element_id = getIdElement(element);
    m_elementQtdUnitList.replace(index, unit);

  } else if (!m_phorm.compare("FLORAIS")) {
  } else if ((!m_phorm.compare("GEL")) || (!m_phorm.compare("CREMES")) ||
             (!m_phorm.compare("LOÇÃO")) || (!m_phorm.compare("POMADA")) ||
             (!m_phorm.compare("SABONETE LIQUIDO")) ||
             (!m_phorm.compare("SHAMPOO")) || (!m_phorm.compare("SOLUÇÃO")) ||
             (!m_phorm.compare("TALCO"))) {
    //#### Calculo de massa a ser manipulado
    // Quantidade de Caps. X Quantidade de Insumo X base X fator de correção
    prepare = (phorm_qtd * (m_momentQtdBase / 100)) * factor;
    m_momentQtdPrepare.sprintf("%.02f", prepare);
    qDebug() << "[ORDER] qtd. manipule: " << m_momentQtdPrepare << " g";

    // Parse das informacoes
    m_qtdPrepareList.replace(index, prepare);
    //        m_qtdPrepareUnitList.append("g");
    m_qtdBaseList.replace(index, m_momentQtdBase);

    oldElementId = getIdElement(m_elementList.at(index));

    m_elementList.replace(index, element);
    m_element_id = getIdElement(element);
    m_elementQtdUnitList.replace(index, unit);
  }

  //#### CALCULA O CUSTO
  QString elementUnitStock = getUnitStock(element);
  float baseCost = unitBaseConvert(elementUnitStock);
  float costTemp = getCost(m_element_id);
  costTemp = costTemp / baseCost;
  float factorCostTemp = getCostFactor(m_element_id);
  float cost = costTemp * factorCostTemp * prepare;

  m_costList.replace(index, cost);
  m_momentCost.sprintf("%.02f", cost);
  qDebug() << "R$ " << m_momentCost;

  if (composedUpdate(oldElementId, m_element_id, m_momentQtdBase, unit) !=
      true) {
    return false;
  }

  return true;
}

/**
 * @brief OrderHandler::cancel
 * @return
 */
bool OrderHandler::cancel() {
  QString statLine = QString(
                         "DELETE FROM tb_composition_order "
                         " WHERE composed=%1")
                         .arg(m_element_composed_id);

  qDebug() << "Cancel Composition:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  if (err.isValid()) {
    return false;
  }

  return true;
}

/**
 * @brief OrderHandler::printDocument
 * @param printer
 */
void OrderHandler::printDocument(QPrinter *printer) {
  printer->setFromTo(1, pageMap.count());

  QPainter painter;
  painter.begin(printer);
  bool firstPage = true;

  for (int page = printer->fromPage(); page <= printer->toPage(); page++) {
    if (!firstPage) {
      printer->newPage();
    }

    printPage(page - 1, &painter, printer);
    firstPage = false;
  }

  painter.end();
}

/**
 * @brief OrderHandler::rePrintLabelsDocument
 * @param printer
 */
void OrderHandler::rePrintLabelsDocument(QPrinter *printer) {
  printer->setFromTo(1, 1);

  QPainter painter;
  painter.begin(printer);
  bool firstPage = true;

  for (int page = printer->fromPage(); page <= printer->toPage(); page++) {
    if (!firstPage) {
      printer->newPage();
    }

    rePrintLabel(&painter, printer);
    firstPage = false;
  }

  painter.end();
}

/**
 * @brief OrderHandler::printLabelsDocument
 * @param printer
 */
void OrderHandler::printLabelsDocument(QPrinter *printer) {
  printer->setFromTo(1, labelMap.count());

  QPainter painter;
  painter.begin(printer);
  bool firstPage = true;

  for (int copy = 0; copy < printer->copyCount(); copy++) {
    for (int page = printer->fromPage(); page <= printer->toPage(); page++) {
      if (!firstPage) {
        printer->newPage();
      }

      printLabels(page - 1, &painter, printer);
      firstPage = false;
    }
  }

  painter.end();
}

/**
 * @brief OrderHandler::currentPageMap
 * @return
 */
QMap<int, OrderItems> OrderHandler::currentPageMap(int index) {
  bool ok;
  int i;

  pageMap[index] = OrderItems();

  m_pre_composed_id = getPreComposedId(m_composedName, m_element_composed_id);
  QString header = QString("PHORM CAMBOIM\tORDEM DE MANIPULACAO – Receita:%1")
                       .arg(m_pre_composed_id);

  pageMap[index].append(header);

  QString infoClient =
      QString(" ").arg(m_costumer).arg(getPhoneClient(m_costumer));

  pageMap[index].append(infoClient);

  QString infoDoctor =
      QString("Dr(a).: %1\tC.R.M: %2").arg(m_doctor).arg(getCRM(m_doctor));

  pageMap[index].append(infoDoctor);

  QString infoProduct =
      QString(
          "Produto: %1\t%2 %4 (x%3)\t(%5 caps/dose = %6 caps)\t\tPreco: R$ %7")
          .arg(m_phorm)
          .arg(m_phormQtd)
          .arg(labelPrintCount())
          .arg(m_phormQtdUnit)
          .arg(m_dose)
          .arg(m_dose * m_phormQtd.toInt(&ok))
          .arg(m_finalCost);

  pageMap[index].append(infoProduct);

  QString infoCaps =
      QString("Veiculo/Excipiente: %1	qsp	\tPosologia: %2")
          .arg(m_excipient)
          .arg(m_posologia);

  pageMap[index].append(infoCaps);

  QString line_1 = QString(
      "===================================================================");
  pageMap[index].append(line_1);
  QString header_2 = QString(
      "Nr.	ITEM		Quantidade     "
      "*--Manipula--*  [ Lote Interno ] 	Lembrete");
  pageMap[index].append(header_2);
  QString temp;
  for (i = 0; i < m_elementList.count(); i++) {
    temp = QString("%1	%2\t%3 %4\t %5 %6\t\t%7   %8")
               .arg(i + 1)
               .arg(m_elementList.at(i))
               .arg(m_qtdBaseList.at(i))
               .arg(m_elementQtdUnitList.at(i))
               .arg(m_qtdPrepareList.at(i))
               .arg(m_qtdPrepareUnitList.at(i))
               .arg(getInternalLote(m_elementList.at(i)))
               .arg(getObsElement(m_elementList.at(i)));
    pageMap[index].append(temp);
  }

  if (m_isExcipient == true) {
    temp = QString("%1	%2\t%3 %4\t %5 %6\t\t%7   %8")
               .arg(i + 1)
               .arg(m_excipient)
               .arg("  qsp")
               .arg("")
               .arg(m_excip_bulk)
               .arg(getUnitPrepareFromElement(m_excipient))
               .arg(getInternalLote(m_excipient))
               .arg("");
    pageMap[index].append(temp);
  }

  QString line_3 = QString(
      "---------------------------------------------------"
      "----------------------------"
      "---------------------------------------------------"
      "----------------------------");
  pageMap[index].append(line_3);

  QString observation = QString("Observacoes: %1").arg(m_description);
  pageMap[index].append(observation);

  QString footer1 = QString(
      "Pesagem: ____________________  ___/___/___\t\t "
      "Enchimento: ______________________");
  pageMap[index].append(footer1);

  QString footer2 = QString(
      "Conferencia: _______________________\t\t Controle "
      "de Qualidade: __________________");
  pageMap[index].append(footer2);

  QString footer3 =
      QString("Prometido para [ %1 %2 ]\t\t Retirado em ___/___/___")
          .arg("06/08/2015")
          .arg("14:00");

  pageMap[index].append(footer3);

  return pageMap;
}

/**
 * @brief OrderHandler::currentLabelMap
 * @param index
 * @return
 */
QMap<int, OrderItems> OrderHandler::currentLabelMap(int index) {
  labelMap[index] = OrderItems();

  QDateTime UTC(QDateTime::currentDateTimeUtc());
  QDateTime local(UTC.toLocalTime());
  qDebug() << "UTC time is:" << UTC;
  qDebug() << "Local time is:" << local;

  QString firstLine =
      QString("\tNº Ordem:%2\tFab.: %3")
          .arg(getPreComposedId(m_composedName, m_element_composed_id))
          .arg(local.toString("dd/MM/yyyy"));

  labelMap[index].append(firstLine);

  QString infoDoctor =
      QString("Dr(a).: %1\t\tValidade: %2").arg(m_doctor).arg(validate());

  labelMap[index].append(infoDoctor);
  labelMap[index].append(" ");

  QString line_three;
  for (int i = 0; i < m_elementList.count(); i++) {
    line_three.append(QString("%1 %2 %3")
                          .arg(m_elementList.at(i))
                          .arg(m_qtdBaseList.at(i))
                          .arg(m_elementQtdUnitList.at(i)));

    if ((i + 1) < m_elementList.count()) {
      line_three.append(" + ");
    }
  }

  labelMap[index].append(line_three);

  QString line_four = QString("qsp %1 %2\t\t\t%3")
                          .arg(m_phormQtd)
                          .arg(m_phormQtdUnit)
                          .arg(m_phorm);

  labelMap[index].append(line_four);

  QString line_five = QString("%1\t\t Uso %2")
                          .arg(m_posologia)
                          .arg(m_usage.compare("E") ? "INTERNO" : "EXTERNO");
  labelMap[index].append(line_five);

  return labelMap;
}

/**
 * @brief OrderHandler::printPage
 * @param index
 * @param painter
 * @param printer
 */
void OrderHandler::printPage(int index, QPainter *painter, QPrinter *printer) {
  int order = pageMap.keys()[index];
  OrderItems items = pageMap[order];
  // Find the dimensions of the text on each page.
  qreal width = 0.0;
  qreal height = 0.0;
  //    QString orderString;

  QFont font("Droid Sans", 6);
  font.setStyleName("Regular");
  font = QFont(font, painter->device());
  QFontMetricsF fontMetrics(font);
  QRectF rect = fontMetrics.boundingRect(QString("%1").arg(m_phorm));
  width = qMax(rect.width(), width);
  height += rect.height();

  qreal scale = 1;
  qreal interLineHeight = 300;

  painter->save();
  painter->translate(15, 15);
  painter->scale(scale, scale);
  painter->setBrush(QBrush(Qt::black));

  qreal x = 15;
  qreal y = 50;

  QDateTime date = getDatePurchase(m_order_id_list.at(index));
  QString dateString = QString("\tData: %1 Hora: %2")
                           .arg(date.toString("dd/MM/yyyy"))
                           .arg(date.toString("HH:mm"));

  items[0].append(dateString);
  items[1].insert(0, QString("Cliente: %1\tFone : %2")
                         .arg(m_costumer)
                         .arg(getPhoneClient(m_costumer)));

  QStandardItemModel *model = new QStandardItemModel();

  foreach (QString item, items) {
    if (item.toInt() == 3) {
      model->setHeaderData(0, Qt::Horizontal, tr("Nr."));
      model->setHeaderData(1, Qt::Horizontal, tr("ITEM"));
      model->setHeaderData(2, Qt::Horizontal, tr("Quantidade"));
      model->setHeaderData(3, Qt::Horizontal, tr("*--Manipula--*"));
      model->setHeaderData(4, Qt::Horizontal, tr("[ Lote Interno ]"));
      model->setHeaderData(5, Qt::Horizontal, tr("Lembrete"));

      QStringList list;
      QStandardItem *newItem = 0;
      for (int i = 0; i < m_elementList.count(); i++) {
        list << tr("%1").arg(i + 1) << m_elementList.at(i)
             << tr("%1 %2")
                    .arg(m_qtdBaseList.at(i))
                    .arg(m_elementQtdUnitList.at(i))
             << tr("%1 %2")
                    .arg(m_qtdPrepareList.at(i))
                    .arg(m_qtdPrepareUnitList.at(i))
             << getInternalLote(m_elementList.at(i))
             << getObsElement(m_elementList.at(i));

        for (int col = 0; col < list.length(); ++col) {
          newItem = new QStandardItem(list.at(col));
          model->setItem(i, col, newItem);
        }

        list.clear();
      }

      QTableView *table = new QTableView;
      table->setModel(model);
      table->resizeColumnsToContents();

      const int rows = table->model()->rowCount();
      const int cols = table->model()->columnCount();
      double totalWidth = 0.0;

      for (int c = 0; c < cols; ++c) {
        totalWidth += table->columnWidth(c);
      }

      double totalHeight = table->horizontalHeader()->count();

      for (int r = 0; r < rows; ++r) {
        totalHeight += table->rowHeight(r);
      }
      QPixmap pixmap(totalWidth, totalHeight);
      QPainter::setRedirected(table->horizontalHeader()->viewport(), &pixmap);
      QPainter::setRedirected(table->viewport(), &pixmap);

      painter->drawPixmap(printer->pageRect().topLeft(), pixmap, pixmap.rect());
    }

    QString str(item.data());
    y += interLineHeight;
    painter->drawText(QPointF(x, y), str);
    //        orderString.append(str);
  }

  painter->restore();
}

/**
 * @brief OrderHandler::saveOrderPrint
 * @param orderString
 */
bool OrderHandler::saveOrderPrint(QString orderString) {
  QString statLine = QString("INSERT INTO tb_order_print VALUES (%1, '%2')")
                         .arg(m_pre_composed_id)
                         .arg(orderString);

  qDebug() << "RECEITA:" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  qDebug() << err;
  if (err.isValid()) {
    return false;
  }

  QPrinter pdfPrinter(QPrinter::HighResolution);
  pdfPrinter.setPaperSize(QSizeF(215, 140), QPrinter::Millimeter);
  pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
  pdfPrinter.setOutputFileName(orderString);
  printDocument(&pdfPrinter);

  return true;
}

/**
 * @brief OrderHandler::rePrintLabel
 * @param painter
 * @param printer
 */
void OrderHandler::rePrintLabel(QPainter *painter, QPrinter *printer) {
  painter->drawPixmap(printer->pageRect().topLeft(), m_pdfImage,
                      m_pdfImage.rect());
  painter->restore();
}

/**
 * @brief OrderHandler::printLabels
 * @param index
 * @param painter
 */
void OrderHandler::printLabels(int index, QPainter *painter,
                               QPrinter *printer) {
  int order = labelMap.keys()[index];
  OrderItems items = labelMap[order];
  // Find the dimensions of the text on each page.
  qreal width = 0.0;
  qreal height = 0.0;
  //    QString orderString;

  QFont font("Droid Sans", 4);
  font.setStyleName("Regular");
  font = QFont(font, painter->device());

  foreach (QString item, items) {
    QString str(item.data());
    QFontMetricsF fontMetrics(font);
    QRectF rect = fontMetrics.boundingRect(QString("%1").arg(str));
    width = qMax(rect.width(), width);
    height = printer->pageRect().height();
  }

  qreal scale = 1;

  qreal remainingHeight = printer->pageRect().height() / scale - height;
  qreal spaceHeight = (remainingHeight / 4.0) / (items.count() + 1);

  painter->save();
  painter->translate(printer->pageRect().width() / 2.0,
                     printer->pageRect().height() / 2.0);
  painter->scale(scale, scale);
  painter->setBrush(QBrush(Qt::black));
  painter->setFont(font);

  qreal x = -width / 2.0;
  qreal y = -height / 2.0 - remainingHeight / 4.0 + spaceHeight;

  items[0].insert(0, QString("%1").arg(m_costumer));

  foreach (QString item, items) {
    QString str(item.data());
    QFontMetricsF fontMetrics(font);
    QRectF rect = fontMetrics.boundingRect(QString("%1").arg(str));
    y += rect.height();
    painter->drawText(QPointF(x, y), str);
    //        orderString.append(str);
  }

  painter->restore();
}

/**
 * @brief OrderHandler::saveLabelPrint
 * @param labelString
 * @return
 */
bool OrderHandler::saveLabelPrint(QString pathLabel) {
  QString statLine = QString("INSERT INTO tb_label_print VALUES (%1, '%2')")
                         .arg(m_pre_composed_id)
                         .arg(pathLabel);

  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  qDebug() << err;
  if (err.isValid()) {
    return false;
  }

  QPrinter pdfPrinter(QPrinter::HighResolution);
  pdfPrinter.setPaperSize(QSizeF(108, 24), QPrinter::Millimeter);
  pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
  pdfPrinter.setOutputFileName(pathLabel);
  printLabelsDocument(&pdfPrinter);

  return true;
}

/**
 * @brief OrderHandler::printPreview
 */
void OrderHandler::printPreview(bool print) {
  if (pageMap.count() == 0) {
    return;
  }

  QPrinter printer(QPrinter::HighResolution);
  printer.setPaperSize(QSizeF(215, 140), QPrinter::Millimeter);
  printer.setPrinterSelectionOption(m_orderPrinter);

  qDebug() << m_orderPrinter;

  QPrintPreviewDialog preview(&printer);
  connect(&preview, SIGNAL(paintRequested(QPrinter *)), this,
          SLOT(printDocument(QPrinter *)));

  if (print == true) {
    printDocument(&printer);
  } else {
    preview.exec();
  }

  QString order =
      QString("/home/phormula/ordens/order_%1.pdf").arg(m_pre_composed_id);
  saveOrderPrint(order);

  m_order_id_list.clear();
  m_countPage = 0;
  pageMap.clear();
}

/**
 * @brief OrderHandler::printLabel
 */
void OrderHandler::printLabel(void) {
  if (labelMap.count() == 0) {
    return;
  }

  QPrinter printer(QPrinter::HighResolution);
  printer.setPaperSize(QSizeF(108, 24), QPrinter::Millimeter);
  printer.setPrinterSelectionOption(m_labelsPrinter);
  qDebug() << " ----------------------  QUANTIDADE DE ROTULOS "
           << labelPrintCount();
  printer.setCopyCount(labelPrintCount());

  printLabelsDocument(&printer);
}

/**
 * @brief OrderHandler::loadPrintLabel
 * @param order_id
 * @param count
 * @return
 */
bool OrderHandler::loadPrintLabel(unsigned int order_id, int count) {
  QString statLine =
      QString("SELECT label FROM tb_label_print WHERE order_id_tb_order=%1")
          .arg(order_id);

  QSqlQuery query = m_database.exec(statLine);
  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }
  query.next();

  int labelCol = query.record().indexOf("label");

  QString label = query.value(labelCol).toString();
  qDebug() << "RESULTADO: " << label;

  QString l_ptrCommand =
      QString("lp -d %1 -n %2 %3").arg(m_labelsPrinter).arg(count).arg(label);

  qDebug() << l_ptrCommand;
  system(l_ptrCommand.toLatin1().constData());

  return true;
}

/**
 * @brief OrderHandler::loadPrintOrder
 * @param order_id
 * @return
 */
bool OrderHandler::loadPrintOrder(unsigned int order_id) {
  QString statLine =
      QString("SELECT recipe FROM tb_order_print WHERE order_id_tb_order=%1")
          .arg(order_id);

  QSqlQuery query = m_database.exec(statLine);
  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }
  query.next();

  int labelCol = query.record().indexOf("recipe");

  QString recipe = query.value(labelCol).toString();
  qDebug() << "RESULTADO: " << recipe;

  QString l_ptrCommand = QString("lp -d %1 %2").arg(m_orderPrinter).arg(recipe);

  qDebug() << l_ptrCommand;
  system(l_ptrCommand.toLatin1().constData());

  return true;
}

/**
 * @brief OrderHandler::printlabelPreview
 */
void OrderHandler::printlabelPreview(bool print) {
  if (labelMap.count() == 0) {
    return;
  }

  QPrinter printer(QPrinter::HighResolution);
  printer.setPaperSize(QSizeF(108, 24), QPrinter::Millimeter);
  printer.setPrinterSelectionOption(m_labelsPrinter);
  qDebug() << " ----------------------  QUANTIDADE DE ROTULOS "
           << labelPrintCount();
  printer.setCopyCount(labelPrintCount());

  QPrintPreviewDialog *preview = new QPrintPreviewDialog(&printer);
  connect(preview, SIGNAL(paintRequested(QPrinter *)), this,
          SLOT(printLabelsDocument(QPrinter *)));

  if (print == true) {
    printLabelsDocument(&printer);
  } else {
    preview->exec();
  }

  QString label =
      QString("/home/phormula/rotulos/label_%1.pdf").arg(m_pre_composed_id);
  QString pwdFile(QGuiApplication::applicationDirPath());
  pwdFile.append(label);
  saveLabelPrint(pwdFile);

  m_countlabels = 0;
  labelMap.clear();
}

/**
 * @brief OrderHandler::rePrintLabelPreview
 * @param path
 * @param count
 */
void OrderHandler::rePrintLabelPreview(QString path, int count) {
  m_pdfImage.load(path);

  QPrinter printer(QPrinter::HighResolution);
  printer.setPaperSize(QSizeF(108, 24), QPrinter::Millimeter);
  printer.setPrinterSelectionOption(m_labelsPrinter);
  qDebug() << " ----------------------  QUANTIDADE DE ROTULOS " << count;
  printer.setCopyCount(count);

  QPrintPreviewDialog *preview = new QPrintPreviewDialog(&printer);
  connect(preview, SIGNAL(paintRequested(QPrinter *)), this,
          SLOT(rePrintLabelsDocument(QPrinter *)));

  preview->exec();
}

/**
 * @brief OrderHandler::setLabelPrintCount
 * @param labelPrintCount
 */
void OrderHandler::setLabelPrintCount(int labelPrintCount) {
  qDebug() << "qtd. label print: " << labelPrintCount;
  m_labelcountCopies = labelPrintCount;
}

/**
 * @brief OrderHandler::labelPrintCount
 * @return
 */
int OrderHandler::labelPrintCount() const { return m_labelcountCopies; }

/**
 * @brief OrderHandler::findOrderFromID
 * @param id
 * @return
 */
bool OrderHandler::findOrderFromID(QString id) {
  bool ok;
  int order_id;
  int client_id;
  struct tm tm;
  char buff[50];

  QString statLine =
      QString("SELECT * FROM tb_order WHERE order_id=%1").arg(id);

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  int orderIdCol = query.record().indexOf("order_id");
  int composedCol = query.record().indexOf("composed_name");

  query.next();

  order_id = query.value(orderIdCol).toInt(&ok);
  qDebug() << order_id;
  m_composedName = query.value(composedCol).toString();
  qDebug() << m_composedName;

  statLine = QString("SELECT * FROM tb_purchase WHERE order_id_tb_order=%1")
                 .arg(order_id);

  query = m_database.exec(statLine);
  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  int dateCol = query.record().indexOf("order_date");
  int clientCol = query.record().indexOf("code_client_tb_clients");

  query.next();

  client_id = query.value(clientCol).toInt(&ok);

  strptime(query.value(dateCol).toString().toLatin1().data(),
           "%Y-%m-%dT%H:%M:%S", &tm);
  strftime(buff, sizeof(buff), "%d/%m/%y %H:%M", &tm);
  m_dateTime = buff;

  qDebug() << m_dateTime;

  statLine =
      QString("SELECT * FROM tb_clients WHERE code_client=%1").arg(client_id);

  query = m_database.exec(statLine);
  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  int nameCol = query.record().indexOf("name");
  int phoneCol = query.record().indexOf("phone_1");

  query.next();

  m_costumer = query.value(nameCol).toString();
  m_phone = query.value(phoneCol).toString();

  qDebug() << m_costumer;
  qDebug() << m_phone;

  return true;
}

/**
 * @brief OrderHandler::loadCostumerList
 * @param costumer
 * @return
 */
bool OrderHandler::loadCostumerList(QString costumer) {
  QString statLine =
      QString("SELECT name FROM tb_clients WHERE name LIKE \'%%1\%'")
          .arg(costumer);

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  qDebug() << query.size();

  if (query.size() <= 0) {
    return false;
  }

  int nameCol = query.record().indexOf("name");

  m_costumerCount = query.size();

  m_costumerList.clear();
  while (query.next()) {
    m_costumerList.append(query.value(nameCol).toString());
  }

  return true;
}

/**
 * @brief OrderHandler::costumerListAt
 * @param index
 * @return
 */
QString OrderHandler::costumerListAt(int index) {
  return m_costumerList.at(index);
}

/**
 * @brief OrderHandler::findOrderFromCostumer
 * @param costumer
 * @return
 */
bool OrderHandler::findOrderFromCostumer(QString costumer) {
  bool ok;
  int costumer_id;
  QList<int> order_id_list;
  struct tm tm;
  QString datetime;
  char buff[50];

  QString statLine =
      QString("SELECT * FROM tb_clients WHERE name=\'%1\'").arg(costumer);

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  int costumerIdCol = query.record().indexOf("code_client");
  int phoneCol = query.record().indexOf("phone_1");

  query.next();

  m_costumer = costumer;
  costumer_id = query.value(costumerIdCol).toInt(&ok);
  m_phone = query.value(phoneCol).toString();

  statLine =
      QString("SELECT * FROM tb_purchase WHERE code_client_tb_clients=%1")
          .arg(costumer_id);

  query = m_database.exec(statLine);

  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  m_costumerCount = query.size();
  int dateCol = query.record().indexOf("order_date");
  int orderIdCol = query.record().indexOf("order_id_tb_order");

  order_id_list.clear();
  m_dateTimeList.clear();

  while (query.next()) {
    strptime(query.value(dateCol).toString().toLatin1().data(),
             "%Y-%m-%dT%H:%M:%S", &tm);
    strftime(buff, sizeof(buff), "%d/%m/%y %H:%M", &tm);
    datetime = buff;

    m_dateTimeList.append(datetime);
    order_id_list.append(query.value(orderIdCol).toInt(&ok));
  }

  m_composedNameList.clear();

  for (int i = 0; i < m_costumerCount; i++) {
    statLine = QString("SELECT composed_name FROM tb_order WHERE order_id=%1")
                   .arg(order_id_list.at(i));

    query = m_database.exec(statLine);
    query.next();

    qDebug() << query.value(0).toString();
    m_composedNameList.append(query.value(0).toString());
  }

  return true;
}

/**
 * @brief OrderHandler::findOrderFromPhone
 * @param phone
 * @return
 */
bool OrderHandler::findOrderFromPhone(QString phone) {
  bool ok;
  int costumer_id;
  QList<int> order_id_list;
  struct tm tm;
  QString datetime;
  char buff[50];

  QString statLine =
      QString("SELECT * FROM tb_clients WHERE phone_1=%1").arg(phone);

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  int costumerIdCol = query.record().indexOf("code_client");
  int nameCol = query.record().indexOf("name");

  query.next();

  m_phone = phone;
  costumer_id = query.value(costumerIdCol).toInt(&ok);
  m_costumer = query.value(nameCol).toString();

  statLine =
      QString("SELECT * FROM tb_purchase WHERE code_client_tb_clients=%1")
          .arg(costumer_id);

  query = m_database.exec(statLine);

  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }

  m_costumerCount = query.size();
  int dateCol = query.record().indexOf("order_date");
  int orderIdCol = query.record().indexOf("order_id_tb_order");

  order_id_list.clear();
  m_dateTimeList.clear();

  while (query.next()) {
    strptime(query.value(dateCol).toString().toLatin1().data(),
             "%Y-%m-%dT%H:%M:%S", &tm);
    strftime(buff, sizeof(buff), "%d/%m/%y %H:%M", &tm);
    datetime = buff;

    m_dateTimeList.append(datetime);
    order_id_list.append(query.value(orderIdCol).toInt(&ok));
  }

  m_composedNameList.clear();

  for (int i = 0; i < m_costumerCount; i++) {
    statLine = QString("SELECT composed_name FROM tb_order WHERE order_id=%1")
                   .arg(order_id_list.at(i));

    query = m_database.exec(statLine);
    query.next();

    qDebug() << query.value(0).toString();
    m_composedNameList.append(query.value(0).toString());
  }

  return true;
}

/**
 * @brief OrderHandler::findOrderFromDate
 * @param date
 * @return
 */
bool OrderHandler::findOrderFromDate(QString date) {
  bool ok;
  int nameCol;
  int phoneCol;
  QList<int> order_id_list;
  QList<int> code_client_list;
  struct tm tm;
  char buff[50];

  m_costumerList.clear();
  m_phoneList.clear();
  m_composedNameList.clear();

  QDateTime endDate(QDateTime::fromString(date, "dd/MM/yy"));
  QString statLine =
      QString(
          "SELECT * FROM tb_purchase WHERE order_date BETWEEN"
          " \'%1\' AND \'%2\'")
          .arg(date)
          .arg(endDate.date().addYears(100).addDays(1).toString("dd/MM/yy"));

  qDebug() << statLine;
  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  if (query.size() <= 0) {
    return false;
  }
  m_dateTimeListCount = query.size();

  int costumerIdCol = query.record().indexOf("code_client_tb_clients");
  int orderIdCol = query.record().indexOf("order_id_tb_order");
  int dateCol = query.record().indexOf("order_date");

  while (query.next()) {
    code_client_list.append(query.value(costumerIdCol).toInt(&ok));
    order_id_list.append(query.value(orderIdCol).toInt(&ok));

    strptime(query.value(dateCol).toString().toLatin1().data(),
             "%Y-%m-%dT%H:%M:%S", &tm);
    strftime(buff, sizeof(buff), "%d/%m/%y %H:%M", &tm);
    m_dateTime = buff;
  }

  for (int i = 0; i < m_dateTimeListCount; i++) {
    statLine = QString("SELECT composed_name FROM tb_order WHERE order_id=%1")
                   .arg(order_id_list.at(i));

    qDebug() << statLine;

    query = m_database.exec(statLine);

    err = m_database.lastError();
    if (err.isValid()) {
      return false;
    }

    query.next();

    m_composedNameList.append(query.value(0).toString());
    qDebug() << query.value(0).toString();

    statLine = QString("SELECT * FROM tb_clients WHERE code_client=%1")
                   .arg(code_client_list.at(i));

    query = m_database.exec(statLine);

    err = m_database.lastError();
    if (err.isValid()) {
      return false;
    }

    nameCol = query.record().indexOf("name");
    phoneCol = query.record().indexOf("phone_1");

    query.next();

    m_costumerList.append(query.value(nameCol).toString());
    qDebug() << query.value(nameCol).toString();
    m_phoneList.append(query.value(phoneCol).toString());
    qDebug() << query.value(phoneCol).toString();
  }

  return true;
}

/**
 * @brief OrderHandler::setNextComposedID
 */
void OrderHandler::setNextComposedID(void) {
  m_element_composed_id = 0;

  QString statLine = QString("SELECT MAX (composed) FROM tb_composition_order");

  QSqlQuery query = m_database.exec(statLine);
  // Number of columms

  query.next();

  m_element_composed_id = query.value(0).toInt();
  m_element_composed_id++;
}

/**
 * @brief OrderHandler::costumerFound
 * @return
 */
QString OrderHandler::costumerFound(void) { return m_costumer; }

/**
 * @brief OrderHandler::phoneFound
 * @return
 */
QString OrderHandler::phoneFound(void) { return m_phone; }

/**
 * @brief OrderHandler::dateTimeFound
 * @return
 */
QString OrderHandler::dateTimeFound(void) { return m_dateTime; }

/**
 * @brief OrderHandler::composedFound
 * @return
 */
QString OrderHandler::composedFound(void) { return m_composedName; }

/**
 * @brief OrderHandler::orderListCount
 * @return
 */
int OrderHandler::orderListCount() { return m_composedCount; }

/**
 * @brief OrderHandler::costumerListCount
 * @return
 */
int OrderHandler::costumerListCount() { return m_costumerCount; }

/**
 * @brief OrderHandler::phoneListCount
 * @return
 */
int OrderHandler::phoneListCount() { return m_dateTimeListCount; }

/**
 * @brief OrderHandler::phoneListAt
 * @param index
 * @return
 */
QString OrderHandler::phoneListAt(int index) {
  if (m_dateTimeListCount < 0) {
    return QString("");
  }

  return m_composedNameList.at(index);
}

/**
 * @brief OrderHandler::orderListAt
 * @param index
 * @return
 */
QString OrderHandler::orderListAt(int index) {
  if (m_composedCount < 0) {
    return QString("");
  }

  return m_composedNameList.at(index);
}

/**
 * @brief OrderHandler::datetimeListAt
 * @param index
 * @return
 */
QString OrderHandler::datetimeListAt(int index) {
  if (m_costumerCount < 0) {
    return QString("");
  }

  return m_dateTimeList.at(index);
}

/**
 * @brief OrderHandler::getOrderIdFromComposed
 * @param composed_id
 * @return
 */
int OrderHandler::getOrderIdFromComposed(int composed_id) {
  bool ok;
  int order_id;

  QString statLine = QString(
                         "SELECT order_id FROM tb_order WHERE "
                         "composed_tb_composition_order=%1")
                         .arg(composed_id);

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Get Orders id:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return 0;
  }

  query.next();
  order_id = query.value(0).toInt(&ok);

  return order_id;
}

/**
 * @brief OrderHandler::editPurchase
 * @param composed
 * @param count
 * @return
 */
bool OrderHandler::editPurchase(int index, QString composed, int count) {
  QString statLine;
  QSqlQuery query;
  QSqlError err;
  int composed_id = m_element_composed_id;
  qDebug() << "####### EDIT PURCHASE ########";
  qDebug() << "Qtd.Itens " << count;
  m_order_index = m_elementMap.value(index);
  for (int i = 0; i < count; i++) {
    statLine =
        QString(
            "SELECT * FROM tb_order "
            "WHERE composed_tb_composition_order = %1 AND composed_name=\'%2\'")
            .arg(composed_id)
            .arg(composed);

    qDebug() << "[" << __FUNCTION__ << " : " << __LINE__ << "] " << statLine;
    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
      return false;
    }

    if (query.size() <= 0) {
      composed_id--;
      continue;
    }

    int orderIdCol = query.record().indexOf("order_id");
    int dosageCol = query.record().indexOf("dosage");
    int composedCol = query.record().indexOf("composed_name");
    int crmCol = query.record().indexOf("crm_tb_doctors");
    int obsCol = query.record().indexOf("obs");
    int additionCol = query.record().indexOf("addition");
    int discountCol = query.record().indexOf("discount");
    int useCol = query.record().indexOf("use");
    int validateCol = query.record().indexOf("validate_id_tb_validate");
    int compositionOrderCol =
        query.record().indexOf("composed_tb_composition_order");

    while (query.next()) {
      if (!composed.compare(query.value(composedCol).toString())) {
        m_order_id = query.value(orderIdCol).toInt();
        setPosologia(query.value(dosageCol).toString());
        setDoctor(getDoctorFromCRM(query.value(crmCol).toInt()));
        setSum(query.value(additionCol).toString());
        setSub(query.value(discountCol).toString());
        setObs(query.value(obsCol).toString());
        if (!query.value(useCol).toString().compare("I")) {
          setUsage("INTERNO");
        } else {
          setUsage("EXTERNO");
        }
        setValidate(getValidateDesc(query.value(validateCol).toInt()));
        m_element_composed_id = query.value(compositionOrderCol).toInt();
        qDebug() << "composition Id " << composed_id;
        goto resumeEdit;
      }
    }
  }

resumeEdit:

  statLine = QString(
                 "SELECT * FROM tb_composition_order "
                 "WHERE composed = %1 ")
                 .arg(m_element_composed_id);

  m_queryRecord = m_database.exec(statLine);
  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  m_composition_count = m_queryRecord.size();

  qDebug() << "Order selected is " << m_order_id;

  return true;
}

/**
 * @brief OrderHandler::loadPurchase
 * @param composed
 * @param costumer
 * @param datetime
 * @return
 */
bool OrderHandler::loadPurchase(QString composed, QString costumer,
                                QString datetime) {
  struct tm tm;
  QList<int> order_id_list;

  strptime(datetime.toLatin1().data(), "%d/%m/%y %H:%M", &tm);

  QDateTime start_date_time(QDate(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday),
                            QTime(tm.tm_hour, tm.tm_min));
  QDateTime end_date_time(start_date_time.addSecs(60));

  qDebug() << "START" << start_date_time;
  qDebug() << "END" << end_date_time;

  QString statLine = QString(
                         "SELECT * FROM tb_purchase "
                         "WHERE code_client_tb_clients = %1 "
                         "AND order_date >= \'%2\'::timestamp "
                         "AND order_date <= \'%3\'::timestamp")
                         .arg(getCostumerId(costumer))
                         .arg(start_date_time.toString("dd/MM/yy HH:mm"))
                         .arg(end_date_time.toString("dd/MM/yy HH:mm"));

  qDebug() << __FUNCTION__ << " [" << __LINE__ << "] :"
           << "Load purchase:\n"
           << statLine;

  QSqlQuery query = m_database.exec(statLine);

  QSqlError err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  int count = query.size();
  int orderIdCol = query.record().indexOf("order_id_tb_order");
  int orderObsCol = query.record().indexOf("order_obs");
  while (query.next()) {
    setDescription(query.value(orderObsCol).toString());
    order_id_list.append(query.value(orderIdCol).toInt());
  }

  setClient(costumer);

  for (int i = 0; i < count; i++) {
    statLine = QString(
                   "SELECT * FROM tb_order "
                   "WHERE order_id = %1 ")
                   .arg(order_id_list.at(i));

    query = m_database.exec(statLine);
    err = m_database.lastError();
    if (err.isValid()) {
      return false;
    }

    int dosageCol = query.record().indexOf("dosage");
    int composedCol = query.record().indexOf("composed_name");
    int crmCol = query.record().indexOf("crm_tb_doctors");
    int obsCol = query.record().indexOf("obs");
    int additionCol = query.record().indexOf("addition");
    int discountCol = query.record().indexOf("discount");
    int useCol = query.record().indexOf("use");
    int validateCol = query.record().indexOf("validade_id_tb_validate");
    int compositionOrderCol =
        query.record().indexOf("composed_tb_composition_order");

    while (query.next()) {
      if (!composed.compare(query.value(composedCol).toString())) {
        m_order_id = order_id_list.at(i);
        setPosologia(query.value(dosageCol).toString());
        setDoctor(getDoctorFromCRM(query.value(crmCol).toInt()));
        setSum(query.value(additionCol).toString());
        setSub(query.value(discountCol).toString());
        setObs(query.value(obsCol).toString());
        if (!query.value(useCol).toString().compare("I")) {
          setUsage("INTERNO");
        } else {
          setUsage("EXTERNO");
        }
        setValidate(getValidateDesc(query.value(validateCol).toInt()));
        m_element_composed_id = query.value(compositionOrderCol).toInt();
        qDebug() << "composition Id " << m_element_composed_id;
        goto resume;
      }
    }
  }

resume:

  statLine = QString(
                 "SELECT * FROM tb_composition_order "
                 "WHERE composed = %1 ")
                 .arg(m_element_composed_id);

  m_queryRecord = m_database.exec(statLine);
  err = m_database.lastError();
  if (err.isValid()) {
    return false;
  }

  m_composition_count = m_queryRecord.size();

  qDebug() << "Order selected is " << m_order_id;

  return true;
}

/**
 * @brief OrderHandler::itemCancelPurchase
 * @param index
 * @return
 */
bool OrderHandler::itemCancelPurchase(int index) {
  loadOrderCost(m_order_id_list.at(index));

  QString statLine = QString("DELETE FROM tb_order WHERE order_id=%1")
                         .arg(m_order_id_list.at(index));

  qDebug() << "DELETE ORDER Stmt:\n" << statLine;
  m_database.exec(statLine);

  QSqlError err = m_database.lastError();

  qDebug() << err.text();
  if (err.isValid()) {
    return false;
  }

  m_order_id_list.removeAt(index);
  return true;
}

/**
 * @brief OrderHandler::cancelPurchase
 */
void OrderHandler::cancelPurchase() {
  QString statLine;

  for (int i = 0; i < m_order_id_list.count(); i++) {
    statLine = QString("DELETE FROM tb_order WHERE order_id=%1")
                   .arg(m_order_id_list.at(i));

    qDebug() << "DELETE ORDER Stmt:\n" << statLine;
    m_database.exec(statLine);
  }
}

/**
 * @brief OrderHandler::compositionCount
 * @return
 */
int OrderHandler::compositionCount(void) { return m_composition_count; }

/**
 * @brief OrderHandler::fillOrderFromComposition
 */
bool OrderHandler::fillOrderFromComposition() {
  int phormIdCol = m_queryRecord.record().indexOf("phorm_id_tb_pharma_phorm");
  int qtdPhormCol = m_queryRecord.record().indexOf("qtd_phorm");
  int qtdPhormUnitCol = m_queryRecord.record().indexOf("qtd_phorm_unit");
  int codeElementCol =
      m_queryRecord.record().indexOf("code_element_tb_element_base");
  int qtdElementCol = m_queryRecord.record().indexOf("qtd_element");
  int qtdElementUnitCol = m_queryRecord.record().indexOf("qtd_element_unit");
  int excipCol = m_queryRecord.record().indexOf("element_id_tb_excipient");

  if (m_queryRecord.next() == false) {
    return false;
  }

  setPhorm(getPhormFromId(m_queryRecord.value(phormIdCol).toInt()));
  setPhormQtd(m_queryRecord.value(qtdPhormCol).toString());
  setPhormQtdUnit(getUnitFromId(m_queryRecord.value(qtdPhormUnitCol).toInt()));
  setExcip(getExcipientName(m_queryRecord.value(excipCol).toInt()));

  setElement(getElementFromId(m_queryRecord.value(codeElementCol).toInt()));
  setElementQtd(m_queryRecord.value(qtdElementCol).toString());
  setElementQtdUnit(
      getUnitFromId(m_queryRecord.value(qtdElementUnitCol).toInt()));

  return true;
}

/**
 * @brief OrderHandler::getOrderId
 * @return
 */
unsigned int OrderHandler::getOrderId() { return m_order_id; }

/**
 * @brief OrderHandler::getCurrentDate
 * @return
 */
QString OrderHandler::getCurrentDate(void) {
  QDate date;

  return date.currentDate().toString("dd/MM/yy");
}
