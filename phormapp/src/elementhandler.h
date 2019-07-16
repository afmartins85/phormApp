#ifndef ELEMENTHANDLER_H
#define ELEMENTHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtQuick/QQuickItem>

class ElementHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString subGroupId READ subGroupId WRITE setSubGroupId)
    Q_PROPERTY(QString element READ element WRITE setElement)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname)
    Q_PROPERTY(QString unitmanip READ unitmanip WRITE setUnitmanip)
    Q_PROPERTY(QString unitstock READ unitstock WRITE setUnitstock)
    Q_PROPERTY(QString concentration READ concentration WRITE setConcentration)
    Q_PROPERTY(QString factor READ factor WRITE setFactor)
    Q_PROPERTY(QString doseMin READ doseMin WRITE setDoseMin)
    Q_PROPERTY(QString doseMax READ doseMax WRITE setDoseMax)
    Q_PROPERTY(bool isControlled READ isControlled WRITE setIsControlled)
    Q_PROPERTY(QString lote READ lote WRITE setLote)
    Q_PROPERTY(QString description READ description WRITE setDescription)

    Q_PROPERTY(QString uiBase READ uiBase WRITE setUiBase)
    Q_PROPERTY(QString uiEquivalent READ uiEquivalent WRITE setUiEquivalent)
    Q_PROPERTY(QString uiUnit READ uiUnit WRITE setUiUnit)
    Q_PROPERTY(QString utrBase READ utrBase WRITE setUtrBase)
    Q_PROPERTY(QString utrEquivalent READ utrEquivalent WRITE setUtrEquivalent)
    Q_PROPERTY(QString utrUnit READ utrUnit WRITE setUtrUnit)

    Q_PROPERTY(QString cost READ cost WRITE setCost)
    Q_PROPERTY(QString costFactor READ costFactor WRITE setCostFactor)
    Q_PROPERTY(QString stockMin READ stockMin WRITE setStockMin)
    Q_PROPERTY(QString stockMax READ stockMax WRITE setStockMax)
    Q_PROPERTY(QString density READ density WRITE setDensity)

    Q_PROPERTY(QString loteSupplier READ loteSupplier WRITE setLoteSupplier)

    Q_PROPERTY(QString select READ select WRITE setSelect)

public:
    explicit ElementHandler(QQuickItem *parent = 0);
    ~ElementHandler();

    void setDatabase(const QSqlDatabase &db);
    bool isOpenDB ();

    QString element() const;
    void setElement(const QString &element);
    QString nickname() const;
    void setNickname(const QString &nickname);
    QString concentration() const;
    void setConcentration(QString &c);
    QString factor() const;
    void setFactor(QString &factor);

    QString doseMin() const;
    QString doseMax() const;
    void setDoseMin(QString &dose);
    void setDoseMax(QString &dose);

    bool isControlled() const;
    void setIsControlled(const bool &arg);
    QString lote() const;
    void setLote(QString &lote);
    QString description() const;
    void setDescription(QString arg);

    QString unitmanip() const;
    void setUnitmanip(const QString &unit);
    QString unitstock() const;
    void setUnitstock(const QString &unit);

    Q_INVOKABLE QString getUnitMan (void);
    Q_INVOKABLE QString getUnitStock (void);

    QString uiBase() const;
    QString uiEquivalent() const;
    QString uiUnit() const;
    QString utrBase() const;
    QString utrEquivalent() const;
    QString utrUnit() const;
    void setUiBase(QString arg);
    void setUiEquivalent(QString arg);
    void setUiUnit(const QString &arg);
    void setUtrBase(QString arg);
    void setUtrEquivalent(QString arg);
    void setUtrUnit(const QString &arg);

    QString cost() const;
    QString costFactor() const;
    QString stockMin() const;
    QString stockMax() const;
    QString density() const;
    void setCost(QString arg);
    void setCostFactor(QString arg);
    void setStockMin(QString arg);
    void setStockMax(QString arg);
    void setDensity(QString arg);

    QString loteSupplier() const;
    void setLoteSupplier(QString arg);

    Q_INVOKABLE bool checkDependence();
    Q_INVOKABLE bool insert();
    Q_INVOKABLE bool update();
    Q_INVOKABLE void loadElementList();
    Q_INVOKABLE int elementListCount ();
    Q_INVOKABLE QString elementListAt (int index);
    Q_INVOKABLE QString elementSubGroupAt (int index);
    Q_INVOKABLE QString elementGroupAt (int index);

    Q_INVOKABLE bool remove(QString item);

    QString subGroupId() const;
    void setSubGroupId(QString subGroupId);

    bool setSelect(QString select);
    QString select() const;

    Q_INVOKABLE QString getSubGroupElement(void);
    Q_INVOKABLE QString getGroupName(void);

    Q_INVOKABLE QString getUiConv(void);
    Q_INVOKABLE QString getUtrConv(void);

    QString getUnitString(int index);
public slots:


private:
    QSqlDatabase m_database;

    int m_codeElement;
    int m_subGroupId;
    QString m_element;
    QString m_nickname;
    int m_unitmanip;
    int m_unitstock;
    float m_concentration;
    float m_factor;
    float m_doseMin;
    float m_doseMax;
    bool m_isControlled;
    QString m_lote;
    float m_uiBase;
    float m_uiEquivalent;
    int m_uiUnit;
    float m_utrBase;
    float m_utrEquivalent;
    int m_utrUnit;
    float m_cost;
    float m_costFactor;
    float m_stockMin;
    float m_stockMax;
    float m_density;
    QString m_description;
    QString m_loteSupplier;

    QString m_subGroupElement;
    QString m_groupName;
    QString m_uiConv;
    QString m_utrConv;
    QString m_unitManipulationText;
    QString m_unitStockText;

    QStringList m_elementList;
    int m_elementListCount;

    QStringList m_groupList;

    QStringList m_subGroupList;
    int m_subGroupListCount;
    QString m_select;
};

#endif // ELEMENTHANDLER_H
