#pragma once

#include "WordCounting/WordCountingSorter.h"
#include "WordCounting/NumericWordSorter.h"
#include "Ui/WordSortingProgressDialog.h"

const int  WORD_HEADER_INDEX = 0;
const int  COUNT_HEADER_INDEX = 1;

class WordTableModel : public QAbstractTableModel
{
public:
    // Создание и уничтожение
    WordTableModel(QObject *parent, QString sourceFilePath);
    virtual ~WordTableModel();

public:
    // Начальная подготовка модели
    void prepare();
    bool isPrepared() const;

public:
    // Параметры таблицы
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

public:
    // Данные
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual void sort(int column, Qt::SortOrder order);

protected:
    // Реализация
    WordRecord record(int row) const;
    void changeSortingOrder(ExternalWordSorter& sorter);
    void bindToFile(QString filePath, QString indexPath);
    void releseCurrentFile();

protected:
    // Реализация (поля)
    bool m_isPrepared;
    WordCountingSorter m_wordCountingSorter;
    NumericWordSorter m_numericWordSorter;
    qint64 m_totalRecordsNumber;
    QFile* m_boundFile;
    QTextStream* m_boundFileStream;
    QFile* m_indexFile;
    QDataStream* m_indexFileStream;
    mutable WordRecordCollection m_cachedPage;
    mutable int m_cachedPageIndex;
    WordSortingProgressDialog m_progressDialog;
};
