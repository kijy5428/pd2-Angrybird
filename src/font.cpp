#include "font.h"
#include <QFontDatabase>
#include <QString>
#include <QStringList>

Font::Font()
{
        int id = QFontDatabase::addApplicationFont(":/font");
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        fontType = new QFont(family);
        fontType->setPointSize(20);
        fontType->setBold(true);
}
