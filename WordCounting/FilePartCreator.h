#pragma once

#include <QTextStream>
#include <QString>

class FilePartCreator
{
public:
    // Константы
    static const int MAX_PARTS_NUMBER = 64;

public:
    // Создание и уничтожение
    FilePartCreator();
    virtual ~FilePartCreator();

public:
    // 
    void openNewFile();
    bool hasFileOpen();
    QTextStream* lastFileStream();
    void closeLastFile();
    int filesNumber();
    QString fileName(int fileIndex);
    void deleteAllCreatedFiles();

private:
    // Реализация (отказ от копирования)
    FilePartCreator(const FilePartCreator&);
    FilePartCreator& operator=(const FilePartCreator&);

protected:
    // Реализация (поля)
    int m_filesNumber;
    QFile* m_lastOpenFile;
    QTextStream* m_lastOpenFileStream;
};

