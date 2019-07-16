#ifndef DIALOGPDF_H
#define DIALOGPDF_H

#include <QMainWindow>

class QAction;
class QPdfWidget;

class DialogPDF : public QMainWindow
{
    Q_OBJECT
public:
    explicit DialogPDF(QWidget *pParent = nullptr, Qt::WindowFlags flags = 0);
public slots:

    void loadFile(const QString &path);

private slots:

    void onOpenFile();

private:

    void createActions();
    void createToolBar();

    QAction *m_pOpenFileAction;
    QPdfWidget *m_pPdfWidget;
};

#endif // DIALOGPDF_H
