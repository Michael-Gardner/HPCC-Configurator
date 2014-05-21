#include "MainWindow.hpp"
#include "Worker.hpp"
#include "ui_MainWindow.h"
#include "Worker.hpp"
#include <QThread>
#include <QFileDialog>
#include <QtQuick/QQuickView>
#include <QQmlContext>
#include <QFileSystemModel>
#include "../configurator_ui/AppData.hpp"
//#include "../configurator_ui/model.h"
//#include <QDeclarativeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    //QString qstrFileName = QFileDialog::getOpenFileName(this, "Open ironment Configuration File", "/etc/HPCCSystems/source/", ("*.xml"));
    //QString qstrFileName = QFileDialog::getOpenFileName(this, "Open ironment Configuration File", "/home/gleb//HPCC2/build/", ("*.qml"));
    QString qstrFileName = QFileDialog::getOpenFileName(this, "Open Environment Configuration File", "/etc/HPCCSystems/source", ("*.xml"));

    QQuickView *pView = new QQuickView();

    //CONFIGURATOR_API::openConfigurationFile("/etc/HPCCSystems/source/demo1.xml");
    CONFIGURATOR_API::openConfigurationFile(qstrFileName.toLocal8Bit().data());


    ApplicationData *pAppData = new ApplicationData();
    pView->rootContext()->setContextProperty("ApplicationData", pAppData);

    TableDataModel *pTableDataModel = new TableDataModel[MAX_ARRAY_X];

    for (int idx = 0; idx < MAX_ARRAY_X; idx++)
    {
        pView->rootContext()->setContextProperty(modelNames[idx], &(pTableDataModel[idx]));
    }

    pView->setSource(QUrl::fromLocalFile(qstrFileName));

    QWidget *container = QWidget::createWindowContainer(pView);

    this->ui->verticalLayout->addWidget(container);

    //Model *pModel = new Model(20,1,container);
    //this->ui->treeView->setModel(pModel);

    ComponentDataModel *pComponentDataModel = new ComponentDataModel(container);
    this->ui->treeView->setModel(pComponentDataModel);




/*
    QFileSystemModel *filemodel = new QFileSystemModel(this);
       filemodel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
       filemodel->setNameFilterDisables(false);
       //filemodel->setRootPath(sPath);
       ui->treeView->setModel(filemodel);
    //tree->setModel( &myModel);*/
}
