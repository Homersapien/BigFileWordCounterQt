#include "StdAfx.h"
#include "Ui/WordSortingProgressDialog.h"

const char SORTING_TASK_LABEL[] = "Sorting... (stage 1 of 2)";
const char MERGING_TASK_LABEL[] = "Merging... (stage 2 of 2)";
const char DEFAULT_TASK_LABEL[] = "Ready.";

// Класс WordSortingProgressDialog
WordSortingProgressDialog::WordSortingProgressDialog(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags)
{
    // Инициализация
    ui.setupUi(this);
    connectEventHandlers();
}
WordSortingProgressDialog::~WordSortingProgressDialog()
{

}
void WordSortingProgressDialog::handleSorterStateChange(const ExternalWordSorter& sorter)
{
    switch(sorter.currentTask())
    {
        case ExternalWordSorter::Sorting:
            ui.m_progressLabel->setText(SORTING_TASK_LABEL);
            break;

        case ExternalWordSorter::Merging:
            ui.m_progressLabel->setText(MERGING_TASK_LABEL);
            break;

        default:
            ui.m_progressLabel->setText(DEFAULT_TASK_LABEL);
            break;
    }

    ui.m_progressBar->setValue(sorter.currentProgress());

    QApplication::processEvents();
}
void WordSortingProgressDialog::connectEventHandlers()
{
    
}
