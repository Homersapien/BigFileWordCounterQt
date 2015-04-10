#pragma once

#include <QDialog>
#include "ui_WordCounterProgressDialog.h"
#include "WordCounting/ExternalWordSorter.h"

class WordSortingProgressDialog : public QDialog, public ExternalSorterListener
{
    Q_OBJECT

public:
    // Создание и уничтожение
    WordSortingProgressDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~WordSortingProgressDialog();

protected:
    // Реализация
    virtual void handleSorterStateChange(const ExternalWordSorter& sorter);
    void connectEventHandlers();

protected:
    // Реализация (поля)
    Ui::WordSortingProgressDialogClass ui;
};
