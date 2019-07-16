#ifndef DOCTORHANDLER_H
#define DOCTORHANDLER_H

#include <QQuickItem>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QtQuick/QQuickItem>

class DoctorHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString crm READ crm WRITE setCrm)
    Q_PROPERTY(QString phone READ phone WRITE setPhone)
    Q_PROPERTY(QString fax READ fax WRITE setFax)
    Q_PROPERTY(QString speciality READ speciality WRITE setSpeciality)

public:
    explicit DoctorHandler(QQuickItem *parent = 0);
    ~DoctorHandler();

    void setDatabase(const QSqlDatabase &db);

    QString name() const;
    void setName(QString name);

    QString crm() const;
    void setCrm(QString crm);

    QString phone() const;
    void setPhone(QString phone);

    QString fax() const;
    void setFax(QString fax);

    QString speciality() const;
    void setSpeciality(QString speciality);

    Q_INVOKABLE bool checkDependence();

    Q_INVOKABLE void loadDoctorList();
    Q_INVOKABLE int doctorListCount();
    Q_INVOKABLE QString doctorListAt(int index);
    Q_INVOKABLE QString crmListAt(int index);
    Q_INVOKABLE QString phoneListAt(int index);

    Q_INVOKABLE bool insert();

    Q_INVOKABLE void filterDoctorList(QString text);
    Q_INVOKABLE int doctorListCountSwap ();
    Q_INVOKABLE QString doctorListSwapAt (int index);
    Q_INVOKABLE QString crmListSwapAt (int index);
    Q_INVOKABLE QString phoneListSwapAt (int index);

    Q_INVOKABLE bool remove(QString crm);
signals:

public slots:


private:
    QSqlDatabase m_database;

    QString m_name;
    QString m_crm;
    unsigned long long m_phone;
    unsigned long long m_fax;
    QString m_speciality;

    QStringList m_doctorList;
    QStringList m_crmList;
    QStringList m_phoneList;

    int m_doctorListCount;

    QStringList m_doctorListSwap;
    QStringList m_crmListSwap;
    QStringList m_phoneListSwap;
    int m_doctorCountSwap;

};

#endif // DOCTORHANDLER_H
