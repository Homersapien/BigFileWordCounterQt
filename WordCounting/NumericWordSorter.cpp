#include "StdAfx.h"
#include "WordCounting/NumericWordSorter.h"

#include <map>
#include <set>
#include <algorithm>

const char* NUMERICALLY_ORDERED_RESULT_SUFFIX = "c";

bool CountComparator(const WordRecord& a, const WordRecord& b)
{
    return (a.count > b.count);
}

// Класс WordCountingSorter
NumericWordSorter::NumericWordSorter(QString sourceFilePath) :
    ExternalWordSorter(sourceFilePath)
{
    
}
void NumericWordSorter::readRecord(QTextStream& sourceStream, WordRecord& record)
{
    sourceStream >> record.word;
    sourceStream >> record.count;
}
NumericWordSorter::~NumericWordSorter(void)
{

}
void NumericWordSorter::sortRecords(WordRecordCollection& words, int& actualNumberOfWords)
{
    std::sort(words.begin(), words.begin() + actualNumberOfWords, CountComparator);
}
KWayMerger* NumericWordSorter::kWayMerger()
{
    return &m_merger;
}
QString NumericWordSorter::fileNameSuffix() const
{
    return NUMERICALLY_ORDERED_RESULT_SUFFIX;
}
