#ifndef MAINTENANCEHANDLER_H
#define MAINTENANCEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtQuick/QQuickItem>

class MaintenanceHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString phormInsert READ phormInsert WRITE setPhormInsert)
    Q_PROPERTY(QString phormId READ phormId WRITE setPhormId)
    Q_PROPERTY(QString unitInsert READ unitInsert WRITE setUnitInsert)
    Q_PROPERTY(QQmlListProperty<QString> phorms READ phorms)
    Q_PROPERTY(QString group READ group WRITE setGroup)
    Q_PROPERTY(QString groupId READ groupId WRITE setGroupId)
    Q_PROPERTY(QString subGroup READ subGroup WRITE setSubGroup)
    Q_PROPERTY(QString validate READ validate WRITE setValidate)
    Q_PROPERTY(QString excipient READ excipient WRITE setExcipient)
    Q_PROPERTY(QString specCapsName READ specCapsName WRITE setSpecCapsName)
    Q_PROPERTY(QString capsCapacity READ capsCapacity WRITE setCapsCapacity)
    Q_PROPERTY(QString capsVolume READ capsVolume WRITE setCapsVolume)
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName)
    Q_PROPERTY(QString VolumePack READ VolumePack WRITE seVolumePack)
    Q_PROPERTY(int labelSize READ labelSize WRITE setLabelSize)
    Q_PROPERTY(QString packId READ packId WRITE setPackId)
    Q_PROPERTY(QString capsForPack READ capsForPack WRITE setCapsForPack)
    Q_PROPERTY(int qtdCaps READ qtdCaps WRITE setQtdCaps)
    Q_PROPERTY(QString priceUp READ priceUp WRITE setPriceUp)
    Q_PROPERTY(QString priceItemInit READ priceItemInit WRITE setPriceItemInit)
    Q_PROPERTY(QString priceItemEnd READ priceItemEnd WRITE setPriceItemEnd)
    Q_PROPERTY(QString costManipulation READ costManipulation WRITE setCostManipulation)
    Q_PROPERTY(QString profitFactor READ profitFactor WRITE setProfitFactor)
    Q_PROPERTY(QString minPrice READ minPrice WRITE setMinPrice)
    Q_PROPERTY(QString priceUPInsert READ priceUPInsert WRITE setPriceUPInsert)
    Q_PROPERTY(QString packFactor READ packFactor WRITE setPackFactor)
    Q_PROPERTY(QString costPack READ costPack WRITE setCostPack)
    Q_PROPERTY(QString packPriceUPInsert READ packPriceUPInsert WRITE setPackPriceUPInsert)

public:
    explicit MaintenanceHandler(QQuickItem *parent = 0);
    ~MaintenanceHandler();

    void setDatabase(const QSqlDatabase &db);
    bool isOpenDB ();

    //Fórmulas farmaceuticas
    bool setPhormInsert(QString const &phorm);
    QString phormInsert() const;
    QQmlListProperty<QString> phorms();
    Q_INVOKABLE void loadPhormList ();
    Q_INVOKABLE int phormListCount ();
    Q_INVOKABLE int phormIndexOf (QString phorm);
    Q_INVOKABLE QString phormListAt (int index);
    Q_INVOKABLE bool phormListRemove(QString const &phorm);
    QString phormId() const;
    bool setPhormId (const QString &phormName);
    Q_INVOKABLE int getPhormId (void);
    Q_INVOKABLE int getFirstPhormId ();
    int getPhormIdFromName(const QString &phorm);

    //Unidades de medidas
    bool setUnitInsert(QString const &unit);
    QString unitInsert() const;
    Q_INVOKABLE void loadUnitsList ();
    Q_INVOKABLE int unitListCount ();
    Q_INVOKABLE QString unitsListAt (int index);
    Q_INVOKABLE int unitIndexOf (QString unit);
    Q_INVOKABLE bool unitsListRemove (QString const &unit);

    //Grupos e Sub Grupos
    QString subGroup() const;
    bool setSubGroup(QString const &sub);
    Q_INVOKABLE void loadSubGroupList (int group_id);
    Q_INVOKABLE int subGroupListCount ();
    Q_INVOKABLE QString subGroupListAt (int index);
    Q_INVOKABLE int subGroupIndexOf (QString sub);
    Q_INVOKABLE bool subGroupListRemove (QString const &sub);

    QString group() const;
    bool setGroup(const QString &group);
    bool setGroupId(const QString &groupName);
    QString groupId() const;
    Q_INVOKABLE void loadGroupList ();
    Q_INVOKABLE int groupListCount ();
    Q_INVOKABLE QString groupListAt (int index);
    Q_INVOKABLE int groupIndexOf (QString group);
    Q_INVOKABLE bool groupListRemove (QString const &group);
    Q_INVOKABLE int getGroupId (void);
    Q_INVOKABLE int getFirstGroupId ();

    Q_INVOKABLE void loadValidateList ();
    Q_INVOKABLE int validateCount ();
    Q_INVOKABLE QString validateListAt (int index);
    QString validate() const;
    bool setValidate(const QString &validate);
    Q_INVOKABLE bool validateRemove (QString const &validate);

    Q_INVOKABLE void loadExcipientList (int phorm_id);
    Q_INVOKABLE int excipientCount ();
    Q_INVOKABLE QString excipientListAt (int index);
    Q_INVOKABLE int excipientIndexOf (QString excip);
    QString excipient() const;
    bool setExcipient(QString const &excipient);
    Q_INVOKABLE bool excipientRemove (QString const &excipient);

    Q_INVOKABLE void loadSpecificationCapsList (QString const &phorm);
    Q_INVOKABLE void loadAllSpecificationCapsList();
    Q_INVOKABLE int specificationCapsCount ();
    QString specCapsName() const;
    QString capsCapacity() const;
    QString capsVolume() const;
    void setCapsCapacity(QString capsCapacity);
    void setCapsVolume(QString capsVolume);
    bool setSpecCapsName(QString specCapsName);
    Q_INVOKABLE QString specCapsListAt(int index);
    Q_INVOKABLE QString specCapacityListAt(int index);
    Q_INVOKABLE QString specVolumeListAt(int index);
    Q_INVOKABLE bool specCapsRemove (QString const &specCaps);

    QString packageName() const;
    QString VolumePack() const;
    int labelSize() const;
    void seVolumePack(QString VolumePack);
    void setLabelSize(int labelSize);
    bool setPackageName(QString packageName);
    Q_INVOKABLE void loadPackList (int phorm_id);
    Q_INVOKABLE void loadAllCapsList ();
    Q_INVOKABLE int packCount ();
    Q_INVOKABLE QString packListAt(int index);
    Q_INVOKABLE QString volumePackListAt(int index);
    Q_INVOKABLE QString labelSizeListAt(int index);
    Q_INVOKABLE bool packRemove (QString const &pack);

    Q_INVOKABLE int capsCount ();

    QString packId() const;
    bool setPackId(const QString &packName);
    Q_INVOKABLE int getPackId ();
    QString capsForPack() const;
    int qtdCaps() const;
    bool setCapsForPack(QString capsForPack);
    void setQtdCaps(int qtdCaps);
    Q_INVOKABLE void loadCapsForPackList (int index);
    Q_INVOKABLE int capsForPackCount();
    Q_INVOKABLE QString capsForPackListAt(int index);
    Q_INVOKABLE QString capacityListAt(int index);
    Q_INVOKABLE int getFirstCapsForPackId();
    Q_INVOKABLE bool capsForPackListRemove (QString const &cap);

    Q_INVOKABLE void loadPriceUpList (int phorm_id);
    QString priceUp() const;
    bool setPriceUp(const QString &priceUp);
    Q_INVOKABLE int priceUpCount();
    Q_INVOKABLE QString priceUpListAt(int index);
    Q_INVOKABLE bool priceUpRemove (const QString &price);

    QString priceItemInit() const;
    QString priceItemEnd() const;
    QString costManipulation() const;
    QString profitFactor() const;
    QString minPrice() const;
    QString packFactor() const;
    QString costPack() const;
    void setPackFactor(QString packFactor);
    void setCostPack(QString CostPack);

    void setPriceItemInit(QString priceItemInit);
    void setPriceItemEnd(QString priceItemEnd);
    void setCostManipulation(QString costManipulation);
    void setProfitFactor(QString profitFactor);
    void setMinPrice(QString minPrice);
    QString priceUPInsert() const;
    bool setPriceUPInsert(QString priceUPInsert);
    QString packPriceUPInsert() const;
    bool setPackPriceUPInsert(QString packPriceUPInsert);

    Q_INVOKABLE bool getPriceUpData(const QString &item);
    Q_INVOKABLE bool getPackPriceUpData(const QString &item);

signals:

public slots:

private:
    static void append_phorms(QQmlListProperty<QString> *list, QString *phorm);

    QSqlDatabase m_database;
    QString m_phormInsert;
    QString m_phormId;
    QString m_unitInsert;
    QList<QString *> m_phorms;
    QStringList m_phormList;
    QStringList m_unitsList;
    int m_phormListCount;
    int m_unitListCount;

    QString m_subGroup;
    QStringList m_subGroupList;
    int m_subGroupListCount;
    QString m_group;
    QStringList m_groupList;
    int m_groupListCount;
    QString m_groupId;

    QString m_validate;
    QStringList m_validateList;
    int m_validateCount;

    int m_excipientCount;
    QStringList m_excipientList;
    QString m_excipient;

    int m_specCapsCount;
    QStringList m_specCapsList;
    QStringList m_specCapsCapacityList;
    QStringList m_specCapsVolumeList;
    QString m_specCapsName;
    QString m_capsCapacity;
    QString m_capsVolume;

    QString m_packId;
    QString m_packageName;
    float m_VolumePack;
    int m_labelSize;
    int m_packListCount;
    QStringList m_packingList;
    QStringList m_volumePackList;
    QStringList m_labelSizeList;

    int m_capsListCount;
    QString m_capsForPack;
    int m_qtdCaps;
    QStringList m_capsForPackList;
    QStringList m_capacityList;

    QString m_priceUp;
    int m_priceUpCount;
    QStringList m_priceUpList;
    int m_priceItemInit;
    int m_priceItemEnd;
    float m_costManipulation;
    float m_profitFactor;
    float m_minPrice;
    float m_packFactor;
    float m_CostPack;
    QString m_priceUPInsert;
    QString m_packPriceUPInsert;
};

#endif // MAINTENANCEHANDLER_H
