#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QtQuick/QQuickItem>

class ClientHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString firstPhone READ firstPhone WRITE setFirstPhone)
    Q_PROPERTY(QString secPhone READ secPhone WRITE setSecPhone)
    Q_PROPERTY(QString cpf READ cpf WRITE setCPF)
    Q_PROPERTY(QString birthday READ birthday WRITE setBirthday)

public:
    explicit ClientHandler(QQuickItem *parent = 0);
    ~ClientHandler();

    QString name () const;
    QString firstPhone () const;
    QString secPhone () const;
    QString cpf ();
    QString birthday ();

    void setName (const QString &);
    void setFirstPhone (QString);
    void setSecPhone (QString);
    void setCPF (const QString &);
    void setBirthday (QString);

    void setDatabase(const QSqlDatabase &db);

    Q_INVOKABLE bool insert();
    Q_INVOKABLE bool checkDependence();

    Q_INVOKABLE void loadClientList();
    Q_INVOKABLE int clientListCount();
    Q_INVOKABLE QString clientListAt (int index);
    Q_INVOKABLE QString firstPhoneAt (int index);
    Q_INVOKABLE QString secPhoneAt (int index);

    Q_INVOKABLE bool remove (QString client);

signals:

public slots:

private:
    QString m_name;
    unsigned long long m_fphone;
    unsigned long long m_sphone;
    QString m_cpf;
    QDate m_birthday;

    QSqlDatabase m_database;

    QStringList m_clientList;
    int m_clientListCount;

    QStringList m_firstPhoneList;
    int m_firstPhoneCount;

    QStringList m_secPhoneList;
    int m_secPhoneCount;

};

#endif // CLIENTHANDLER_H
