#include "StdAfx.h"
#include "Ui/WordTableModel.h"
#include "WordCounting/IndexingStreamWriter.h"

const char WORD_HEADER_TEXT[] = "Word";
const char COUNT_HEADER_TEXT[] = "Count";
const int INVALID_PAGE_INDEX = -1;

// Класс WordTableModel
WordTableModel::WordTableModel(QObject *parent, QString sourceFilePath) :
    QAbstractTableModel(parent),
    m_isPrepared(false),
    m_wordCountingSorter(sourceFilePath),
    m_numericWordSorter(m_wordCountingSorter.resultFileName()),
    m_boundFile(NULL),
    m_boundFileStream(NULL),
    m_indexFile(NULL),
    m_indexFileStream(NULL),
    m_cachedPageIndex(INVALID_PAGE_INDEX),
    m_progressDialog()
{
    // Предусловия
    assert(!sourceFilePath.isEmpty());
}
WordTableModel::~WordTableModel()
{
    releseCurrentFile();
}
void WordTableModel::prepare()
{
	 // Предусловия
    assert(!isPrepared());
	 
    // Создание кэша записей
    m_cachedPage.resize(IndexingStreamWriter::WORD_PACK_LENGTH);

    // Настройка сортирощиков
    m_wordCountingSorter.setListener(&m_progressDialog);
    m_numericWordSorter.setListener(&m_progressDialog);

    m_isPrepared = true;

    // Подсчет слов и сортировка файла-источника
    changeSortingOrder(m_wordCountingSorter);
}
bool WordTableModel::isPrepared() const
{
    return m_isPrepared;
}
Qt::ItemFlags WordTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}
int WordTableModel::rowCount(const QModelIndex &parent) const
{
    // Предусловия
    assert(isPrepared());

    return (int)m_totalRecordsNumber;
}
int WordTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}
QVariant WordTableModel::data(const QModelIndex &index, int role) const
{
    // Предусловия
    assert(isPrepared());

    switch(role)
    {
        case Qt::DisplayRole:
            {
                WordRecord currentRecord = record(index.row());
                if(index.column() == 0)
                {
                    return currentRecord.word;
                }
                else
                {
                    return currentRecord.count;
                }
            }
            break;
        
        default:
            return QVariant();
    }
}
QVariant WordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch(section)
    {
        case WORD_HEADER_INDEX:
            return WORD_HEADER_TEXT;
            break;

        case COUNT_HEADER_INDEX:
            return COUNT_HEADER_TEXT;
            break;

        default:
            return QVariant();
    }
}
void WordTableModel::sort(int column, Qt::SortOrder order)
{
    switch(column)
    {
        case 0:
            changeSortingOrder(m_wordCountingSorter);
            break;

        case 1:
            changeSortingOrder(m_numericWordSorter);
            break;
    }
}
WordRecord WordTableModel::record(int row) const
{
    // Предусловия
    assert(isPrepared());

    // Кэширование страницы, если требуется
    int pageIndex = row / IndexingStreamWriter::WORD_PACK_LENGTH;
    if(pageIndex != m_cachedPageIndex)
    {
        // Устанока стартового смещения в файле
        qint64 pageOffsetInFile = 0;
        if(pageIndex > 0)
        {
            m_indexFile->seek(pageIndex * sizeof(qint64));
            (*m_indexFileStream) >> pageOffsetInFile;
        }
        m_boundFileStream->seek(pageOffsetInFile);

        // Последовательное чтение записей
        int currentSubindex = 0;
        while(!m_boundFileStream->atEnd() && currentSubindex < IndexingStreamWriter::WORD_PACK_LENGTH)
        {
            // Чтение следующей записи
            (*m_boundFileStream) >> m_cachedPage[currentSubindex].word >> m_cachedPage[currentSubindex].count;
            
            ++currentSubindex;
        }

        m_cachedPageIndex = pageIndex;
    }

    // Определение индекса записи внутри страницы
    int targetSubindex = row % IndexingStreamWriter::WORD_PACK_LENGTH;

    return m_cachedPage[targetSubindex];
}
void WordTableModel::changeSortingOrder(ExternalWordSorter& sorter)
{
    if(!sorter.workIsDone())
    {
        // Сортировка
        m_progressDialog.show();
        releseCurrentFile();
        sorter.performSorting();
        m_progressDialog.hide();
    }

    // Привязка к соответстующим файлам
    bindToFile(sorter.resultFileName(), sorter.resultIndexFileName());
    dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
}
void WordTableModel::bindToFile(QString filePath, QString indexPath)
{
    // Освобождение текущего файла
    releseCurrentFile();

    // Открытие файла-источника статистики
    m_boundFile = new QFile(filePath);
    m_boundFile->open(QFile::ReadOnly | QFile::Text);

    // Открытие файла-источника статистики
    m_indexFile = new QFile(indexPath);
    m_indexFile->open(QFile::ReadOnly);

    // Проверки успешности открытия файла не предусмотрено
    assert(m_boundFile->isOpen());
    assert(m_indexFile->isOpen());

    // Создание потоков чтения
    m_boundFileStream = new QTextStream(m_boundFile);
    m_indexFileStream = new QDataStream(m_indexFile);

    // Считывание количества записей
    (*m_indexFileStream) >> m_totalRecordsNumber;

    m_cachedPageIndex = INVALID_PAGE_INDEX;
}
void WordTableModel::releseCurrentFile()
{
    if(m_boundFileStream != NULL)
    {
        delete m_boundFileStream;
        m_boundFileStream = NULL;
    }

    if(m_boundFile != NULL)
    {
        m_boundFile->close();
        delete m_boundFile;
        m_boundFile = NULL;
    }

    if(m_indexFileStream != NULL)
    {
        delete m_indexFileStream;
        m_indexFileStream = NULL;
    }

    if(m_indexFile != NULL)
    {
        m_indexFile->close();
        delete m_indexFile;
        m_indexFile = NULL;
    }
}
