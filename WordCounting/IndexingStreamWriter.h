#pragma once

#include <QApplication>

class IndexingStreamWriter
{
public:
    // Константы
    const static int WORD_PACK_LENGTH = 128;

public:
    // Создание и уничтожение
    IndexingStreamWriter(QTextStream* targetStream, QDataStream* indexStream);
    virtual ~IndexingStreamWriter();

public:
    // Вывод
    void prepareStreams();
    bool streamsArePrepared();
    void writeRecord(const QString& record);
    void finishStreams();
    bool streamsAreFinished();

private:
    // Реализация (отказ от копирования)
    IndexingStreamWriter(const IndexingStreamWriter&);
    IndexingStreamWriter& operator=(const IndexingStreamWriter&);

protected:
    // Реализация (поля)
    QTextStream* m_targetStream;
    QDataStream* m_indexStream;
    int m_unpackedRecordsNumber;
    qint64 m_totalRecordsNumber;
    bool m_streamsArePrepared;
    bool m_streamsAreFinished;
};

