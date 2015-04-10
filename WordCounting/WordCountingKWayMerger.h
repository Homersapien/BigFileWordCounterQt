#pragma once

#include <map>
#include "WordCounting/ExternalWordSorter.h"

struct CountSourcePair
{
    CountSourcePair() :
        count(0)
    {}

    int count;
    KWayMergerItem::Bitset sourceParts; 
};

class WordCountingKWayMerger : public KWayMerger
{
public:
    // Создание и уничтожение
    WordCountingKWayMerger();
    virtual ~WordCountingKWayMerger();

public:
    // Управление набором и осуществление выбора
    virtual void collectItem(const KWayMergerItem& item);
    virtual bool isEmpty();
    virtual void returnTopItem(KWayMergerItem& item);
    virtual void clear();

protected:
    // Реализация (поля)
    std::map<QString, CountSourcePair> m_items;
};
