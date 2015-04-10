#include "StdAfx.h"
#include "WordCounting/FilePartCreator.h"

const char* TEMP_FILE_NAME_FORMAT = "sorted_part_%d.tmp";

// Класс FilePartCreator
FilePartCreator::FilePartCreator() :
    m_filesNumber(0),
    m_lastOpenFile(NULL),
    m_lastOpenFileStream(NULL)
{

}
FilePartCreator::~FilePartCreator()
{
    closeLastFile();
}
void FilePartCreator::openNewFile()
{
    // Создание нового файла
    m_lastOpenFile = new QFile(fileName(m_filesNumber));
    m_lastOpenFile->open(QFile::WriteOnly);
    ++m_filesNumber;

    // Создание потока записи в новый файл
    m_lastOpenFileStream = new QTextStream(m_lastOpenFile);
}
bool FilePartCreator::hasFileOpen()
{
    return (m_lastOpenFile != NULL);
}
QTextStream* FilePartCreator::lastFileStream()
{
    // Предусловия
    assert(hasFileOpen());

    return m_lastOpenFileStream;
}
void FilePartCreator::closeLastFile()
{
    if(hasFileOpen())
    {
        m_lastOpenFile->close();
        delete m_lastOpenFileStream;
        m_lastOpenFileStream = NULL;
        delete m_lastOpenFile;
        m_lastOpenFile = NULL;
    }
}
int FilePartCreator::filesNumber()
{
    return m_filesNumber;
}
QString FilePartCreator::fileName(int partIndex)
{
    QString result;
    result.sprintf(TEMP_FILE_NAME_FORMAT, partIndex);
    return result;
}
void FilePartCreator::deleteAllCreatedFiles()
{
    // Закрытие последнего открытого файла
    closeLastFile();

    // Удаление всех созданных файлов
    for(int partIndex = 0; partIndex < m_filesNumber; ++partIndex)
    {
        QFile::remove(fileName(partIndex));
    }

    m_filesNumber = 0;
}
