#ifndef QSEARCHBARPRIVATE_P_H
#define QSEARCHBARPRIVATE_P_H

#include <QModelIndexList>

class QSearchBarPrivate
{
public:
    QSearchBarPrivate();
    ~QSearchBarPrivate();

    QModelIndexList results;
};

#endif // QSEARCHBARPRIVATE_P_H
