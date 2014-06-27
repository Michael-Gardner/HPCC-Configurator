#include "MainWindow.hpp"
#include "Worker.hpp"
#include "ui_MainWindow.h"
#include "Worker.hpp"
#include "AppData.hpp"
#include <QThread>
#include <QFileDialog>
#include <QtQuick/QQuickView>
#include <QQmlContext>
#include <QFileSystemModel>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QQmlEngine>
#include <QStringListModel>
#include <QComboBox>
#include <QListWidget>
#include "ComponentSelectorDialog.h"
#include "finddialog.h"

#include "../configurator_ui/AppData.hpp"
//#include "../configurator_ui/model.h"
//#include <QDeclarativeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pView(NULL),
    m_pAppData(NULL),
    m_pTableDataModel(NULL),
    m_pComponentDataModel(NULL)
{
    ui->setupUi(this);
    m_pAppData = new ApplicationData();
    m_pTableDataModel = new TableDataModel[MAX_ARRAY_X];

    /*m_pThread = new QThread();
    m_pWorker = new CWorker();

    m_pWorker->moveToThread(m_pThread);

    //connect(m_pWorker, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(m_pWorker, SIGNAL(workRequested()), m_pThread, SLOT(start()));
    connect(m_pThread, SIGNAL(started()), m_pWorker, SLOT(doWork()));
    connect(m_pWorker, SIGNAL(finished()), m_pThread, SLOT(quit()), Qt::DirectConnection);
*/
}

MainWindow::~MainWindow()
{
  /*  m_pWorker->abort();
    m_pThread->quit();

    delete m_pThread;
    delete m_pWorker;*/
    delete m_pAppData;
    //delete m_pTableDataModel;
    delete m_pComponentDataModel;
    delete ui;

}

void MainWindow::addComponentToList(char *pComponent)
{
    QString qstrComp(pComponent);

    this->ui->menuAdd_Component->addAction(qstrComp);
}

void MainWindow::addServiceToList(char *pService)
{
    QString qstrComp(pService);

    this->ui->menuAdd_Service->addAction(qstrComp);
}

void MainWindow::on_actionOpen_triggered()
{
    QString qstrFileName = QFileDialog::getOpenFileName(this, "Open Environment Configuration File", "/etc/HPCCSystems/source", ("*.xml"));

    if (qstrFileName.length() == 0)
    {
        return;
    }

    m_pView = new QQuickView();

    if (m_envFile != "")
    {
        CONFIGURATOR_API::reload(m_envFile.toLocal8Bit().data());
    }
    else
    {
        CONFIGURATOR_API::openConfigurationFile(qstrFileName.toLocal8Bit().data());
    }

    m_envFile = qstrFileName;


    //m_pAppData = new ApplicationData();
    m_pView->rootContext()->setContextProperty("ApplicationData", m_pAppData);

    for (int idx = 0; idx < MAX_ARRAY_X; idx++)
    {
        m_pView->rootContext()->setContextProperty(modelNames[idx], &(m_pTableDataModel[idx]));
    }

    QWidget *container = QWidget::createWindowContainer(m_pView);

    m_pView->setResizeMode(QQuickView::SizeRootObjectToView);

    this->ui->verticalLayout->addWidget(container);

    delete m_pComponentDataModel;
    m_pComponentDataModel = NULL;

    m_pComponentDataModel = new ComponentDataModel(container);

    this->ui->treeView->setModel(m_pComponentDataModel);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QUrl url;
    //QString qstrQML(CONFIGURATOR_API::getQML(index.internalPointer()));
    QString qstrFileName("/tmp/");
    qstrFileName.append(CONFIGURATOR_API::getFileName(index.internalPointer()));
    qstrFileName.append(".qml");

    qDebug() << qstrFileName;

    QFile::remove(qstrFileName);
    QFile qFile(qstrFileName.toLocal8Bit().data());

    if (qFile.open(QIODevice::WriteOnly | QIODevice::Truncate) == 0)
    {
        return;
    }

    QTextStream out(&qFile);
    out << CONFIGURATOR_API::getQML(index.internalPointer(), index.row());

    qFile.close();

    //url.fromLocalFile(qstrFileName.toLocal8Bit().data());
    m_pView->engine()->clearComponentCache();
    m_pView->setSource(QUrl::fromLocalFile(qstrFileName.toLocal8Bit().data()));


    //m_pView->setSource(QUrl::fromLocalFile("/tmp/sasha.xsd.qml"));
    //m_pView->setSource(url);
    //m_pView->show();


    /*m_pView = new QQuickView();

    ApplicationData *pAppData = new ApplicationData();
    m_pView->rootContext()->setContextProperty("ApplicationData", pAppData);

    TableDataModel *pTableDataModel = new TableDataModel[MAX_ARRAY_X];

    for (int idx = 0; idx < MAX_ARRAY_X; idx++)
    {
        m_pView->rootContext()->setContextProperty(modelNames[idx], &(pTableDataModel[idx]));
    }

    m_pView->setSource(url);*/

    //QWidget *container = QWidget::createWindowContainer(m_pView);
    //this->ui->verticalLayout->addWidget(container);
}

void MainWindow::on_actionReload_triggered()
{
    CONFIGURATOR_API::reload(m_envFile.toLocal8Bit().data());
}

void MainWindow::on_actionGenerate_QML_triggered()
{
    CComponenetSelectorDialog compSelDialog(this, QML_OUTPUT);
    compSelDialog.setModal(true);
    compSelDialog.exec();
}

void MainWindow::on_actionGenerate_DockBook_triggered()
{
    CComponenetSelectorDialog compSelDialog(this, DOCBOOK_OUTPUT);
    compSelDialog.setModal(true);
    compSelDialog.exec();
}


void MainWindow::on_actionGenerate_Internal_Data_Structure_Debug_triggered()
{

}

void MainWindow::on_actionGenerate_Dojo_triggered()
{
    CComponenetSelectorDialog compSelDialog(this, DOJO_OUTPUT);
    compSelDialog.setModal(true);
    compSelDialog.exec();
}
