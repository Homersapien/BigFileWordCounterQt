#include "WordCounting/WordCountingKWayMerger.h"

// Класс WordCountingKWayMerger
WordCountingKWayMerger::WordCountingKWayMerger() :
    KWayMerger()
{

}
WordCountingKWayMerger::~WordCountingKWayMerger()
{

}
void WordCountingKWayMerger::collectItem(const KWayMergerItem& item)
{
    // Сложение счетчиков
    m_items[item.wordRecord.word].count += item.wordRecord.count;

    // Сложение битовых полей
    m_items[item.wordRecord.word].sourceParts |= item.sourceParts;
}
bool WordCountingKWayMerger::isEmpty()
{
    return m_items.empty();
}
void WordCountingKWayMerger::returnTopItem(KWayMergerItem& item)
{
    // Копирование элемента
    item.wordRecord.word = m_items.begin()->first;
    item.wordRecord.count = m_items.begin()->second.count;
    item.sourceParts = m_items.begin()->second.sourceParts;

    // Удаление элемента из коллекции
    m_items.erase(m_items.begin());
}
void WordCountingKWayMerger::clear()
{
    m_items.clear();
}
