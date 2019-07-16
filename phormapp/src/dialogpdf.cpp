#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include "QPdfWidget"
#include "dialogpdf.h"

DialogPDF::DialogPDF(QWidget *pParent, Qt::WindowFlags flags)
    : QMainWindow(pParent, flags)
{
    setWindowIcon(QIcon(":/images/icons/pdf.png"));

    m_pPdfWidget = new QPdfWidget();
    setCentralWidget(m_pPdfWidget);

    createActions();
    createToolBar();
}

void DialogPDF::loadFile(const QString &path)
{
    if (m_pPdfWidget->loadFile(path)) {
        // Update window title with the file name
        QFileInfo fi(path);
        setWindowTitle(fi.fileName());
    }
}

void DialogPDF::createActions()
{
    m_pOpenFileAction = new QAction(QIcon(":/images/icons/folder.png"), tr("&Open..."), this);
    connect(m_pOpenFileAction, &QAction::triggered, this, &DialogPDF::onOpenFile);
}

void DialogPDF::createToolBar()
{
    QToolBar *pToolBar = addToolBar(tr("File"));
    pToolBar->setMovable(false);
    pToolBar->addAction(m_pOpenFileAction);
}

void DialogPDF::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("PDF file (*.pdf)"));
    if (fileName.isEmpty()) {
        return;
    }

    loadFile(fileName);
}
