#include "StdAfx.h"
#include <map>
#include <set>
#include <algorithm>
#include "WordCounting/WordCountingSorter.h"

const char ALPHABETICALLY_ORDERED_RESULT_SUFFIX[] = "a";

bool WordComparator(const WordRecord& a, const WordRecord& b)
{
    return (a.word < b.word);
}
bool CountingWordComparator(WordRecord& a, WordRecord& b)
{
    if(a.word == b.word)
    {
        ++a.count;
        return true;
    }
    else
    {
        return false;
    }
};

// Класс WordCountingSorter
WordCountingSorter::WordCountingSorter(QString sourceFilePath) :
    ExternalWordSorter(sourceFilePath)
{
    
}
WordCountingSorter::~WordCountingSorter(void)
{

}
void WordCountingSorter::readRecord(QTextStream& sourceStream, WordRecord& record)
{
    sourceStream >> record.word;
    record.count = 1;
}
void WordCountingSorter::sortRecords(WordRecordCollection& words, int& actualNumberOfWords)
{
    std::sort(words.begin(), words.begin() + actualNumberOfWords, WordComparator);
    WordRecordCollection::iterator newEnd = std::unique(words.begin(),
                                                        words.begin() + actualNumberOfWords,
                                                        CountingWordComparator);
    actualNumberOfWords = (newEnd - words.begin());  
}
KWayMerger* WordCountingSorter::kWayMerger()
{
    return &m_merger;
}
QString WordCountingSorter::fileNameSuffix() const
{
    return ALPHABETICALLY_ORDERED_RESULT_SUFFIX;
}
