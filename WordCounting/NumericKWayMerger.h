#pragma once

#include <set>
#include "WordCounting/ExternalWordSorter.h"

class NumericItemComparator
{
public:
    bool operator()(const KWayMergerItem& a, const KWayMergerItem& b)
    {
        return (a.wordRecord.count > b.wordRecord.count);
    }
};

class NumericKWayMerger : public KWayMerger
{
public:
    // Создание и уничтожение
    NumericKWayMerger();
    virtual ~NumericKWayMerger();

public:
    // Управление набором и осуществление выбора
    virtual void collectItem(const KWayMergerItem& item);
    virtual bool isEmpty();
    virtual void returnTopItem(KWayMergerItem& item);
    virtual void clear();

protected:
    // Реализация (поля)
    std::multiset<KWayMergerItem, NumericItemComparator> m_items;
};
