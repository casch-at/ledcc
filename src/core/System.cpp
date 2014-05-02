/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "System.hpp"
#include <QDesktopServices>
#include <QDir>


/*!
 \brief Retrive the system user default config path

 \return QString Path to system user config path
*/
QString System::getConfigPath()
{
    QString userPath;
    QString homePath = QDir::homePath();

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    // we can't use QDesktopServices on X11 as it uses XDG_DATA_HOME instead of XDG_CONFIG_HOME
    QByteArray env = qgetenv("XDG_CONFIG_HOME");
    if (env.isEmpty()) {
        userPath = homePath;
        userPath += "/.config";
    }
    else if (env[0] == '/') {
        userPath = QFile::decodeName(env);
    }
    else {
        userPath = homePath;
        userPath += '/';
        userPath += QFile::decodeName(env);
    }

    userPath += "/ledcc/";
#else
    userPath = QDir::fromNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    // storageLocation() appends the application name ("/keepassx/") to the end
#endif
    return userPath;
}
