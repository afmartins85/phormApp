#include "clienthandler.h"
#include <QDebug>
#include <QRegExp>
#include <QDateTime>

ClientHandler::ClientHandler(QQuickItem *parent) : QQuickItem(parent)
{
    qDebug("Client handler Contructor");
}
ClientHandler::~ClientHandler()
{
    qDebug("Client handler Destructor");
}

void ClientHandler::setDatabase(const QSqlDatabase &db)
{
   m_database = db;

   QString statLine = QString ("SELECT MIN(states_id) FROM tb_states");

   QSqlQuery query = m_database.exec(statLine);
   //Number of columms

   int id = 0;
   while (query.next()) {
       bool result;
       id = query.value(0).toInt(&result);
   }
   qDebug() << "Estados cadastrados " << id;

   if (id <= 0) {

       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('AC', 'ACRE')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('AL', 'ALAGOAS')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('AP', 'AMAPÁ')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('AM', 'AMAZONAS')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('BA', 'BAHIA')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('CE', 'CEARÁ')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('DF', 'DISTRITO FEDERAL')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('ES', 'ESPÍRITO SANTO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('GO', 'GOIÁS')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('MA', 'MARANHÃO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('MT', 'MATO GROSSO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('MS', 'MATO GROSSO DO SUL')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('MG', 'MINAS GERAIS')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('PA', 'PARÁ')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('PB', 'PARAÍBA')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('PR', 'PARANÁ')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('PE', 'PERNAMBUCO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('PI', 'PIAUÍ')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('RJ', 'RIO DE JANEIRO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('RN', 'RIO GRANDE DO NORTE')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('RS', 'RIO GRANDE DO SUL')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('RO', 'RONDÔNIA')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('RR', 'RORAIMA')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('SC', 'SANTA CATARINA')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('SP', 'SÃO PAULO')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('SE', 'SERGIPE')");
       query.exec("INSERT INTO tb_states (state_name, observation) VALUES ('TO', 'TOCANTINS')");
       query.exec("COMMIT");
   }
}

/**
 * @brief ClientHandler::name
 * @return
 */
QString ClientHandler::name () const
{
    return m_name;
}

/**
 * @brief ClientHandler::firstPhone
 * @return
 */
QString ClientHandler::firstPhone() const
{
    QString str;

    str.sprintf("%lld", m_fphone);
    return str;
}

/**
 * @brief ClientHandler::secPhone
 * @return
 */
QString ClientHandler::secPhone () const
{
    QString str;

    str.sprintf("%lld", m_sphone);
    return str;
}

/**
 * @brief ClientHandler::cpf
 * @return
 */
QString ClientHandler::cpf ()
{
    return m_cpf;
}

/**
 * @brief ClientHandler::birthday
 * @return
 */
QString ClientHandler::birthday ()
{
    QString str("16/07/1985");

    return str;
}

/**
 * @brief ClientHandler::setName
 * @param s
 */
void ClientHandler::setName (const QString & s)
{
    qDebug() << "name " << s;
    m_name = s;
}

/**
 * @brief ClientHandler::setFirstPhone
 * @param phone
 */
void ClientHandler::setFirstPhone (QString phone)
{
    bool ok;
    QString temp, temp2, temp3;

    phone.replace("(", ",");
    phone.replace(")", ",");
    phone.replace(" ", "\x0");

    temp2 = phone.section(',', -1, -1);
    temp = phone.section(",", 1, 1);
    temp3 = temp;
    temp3.append(temp2);

    m_fphone = temp3.toULongLong(&ok);

    qDebug() << "m_fphone: " << m_fphone;
}

/**
 * @brief ClientHandler::setSecPhone
 * @param phone
 */
void ClientHandler::setSecPhone(QString phone)
{
    bool ok;
    QString temp, temp2, temp3;

    phone.replace("(", ",");
    phone.replace(")", ",");
    phone.replace(" ", "\x0");

    temp2 = phone.section(',', -1, -1);
    temp = phone.section(",", 1, 1);
    temp3 = temp;
    temp3.append(temp2);

    m_sphone = temp3.toULongLong(&ok);

    qDebug() << "m_sphone: " << m_sphone;
}

/**
 * @brief ClientHandler::setCPF
 * @param cpf
 */
void ClientHandler::setCPF(const QString &cpf)
{
    m_cpf = cpf;
}

/**
 * @brief ClientHandler::setBirthday
 * @param birth
 */
void ClientHandler::setBirthday(QString birth)
{
    QString temp, temp2, temp3;

    temp2 = birth.section('/', 0, 0);
    temp = birth.section("/", 1, 1);
    temp3 = birth.section("/", 2, 2);

    m_birthday.setDate(temp3.toInt(), temp.toInt(), temp2.toInt());

    qDebug() << "Date " << m_birthday;
}

/**
 * @brief ClientHandler::insert
 * @return
 */
bool ClientHandler::insert()
{

    if (m_birthday.isNull() == true) {
        qDebug() << "mBirthday is NULL";
        m_birthday = QDate(2000, 1, 1);
    }


    QString statLine = QString ("INSERT INTO tb_clients (name, cpf, phone_1, phone_2, birthday, status) VALUES"
                                " (\'%1\', \'%2\', %3, %4, \'%5\', \'%6\')").arg(m_name).arg(m_cpf).arg(m_fphone).arg(
                                m_sphone).arg(m_birthday.toString("dd-MM-yyyy")).arg("A");

    qDebug() << statLine;

    m_database.exec(statLine);

    QSqlError err = m_database.lastError();

    qDebug() << err.text();

    if (err.isValid()) {
        return false;
    }

    m_database.commit();

    return true;
}

/**
 * @brief ClientHandler::checkDependence
 * @return
 */
bool ClientHandler::checkDependence()
{
    return true;
}

/**
 * @brief ClientHandler::loadClientList
 */
void ClientHandler::loadClientList()
{
    //####  Requisita todos os clients presentes no banco
    QString statLine = QString ("SELECT * FROM tb_clients");

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms
    m_clientListCount = query.size();

    int nameCol = query.record().indexOf("name");
    int fPhoneCol = query.record().indexOf("phone_1");
    int sPhoneCol = query.record().indexOf("phone_2");

    m_clientList.clear();
    m_firstPhoneList.clear();
    m_secPhoneList.clear();

    while (query.next()) {
        m_clientList.append(query.value(nameCol).toString());
        m_firstPhoneList.append(query.value(fPhoneCol).toString());
        m_secPhoneList.append(query.value(sPhoneCol).toString());
    }
}

/**
 * @brief ClientHandler::clientListCount
 * @return
 */
int ClientHandler::clientListCount()
{
    return m_clientListCount;
}

/**
 * @brief ClientHandler::clientListAt
 * @param index
 * @return
 */
QString ClientHandler::clientListAt(int index)
{
    if (m_clientListCount < 0) {
        return QString ("");
    }
    return m_clientList.at(index);
}

/**
 * @brief ClientHandler::firstPhoneAt
 * @param index
 * @return
 */
QString ClientHandler::firstPhoneAt(int index)
{
    if (m_clientListCount < 0) {
        return QString ("");
    }
    return m_firstPhoneList.at(index);
}

/**
 * @brief ClientHandler::secPhoneAt
 * @param index
 * @return
 */
QString ClientHandler::secPhoneAt(int index)
{
    if (m_clientListCount < 0) {
        return QString ("");
    }
    return m_secPhoneList.at(index);
}

/**
 * @brief ClientHandler::remove
 * @param client
 * @return
 */
bool ClientHandler::remove(QString client)
{
    QString statLine = QString ("SELECT code_client FROM tb_clients WHERE name=\'%1\'").arg(client);

    QSqlQuery query = m_database.exec(statLine);
    //Number of columms

    QSqlError err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    query.next();

    QString clientId = query.value(0).toString();

    statLine = QString ("DELETE FROM tb_address WHERE code_client_tb_clients=%1").arg(clientId);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    statLine = QString ("DELETE FROM tb_clients WHERE name=\'%1\'").arg(client);

    query = m_database.exec(statLine);
    //Number of columms

    err = m_database.lastError();
    if (err.isValid()) {
        return false;
    }

    return true;
}
