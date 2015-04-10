#include "StdAfx.h"
#include <QFileDialog>
#include "Ui/WordCounterWindow.h"
#include "Ui/WordTableModel.h"
#include "WordCounting/WordCountingSorter.h"

const int  WORD_HEADER_WIDTH = 220;
const int  COUNT_HEADER_WIDTH = 80;

// Класс WordCounterWindow
WordCounterWindow::WordCounterWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags)
{
    // Инициализация
    ui.setupUi(this);
    connectEventHandlers();

    handleOpenAction();
}
WordCounterWindow::~WordCounterWindow()
{

}
void WordCounterWindow::connectEventHandlers()
{
    connect(ui.m_actionExit, SIGNAL(triggered()), this, SLOT(handleExitAction()));
    connect(ui.m_actionOpen, SIGNAL(triggered()), this, SLOT(handleOpenAction()));
}
void WordCounterWindow::handleExitAction()
{
    this->close();
}
void WordCounterWindow::handleOpenAction()
{
    // Выбор файла
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Open File"), 0, tr("Text Files (*.txt)"));
    
    if(!selectedFileName.isEmpty())
    {
        // Установка модели таблицы
        WordTableModel* model = new WordTableModel(0, selectedFileName);
        model->prepare();
        ui.m_tableView->setModel(model);

        // Настройка размера строк и колонок таблицы
        ui.m_tableView->verticalHeader()->setDefaultSectionSize(20);
        ui.m_tableView->setColumnWidth(WORD_HEADER_INDEX, WORD_HEADER_WIDTH);
        ui.m_tableView->setColumnWidth(COUNT_HEADER_INDEX, COUNT_HEADER_WIDTH);
        ui.m_tableView->setEnabled(true);
    }
    else
    {
        // Очистка списка
        QAbstractItemModel* model = ui.m_tableView->model();
        if(model != NULL)
        {
            ui.m_tableView->setModel(NULL);
            delete model;
        }
        ui.m_tableView->setEnabled(false);
    }
}
