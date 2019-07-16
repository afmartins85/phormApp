#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include <QDateTime>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QtQuick/QQuickItem>

typedef QList<QString> OrderItems;
class QPdfWidget;

typedef QMap<int, int> CElementMap;

class OrderHandler : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QString phorm READ phorm WRITE setPhorm)
  Q_PROPERTY(QString phormQtd READ phormQtd WRITE setPhormQtd)
  Q_PROPERTY(QString phormQtdUnit READ phormQtdUnit WRITE setPhormQtdUnit)
  Q_PROPERTY(QString element READ element WRITE setElement)
  Q_PROPERTY(QString elementQtd READ elementQtd WRITE setElementQtd)
  Q_PROPERTY(QString elementQtdUnit READ elementQtdUnit WRITE setElementQtdUnit)
  Q_PROPERTY(QString excip READ excip WRITE setExcip)
  Q_PROPERTY(QString validate READ validate WRITE setValidate)
  Q_PROPERTY(QString usage READ usage WRITE setUsage)

  Q_PROPERTY(QString specCaps READ specCaps WRITE setSpecCaps)
  Q_PROPERTY(QString package READ package WRITE setPackage)
  Q_PROPERTY(QString posologia READ posologia WRITE setPosologia)
  Q_PROPERTY(QString obs READ obs WRITE setObs)
  Q_PROPERTY(QString doctor READ doctor WRITE setDoctor)
  Q_PROPERTY(QString sum READ sum WRITE setSum)
  Q_PROPERTY(QString sub READ sub WRITE setSub)
  Q_PROPERTY(QString finalCost READ finalCost WRITE setFinalCost)
  Q_PROPERTY(bool isDelivery READ isDelivery WRITE setIsDelivery)
  Q_PROPERTY(QString costDelivery READ costDelivery WRITE setCostDelivery)

  Q_PROPERTY(QString client READ client WRITE setClient)
  Q_PROPERTY(QString description READ description WRITE setDescription)
  Q_PROPERTY(float totalCost READ totalCost WRITE setTotalCost)
  Q_PROPERTY(QString orderStatus READ orderStatus WRITE setOrderStatus)

  Q_PROPERTY(int labelPrintCount READ labelPrintCount WRITE setLabelPrintCount)

 public:
  OrderHandler(QQuickItem *parent = 0);
  ~OrderHandler();

  void setDatabase(const QSqlDatabase &db);

  QString phorm() const;
  void setPhorm(const QString &phorm);

  QString phormQtd() const;
  void setPhormQtd(const QString &phormQtd);

  QString phormQtdUnit() const;
  void setPhormQtdUnit(const QString &phormQtdUnit);

  QString element() const;
  void setElement(const QString &element);
  Q_INVOKABLE QString elementAt(int index);

  QString elementQtd() const;
  void setElementQtd(const QString &elementQtd);
  Q_INVOKABLE QString elementQtdAt(int index);

  QString elementQtdUnit() const;
  void setElementQtdUnit(const QString &elementQtdUnit);
  Q_INVOKABLE QString getElementQtdUnit(int index);

  QString excip();
  void setExcip(const QString &excip);

  QString validate();
  void setValidate(const QString &validate);

  QString usage();
  void setUsage(QString usage);

  Q_INVOKABLE bool endPurchaseProcess();

  int getIdPhorm(const QString &phorm);
  QString getPhormFromId(const int id);
  int getIdElement(const QString &element);
  QString getElementFromId(const int id);
  QString getUnitPrepareFromElement(QString const &element);
  int getIdUnit(const QString &unitBaseConvert);
  QString getUnitFromId(const int id);
  void loadCapsSpecification(int phorm_id);

  int getIdExcipient(const QString &excipient);
  QString getExcipientName(int id);

  int getIdValidate(const QString &validate);
  QString getValidateDesc(int id);

  char getUsageDesc(int id);

  float unitBaseConvert(QString unit);
  float getDensity(const QString &element);
  float getCorrectionFactor(const QString &element);
  float getCostFactor(int id);
  float getCost(int id);
  QString getUnitStock(const QString &element);
  unsigned int getDoctor(const QString &doctor);
  QString getDoctorFromCRM(const int crm);
  unsigned int getSpecCaps(const QString &spec);
  unsigned int getPacking(const QString &packing);
  unsigned int getClient(const QString &client);
  QString getInternalLote(const QString &element);

  Q_INVOKABLE bool addElementToCart(bool isRecording);
  bool composedInsert();
  bool composedDelete(int elementId);
  bool composedUpdate(int oldElement, int newElement, int qtdElement,
                      QString unit);
  QString getObsElement(QString element);

  Q_INVOKABLE QString qtdPrepare(void);
  Q_INVOKABLE float qtdBase(void);
  Q_INVOKABLE QString cost(void);
  Q_INVOKABLE float costListAt(int index);
  Q_INVOKABLE QString getCapsSpecDetected();

  Q_INVOKABLE void clearState(void);
  Q_INVOKABLE float getCostExcip(void);

  QString specCaps() const;
  void setSpecCaps(const QString &specCaps);
  void setPackage(const QString &package);
  QString package() const;
  void setPosologia(const QString &posologia);
  QString posologia() const;
  void setObs(const QString &obs);
  QString obs() const;
  void setDoctor(const QString &doctor);
  QString doctor() const;

  QString sum() const;
  void setSum(const QString &sum);
  QString sub() const;
  void setSub(const QString &sub);
  QString finalCost() const;
  void setFinalCost(QString finalCost);
  bool isDelivery() const;
  void setIsDelivery(const bool &isDelivery);
  QString costDelivery() const;
  void setCostDelivery(QString costDelivery);

  Q_INVOKABLE bool updateComposedToCart();
  Q_INVOKABLE bool addComposedToCart(int index);
  Q_INVOKABLE QString composedName();

  Q_INVOKABLE bool finishPurchase();

  Q_INVOKABLE void printPreview(bool print);
  Q_INVOKABLE void printlabelPreview(bool print);
  void rePrintLabelPreview(QString path, int count);
  Q_INVOKABLE void printLabel(void);
  Q_INVOKABLE bool loadPrintLabel(unsigned int order_id, int count);
  Q_INVOKABLE bool loadPrintOrder(unsigned int order_id);

  QString client() const;
  QString description() const;
  float totalCost() const;
  QString orderStatus() const;
  void setClient(QString client);
  void setDescription(QString description);
  void setTotalCost(float totalCost);
  void setOrderStatus(QString orderStatus);
  unsigned int getPurchaseId(void);
  unsigned int getPreComposedId(const QString &c_name, unsigned int order_id);
  QDateTime getDatePurchase(unsigned int purchase_id);
  QString getPhoneClient(QString client);
  QString getCRM(QString doctor);
  int getCostumerId(QString name);

  Q_INVOKABLE bool remove(int index);
  Q_INVOKABLE bool update(int index, QString phormQtd, QString element,
                          QString qtd, QString unit);
  Q_INVOKABLE bool cancel(void);

  Q_INVOKABLE bool findOrderFromID(QString id);
  Q_INVOKABLE bool findOrderFromCostumer(QString costumer);
  Q_INVOKABLE bool findOrderFromPhone(QString phone);
  Q_INVOKABLE bool findOrderFromDate(QString date);

  Q_INVOKABLE bool loadCostumerList(QString costumer);
  Q_INVOKABLE QString costumerListAt(int index);
  Q_INVOKABLE int orderListCount();
  Q_INVOKABLE int costumerListCount();
  Q_INVOKABLE int phoneListCount();

  Q_INVOKABLE QString datetimeListAt(int index);
  Q_INVOKABLE QString orderListAt(int index);
  Q_INVOKABLE QString phoneListAt(int index);
  int getOrderIdFromComposed(int composed_id);

  Q_INVOKABLE QString costumerFound();
  Q_INVOKABLE QString phoneFound();
  Q_INVOKABLE QString dateTimeFound();
  Q_INVOKABLE QString composedFound();
  Q_INVOKABLE void setNextComposedID();

  Q_INVOKABLE bool editPurchase(int index, QString composed, int count);
  Q_INVOKABLE bool loadPurchase(QString composed, QString costumer,
                                QString datetime);
  Q_INVOKABLE bool itemCancelPurchase(int index);
  Q_INVOKABLE void cancelPurchase(void);
  Q_INVOKABLE int compositionCount();
  Q_INVOKABLE bool fillOrderFromComposition();

  Q_INVOKABLE unsigned int getOrderId(void);

  int labelPrintCount() const;
  void setLabelPrintCount(int labelPrintCount);

  Q_INVOKABLE QString getCurrentDate();
  Q_INVOKABLE void loadPrintConfig();

  bool saveOrderPrint(QString orderString);
  bool saveLabelPrint(QString pathLabel);

  void loadOrderCost(int index);
  Q_INVOKABLE float getOrderCost();
  Q_INVOKABLE float removeExcip();

  bool setMinPrice(const QString &phorm);
  Q_INVOKABLE float getMinPrice(void);

 public slots:
  void printDocument(QPrinter *printer);
  void printLabelsDocument(QPrinter *printer);
  void rePrintLabelsDocument(QPrinter *printer);
  void printPage(int index, QPainter *painter, QPrinter *printer);
  void printLabels(int index, QPainter *painter, QPrinter *printer);
  void rePrintLabel(QPainter *painter, QPrinter *printer);

 private:
  QMap<int, OrderItems> currentPageMap(int index);
  QMap<int, OrderItems> currentLabelMap(int index);
  CElementMap m_elementMap;

  QSqlDatabase m_database;
  QPrinter *m_printer;
  QSqlQuery m_queryRecord;

  int m_composition_count;

  unsigned int m_element_composed_id;
  unsigned int m_pre_composed_id;
  unsigned int m_phorm_id;
  QString m_phorm;
  QString m_phormQtd;
  QString m_phormQtdUnit;
  int m_dose;
  unsigned int m_element_id;
  QString m_element;
  QString m_elementQtd;
  QString m_elementQtdUnit;
  QList<QString> m_elementQtdUnitList;
  QString m_validate;
  QString m_capsSpecDetected;
  int m_validate_id;

  QString m_excipient;
  unsigned int m_excipient_id;
  float cost_excip;
  bool m_isExcipient;
  float m_excip_bulk;

  int m_volumeCount;
  QList<float> m_capsVolume;
  QStringList m_capsSpecName;

  QList<float> m_costList;
  QList<float> m_qtdPrepareList;
  QList<QString> m_qtdPrepareUnitList;
  QList<float> m_qtdBaseList;
  QList<int> m_baseList;
  QList<QString> m_elementList;

  QString m_momentCost;
  QString m_momentQtdPrepare;
  float m_momentQtdBase;
  QString m_usage;

  QString m_specCaps;
  QString m_package;
  QString m_posologia;
  QString m_obs;
  QString m_doctor;
  QString m_sum;
  QString m_sub;
  float m_finalCost;
  bool m_isDelivery;
  float m_costDelivery;
  float m_orderCost;

  QString m_costumer;
  QList<QString> m_costumerList;
  QString m_description;
  float m_totalCost;
  QString m_orderStatus;

  QString m_composedName;
  QList<QString> m_composedNameList;
  int m_composedCount;
  int m_costumerCount;

  QList<int> m_order_id_list;
  unsigned int m_order_id;
  unsigned int m_order_index;

  QString m_dateTime;
  QList<QString> m_dateTimeList;
  int m_dateTimeListCount;

  QString m_phone;
  QList<QString> m_phoneList;

  QMap<int, OrderItems> pageMap;
  QMap<int, OrderItems> labelMap;
  int m_countPage;
  int m_countlabels;
  int m_labelcountCopies;
  QString m_labelsPrinter;
  QString m_orderPrinter;

  QPixmap m_pdfImage;

  float m_minCostPhorm;
};

#endif  // ORDERHANDLER_H
