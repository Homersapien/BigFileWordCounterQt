#pragma once

#include <QMainWindow>
#include "ui_WordCounter.h"

class WordCounterWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    // Создание и уничтожение
    WordCounterWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~WordCounterWindow();

protected:
    // Реализация
    void connectEventHandlers();

protected slots:
    // Реализация (слоты)
    void handleExitAction();
    void handleOpenAction();

protected:
    // Реализация (поля)
    Ui::WordCounterClass ui;
};
