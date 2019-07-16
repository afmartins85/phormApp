#ifndef LOCALHANDLER_H
#define LOCALHANDLER_H

#include <QQuickItem>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QtQuick/QQuickItem>


class LocalHandler : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress)
    Q_PROPERTY(QString district READ district WRITE setDistrict)
    Q_PROPERTY(QString city READ city WRITE setCity)
    Q_PROPERTY(QString state READ state WRITE setState)
    Q_PROPERTY(QString stateId READ stateId WRITE setStateId)
    Q_PROPERTY(QString complement READ complement WRITE setComplement)
    Q_PROPERTY(QString insertCity READ insertCity WRITE setInsertCity)
public:
    explicit LocalHandler(QQuickItem *parent = 0);
    ~LocalHandler();

    void setDatabase(const QSqlDatabase &db);

    QString address() const;
    QString district() const;
    QString city() const;
    QString state() const;
    QString complement() const;
    void setAddress(const QString &address);
    void setDistrict(const QString &district);
    void setCity(const QString &city);
    void setState(const QString &state);
    void setComplement(const QString &complement);

    Q_INVOKABLE QString stateListAt(int index);
    Q_INVOKABLE void loadStateList();
    Q_INVOKABLE int statesCount();
    Q_INVOKABLE int getFirstStateId();
    Q_INVOKABLE void loadCityList(int state_id);
    Q_INVOKABLE int cityListCount();
    Q_INVOKABLE QString cityListAt(int index);

    QString stateId() const;
    bool setStateId(const QString &stateId);
    Q_INVOKABLE int getStateId();
    QString insertCity() const;
    bool setInsertCity(QString insertCity);

    Q_INVOKABLE bool insert(int type, QString str);

signals:

public slots:

private:

    QSqlDatabase m_database;

    QString m_addr;
    QString m_district;
    QString m_city;
    QString m_state;
    QString m_compl;

    QStringList m_statesList;
    QStringList m_cityList;
    int m_statesListCount;
    int m_cityListCount;

    QString m_stateId;
    QString m_insertCity;

};

#endif // LOCALHANDLER_H
