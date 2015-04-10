#include "StdAfx.h"
#include "WordCounting/ExternalWordSorter.h"

const int MAX_SIMULTANEOUS_WORDS = 200000;
const int WORDS_PER_UPDATE = 5000;

const char* RESULT_FILE_NAME_FORMAT = "result_%1.tmp";
const char* RESULT_INDEX_FILE_NAME_FORMAT = "index_%1.tmp";

// Класс ExternalWordSorter
ExternalWordSorter::ExternalWordSorter(QString sourceFilePath) :
    m_sourceFilePath(sourceFilePath),
    m_listener(NULL),
    m_currentProgress(0),
    m_currentTask(ExternalWordSorter::None),
    m_workIsDone(false)
{
}
ExternalWordSorter::~ExternalWordSorter()
{
}
QString ExternalWordSorter::resultFileName() const
{
    return QString(RESULT_FILE_NAME_FORMAT).arg(fileNameSuffix());
}
QString ExternalWordSorter::resultIndexFileName() const
{
    return QString(RESULT_INDEX_FILE_NAME_FORMAT).arg(fileNameSuffix());
}
int ExternalWordSorter::currentProgress() const
{
    return m_currentProgress;
}
ExternalWordSorter::Task ExternalWordSorter::currentTask() const
{
    return m_currentTask;
}
void ExternalWordSorter::setListener(ExternalSorterListener* listenter)
{
    m_listener = listenter;
}
void ExternalWordSorter::performSorting()
{
    // Утверждения
    assert(!m_workIsDone);

    // Открытие исходного файла со словами
    QFile sourceFile(m_sourceFilePath);
    sourceFile.open(QFile::ReadOnly | QFile::Unbuffered | QFile::Text);
    QTextStream sourceStream(&sourceFile);

    // Обработки ошибки открытия файла не предусмотрено
    assert(sourceFile.isOpen());

    // Обновление состояния: начало сортировки
    m_currentTask = ExternalWordSorter::Sorting;
    m_currentProgress = 0;
    wakeUpListener();
    
    // Расщепление и сортирока исходного файла
    FilePartCreator sortedPartCreator;
    splitSourceFileAndSort(sourceStream, sortedPartCreator);
    sortedPartCreator.closeLastFile();

    // Закрытие исходного файла
    sourceFile.close();
    
    // Открытие файла с результатом
    QFile resultFile(resultFileName());
    resultFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream resultFileStream(&resultFile);
        
    // Открытие индексного файла
    QFile indexFile(resultIndexFileName());
    indexFile.open(QFile::WriteOnly);
    QDataStream indexFileStream(&indexFile);

    // Обновление состояния: переход к слиянию
    m_currentTask = ExternalWordSorter::Merging;
    m_currentProgress = 0;
    wakeUpListener();

    // Объединение отдельных частей
    int sortedPartsNumber = sortedPartCreator.filesNumber();
    if(sortedPartsNumber > 0)
    {
        // Обработки ошибки открытия файла не предусмотрено
        assert(resultFile.isOpen());
        assert(indexFile.isOpen());

        // Открытие файлов отсортированных частей
        FileCollection sortedParts(sortedPartsNumber);
        TextStreamCollection sortedPartStreams(sortedPartsNumber);
        for(int partIndex = 0; partIndex < sortedPartsNumber; ++partIndex)
        {
            // Открытие очередной части
            sortedParts[partIndex] = new QFile(sortedPartCreator.fileName(partIndex));
            sortedParts[partIndex]->open(QFile::ReadOnly | QFile::Text);
            sortedPartStreams[partIndex] = new QTextStream(sortedParts[partIndex]);

            // Утверждение: в каждой части должны быть данные
            assert(!sortedPartStreams[partIndex]->atEnd());
        }

        // Создание индексатора
        IndexingStreamWriter resultWriter(&resultFileStream, &indexFileStream);
        resultWriter.prepareStreams();

        // Слияние
        mergeSortedParts(sortedPartStreams, resultWriter);

        // Завершение индексного файла
        resultWriter.finishStreams();

        // Закрытие файла с результатом и индексного файла
        indexFile.close();
        resultFile.close();

        // Закрытие файлов отсортированных частей
        for(int partIndex = 0; partIndex < sortedPartsNumber; ++partIndex)
        {
            sortedParts[partIndex]->close();
            delete sortedParts[partIndex];
            delete sortedPartStreams[partIndex];
        }

        // Удаление отсортированных частей
        sortedPartCreator.deleteAllCreatedFiles();
    }

    // Сортировка выполнена
    m_workIsDone = true;
}
bool ExternalWordSorter::workIsDone() const
{
    return m_workIsDone;
}
void ExternalWordSorter::splitSourceFileAndSort(QTextStream& sourceStream, FilePartCreator& sortedPartCreator)
{
	 // Создание буфера записей
    WordRecordCollection words;
    words.resize(MAX_SIMULTANEOUS_WORDS);
    int actualNumberOfWords = 0;
    int wordsSinceLastUpate = 0;
    
    quint64 sourceFileSize = sourceStream.device()->size();
    while(!sourceStream.atEnd())
    {
        // Чтение очередного слова
        readRecord(sourceStream, words[actualNumberOfWords]);
        ++wordsSinceLastUpate;
        ++actualNumberOfWords;
        
        if(wordsSinceLastUpate >= WORDS_PER_UPDATE)
        {
            // Обновление прогресса
            m_currentProgress = ((100 * sourceStream.device()->pos()) / (sourceFileSize));
            wakeUpListener();
            wordsSinceLastUpate = 0;
        }

        if(actualNumberOfWords >= MAX_SIMULTANEOUS_WORDS)
        {
            // Сортировка и вывод накопленных записей
            sortRecords(words, actualNumberOfWords);
            storeRecords(words, actualNumberOfWords, sortedPartCreator);
            actualNumberOfWords = 0;
        }
    }

    if(actualNumberOfWords > 0)
    {
        // Запись последней части
        sortRecords(words, actualNumberOfWords);
        storeRecords(words, actualNumberOfWords, sortedPartCreator);
    }
}
void ExternalWordSorter::storeRecords(WordRecordCollection& words, int actualNumberOfWords, FilePartCreator& sortedPartCreator)
{
    // Открытие нового файла для записи
    sortedPartCreator.openNewFile();
    QTextStream* currentPartStream = sortedPartCreator.lastFileStream();

    // Запись части данных
    for(int wordIndex = 0; wordIndex < actualNumberOfWords; ++wordIndex)
    {
        (*currentPartStream) << ' ' << words[wordIndex].word << ' ' << words[wordIndex].count;
    }

    // Закрытие файла
    sortedPartCreator.closeLastFile();
}
void ExternalWordSorter::mergeSortedParts(const TextStreamCollection& sortedParts, IndexingStreamWriter& resultWriter)
{
    // Получение слиятеля
    KWayMerger* merger = kWayMerger();

    // Чтение первых записей каждой отсортированной части
    size_t partsNumber = sortedParts.size();
    qint64 totalPartFilesSize = 0;
    for(int partIndex = 0; partIndex < partsNumber; ++partIndex)
    {
        // Чтение записей
        KWayMergerItem currentItem;
        (*sortedParts[partIndex]) >> currentItem.wordRecord.word >> currentItem.wordRecord.count;
        currentItem.sourceParts.set(partIndex);
        merger->collectItem(currentItem);

        // Вычисление общего размера всех частей
        totalPartFilesSize += sortedParts[partIndex]->device()->size();
    }

    int wordsSinceLastUpate = 0;
    while(!merger->isEmpty())
    {
        // Выбор верхнего элемента
        KWayMergerItem topItem;
        merger->returnTopItem(topItem);
        
        // Вывод верхнего элемента в файл с результатом
        QString resultRecord = ' ' + topItem.wordRecord.word + ' ' + QString::number(topItem.wordRecord.count);
        resultWriter.writeRecord(resultRecord);

        // Чтение следующих элементов из тех отсортированных частей, в которых находился верхний элемент
        for(int partIndex = 0; partIndex < partsNumber; ++partIndex)
        {
            if(topItem.sourceParts.test(partIndex) &&
                !sortedParts[partIndex]->atEnd())
            {
                KWayMergerItem currentItem;
                (*sortedParts[partIndex]) >> currentItem.wordRecord.word >> currentItem.wordRecord.count;
                currentItem.sourceParts.set(partIndex);
                merger->collectItem(currentItem);
            }
        }

        ++wordsSinceLastUpate;
        if(wordsSinceLastUpate >= WORDS_PER_UPDATE)
        {
            // Подсчет размера прочитанных данных 
            qint64 numberOfBytesRead = 0;
            for(int partIndex = 0; partIndex < partsNumber; ++partIndex)
            {
                numberOfBytesRead += sortedParts[partIndex]->device()->pos();
            }

            // Обновление прогресса
            m_currentProgress = ((100 * numberOfBytesRead) / (totalPartFilesSize));
            wakeUpListener();
            wordsSinceLastUpate = 0;
        }
    }
}
void ExternalWordSorter::wakeUpListener()
{
    if(m_listener != NULL)
    {
        m_listener->handleSorterStateChange(*this);
    }
}

// Класс KWayMerger
KWayMerger::KWayMerger()
{

}
KWayMerger::~KWayMerger()
{

}
