#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <bitset>
#include "WordCounting/IndexingStreamWriter.h"
#include "WordCounting/FilePartCreator.h"

class ExternalSorterListener;
class KWayMerger;

const int START_WORD_BUFFER_SIZE = 16;
struct WordRecord
{
    WordRecord() :
        count(0)
    {
        word.reserve(START_WORD_BUFFER_SIZE);
    }
    WordRecord(QString aWord, int aCount) :
        word(aWord),
        count(aCount)
    {}

    QString word;
    int count;
};

typedef std::vector<QFile*> FileCollection;
typedef std::vector<QTextStream*> TextStreamCollection;
typedef std::vector<WordRecord> WordRecordCollection;
typedef WordRecordCollection::const_iterator ConstWordRecordCollectionIterator;

class ExternalWordSorter
{
public:
    // Константы
    enum Task
    {
        None,
        Sorting,
        Merging
    };

public:
    // Создание и уничтожение
    ExternalWordSorter(QString sourceFilePath);
    virtual ~ExternalWordSorter();

public:
    // Сортировка
    void performSorting();
    bool workIsDone()const;
    QString resultFileName() const;
    QString resultIndexFileName() const;

public:
    // Обновление состояния
    int currentProgress() const;
    Task currentTask() const;
    void setListener(ExternalSorterListener* listenter);

protected:
    // Реализация
    void splitSourceFileAndSort(QTextStream& sourceStream, FilePartCreator& sortedPartCreator);
    void storeRecords(WordRecordCollection& words, int actualNumberOfWords, FilePartCreator& sortedPartCreator);
    void mergeSortedParts(const TextStreamCollection& sortedParts, IndexingStreamWriter& resultWriter);
    void wakeUpListener();

protected:
    // Реализация (абстрактные методы)
    virtual void readRecord(QTextStream& sourceStream, WordRecord& record) = 0;
    virtual void sortRecords(WordRecordCollection& words, int& actualNumberOfWords) = 0;
    virtual KWayMerger* kWayMerger() = 0;
    virtual QString fileNameSuffix() const = 0;

private:
    // Реализация (отказ от копирования)
    ExternalWordSorter(const ExternalWordSorter&);
    ExternalWordSorter& operator=(const ExternalWordSorter&);

protected:
    // Реализация (поля)
    QString m_sourceFilePath;
    ExternalSorterListener* m_listener;
    int m_currentProgress;
    Task m_currentTask;
    bool m_workIsDone;
};

class ExternalSorterListener
{
public:
    // Обработка событий сортировщика
    virtual void handleSorterStateChange(const ExternalWordSorter& sorter) = 0;
};

struct KWayMergerItem
{
    typedef std::bitset<FilePartCreator::MAX_PARTS_NUMBER> Bitset;
    WordRecord wordRecord;
    Bitset sourceParts;
};

class KWayMerger
{
public:
    // Создание и уничтожение
    KWayMerger();
    virtual ~KWayMerger();

public:
    // Управление набором и осуществление выбора
    virtual void collectItem(const KWayMergerItem& item) = 0;
    virtual bool isEmpty() = 0;
    virtual void returnTopItem(KWayMergerItem& item) = 0;
    virtual void clear() = 0;

private:
    // Реализация (отказ от копирования)
    KWayMerger(const KWayMerger&);
    KWayMerger& operator=(const KWayMerger&);
};
