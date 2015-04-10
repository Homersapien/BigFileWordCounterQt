#include "StdAfx.h"
#include "WordCounting/IndexingStreamWriter.h"

// Класс IndexingStreamWriter
IndexingStreamWriter::IndexingStreamWriter(QTextStream* targetStream, QDataStream* indexStream) :
    m_targetStream(targetStream),
    m_indexStream(indexStream),
    m_unpackedRecordsNumber(0),
    m_totalRecordsNumber(0),
    m_streamsArePrepared(false),
    m_streamsAreFinished(false)
{
    assert(targetStream != NULL);
    assert(m_indexStream != NULL);
}
IndexingStreamWriter::~IndexingStreamWriter()
{
}
void IndexingStreamWriter::prepareStreams()
{
    // Предусловия
    assert(!m_streamsArePrepared);

    m_indexStream->device()->seek(sizeof(qint64));
    m_streamsArePrepared = true;
}
bool IndexingStreamWriter::streamsArePrepared()
{
    return m_streamsArePrepared;
}
void IndexingStreamWriter::writeRecord(const QString& record)
{
    // Предусловия
    assert(m_streamsArePrepared);
    assert(!m_streamsAreFinished);

    // Вывод в целевой файл
    (*m_targetStream) << record;
     ++m_totalRecordsNumber;
     ++m_unpackedRecordsNumber;

    if(m_unpackedRecordsNumber == WORD_PACK_LENGTH)
    {
        // Запись указателя на текущую пачку записей
        m_targetStream->flush();
        (*m_indexStream) << m_targetStream->pos();
        m_unpackedRecordsNumber = 0;
    }
}
void IndexingStreamWriter::finishStreams()
{
    // Предусловия
    assert(m_streamsArePrepared);
    assert(!m_streamsAreFinished);

    m_indexStream->device()->seek(0);
    (*m_indexStream) << m_totalRecordsNumber;
    m_streamsAreFinished = true;
}
bool IndexingStreamWriter::streamsAreFinished()
{
    return m_streamsAreFinished;
}
