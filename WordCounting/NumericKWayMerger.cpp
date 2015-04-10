#include "WordCounting/NumericKWayMerger.h"

// Класс WordCountingKWayMerger
NumericKWayMerger::NumericKWayMerger() :
    KWayMerger()
{

}
NumericKWayMerger::~NumericKWayMerger()
{

}
void NumericKWayMerger::collectItem(const KWayMergerItem& item)
{
    m_items.insert(item);
}
bool NumericKWayMerger::isEmpty()
{
    return m_items.empty();
}
void NumericKWayMerger::returnTopItem(KWayMergerItem& item)
{
    // Копирование элемента
    item = *m_items.begin();

    // Удаление элемента из коллекции
    m_items.erase(m_items.begin());
}
void NumericKWayMerger::clear()
{
    m_items.clear();
}
