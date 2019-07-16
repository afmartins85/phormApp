#ifndef SUPPLIERSHANDLER_H
#define SUPPLIERSHANDLER_H

#include <QQuickItem>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QtQuick/QQuickItem>


class SuppliersHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(QString socialName READ socialName WRITE setSocialName)
    Q_PROPERTY(QString cnpj READ cnpj WRITE setCnpj)
    Q_PROPERTY(QString stateRegistration READ stateRegistration WRITE setStateRegistration)
    Q_PROPERTY(QString phone READ phone WRITE setPhone)
    Q_PROPERTY(QString fax READ fax WRITE setFax)
    Q_PROPERTY(QString contactName READ contactName WRITE setContactName)
    Q_PROPERTY(QString status READ status WRITE setStatus)

    Q_PROPERTY(QString codeBank READ codeBank WRITE setcodeBank)
    Q_PROPERTY(QString agencia READ agencia WRITE setAgencia)
    Q_PROPERTY(QString cc READ cc WRITE setCc)

public:
    explicit SuppliersHandler(QQuickItem *parent = 0);
    ~SuppliersHandler();

    void setDatabase(const QSqlDatabase &db);

    QString type() const;
    QString socialName() const;
    QString cnpj() const;
    QString stateRegistration() const;
    QString phone() const;
    QString fax() const;
    QString contactName() const;
    QString status() const;
    void setType(QString type);
    void setSocialName(QString socialName);
    void setCnpj(QString cnpj);
    void setStateRegistration(QString stateRegistration);
    void setPhone(QString phone);
    void setFax(QString fax);
    void setContactName(QString contactName);
    void setStatus(QString status);

    QString codeBank() const;
    QString agencia() const;
    QString cc() const;
    void setcodeBank(QString codeBank);
    void setAgencia(QString agencia);
    void setCc(QString cc);

    Q_INVOKABLE bool checkDependence();

    Q_INVOKABLE void supplierLoadList();
    Q_INVOKABLE int supplierListCount();
    Q_INVOKABLE QString supplyListAt(int index);
    Q_INVOKABLE QString cnpjListAt(int index);
    Q_INVOKABLE QString phoneListAt(int index);

    Q_INVOKABLE bool insert();
    Q_INVOKABLE bool remove(QString supply);

signals:

public slots:


private:
    QSqlDatabase m_database;

    QString m_type;
    QString m_socialName;
    QString m_cnpj;
    QString m_stateRegistration;
    unsigned long long m_phone;
    unsigned long long m_fax;
    QString m_contactName;
    QString m_status;
    QString m_codeBank;
    QString m_agencia;
    QString m_cc;

    int m_supplierListCount;
    QStringList m_supplierList;
    QStringList m_cnpjList;
    QStringList m_phoneList;

};

#endif // SUPPLIERSHANDLER_H
