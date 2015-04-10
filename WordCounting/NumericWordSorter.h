#pragma once

#include "WordCounting/ExternalWordSorter.h"
#include "WordCounting/NumericKWayMerger.h"

class NumericWordSorter : public ExternalWordSorter
{
public:
    // Создание и уничтожение
    NumericWordSorter(QString sourceFilePath);
    virtual ~NumericWordSorter();

protected:
    // Реализация
    virtual void readRecord(QTextStream& sourceStream, WordRecord& record);
    virtual void sortRecords(WordRecordCollection& words, int& actualNumberOfWords);
    virtual KWayMerger* kWayMerger();
    virtual QString fileNameSuffix() const;

protected:
    // Реализация (поля)
    NumericKWayMerger m_merger;
};
