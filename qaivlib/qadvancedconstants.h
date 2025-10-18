/*
Copyright (c) 2019, Grgo Mariani
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Krasnoshchekov Petr ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Krasnoshchekov Petr BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CUSTOM_CONSTANTS_H
#define CUSTOM_CONSTANTS_H

#include <Qt>
#include <QSize>
#include <QString>

enum CustomRoles {
    MultiLevelHeaderView_Rows = Qt::UserRole,      // - User role to identify
    MultiLevelHeaderView_Columns,                  //     header rows and columns
    Item_Rotated,                           // - Appended data for rotated items
    Item_Order_Identify,                    // - Bind item to column/row index
    Item_Filtering                          // - Show filter button for section
};


enum class RotationType {
    DEFAULT,
    HORIZONTAL,
    VERTICAL
};

inline const QSize HeaderIconSize = QSize(32, 32);

inline const QString HeaderFilterIcon = QStringLiteral(":/svg/filter.svg");

// Adjusted size for better Qt6.8+ HiDPI scaling
inline const QSize HeaderSortMarkSize = QSize(16, 16);
inline const QString HeaderSortUpIcon = QStringLiteral(":/svg/up-arrow-blue.svg");
inline const QString HeaderSortDownIcon = QStringLiteral(":/svg/down-arrow-blue.svg");


#endif // CUSTOM_CONSTANTS_H
