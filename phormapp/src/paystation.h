#ifndef PAYSTATION_H
#define PAYSTATION_H

#include <QObject>
#include <QLibrary>

class PayStation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float cost READ cost WRITE setCost)
public:
    explicit PayStation(QObject *parent = 0);

    float cost () const;
    void setCost (const float &);
    void sale (void);

    QLibrary *clisitef;

private:
    float m_cost;
    bool m_cardCredit;
    bool m_cardDebit;

    bool checkPinPad (void);
    int readField(char *pMensagem, char *pCampo);
    int collectData(int comando, unsigned long tipoCampo, char* pDadosComando);
    int result (unsigned long TipoCampo, char *pResultado);
    int LeCampo (short TamMinimo, short TamMaximo, char *pMensagem, char *pCampo);
    void delay(int secs);


signals:

public slots:

};

#endif // PAYSTATION_H
