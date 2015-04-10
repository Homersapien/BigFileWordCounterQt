#pragma once

#include "WordCounting/ExternalWordSorter.h"
#include "WordCounting/WordCountingKWayMerger.h"

class WordCountingSorter : public ExternalWordSorter
{
public:
    // Создание и уничтожение
    WordCountingSorter(QString sourceFilePath);
    virtual ~WordCountingSorter();

protected:
    // Реализация
    virtual void readRecord(QTextStream& sourceStream, WordRecord& record);
    virtual void sortRecords(WordRecordCollection& words, int& actualNumberOfWords);
    virtual KWayMerger* kWayMerger();
    virtual QString fileNameSuffix() const;

protected:
    // Реализация (поля)
    WordCountingKWayMerger m_merger;
};
