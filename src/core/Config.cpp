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

#include "Config.hpp"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>
#include <QTemporaryFile>

using namespace Settings;

Config * Config::m_instance(Q_NULLPTR);

Config::~Config()
{

}

QVariant Config::get(const QString &key)
{
    return m_settings->value(key);
}

QVariant Config::get(const QString &key, const QVariant &defaultValue)
{
    return m_settings->value(key, defaultValue);
}

void Config::set(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

Config *Config::instance()
{
    if (!m_instance) {
        m_instance = new Config(qApp);
        QCoreApplication::setApplicationName("3D-LED Cube");
        QCoreApplication::setOrganizationName("ledcc");
    }
    return m_instance;
}

void Config::createConfigFromFile(QString &file)
{
    Q_ASSERT(!m_instance);
    m_instance = new Config(file,qApp);
}

void Config::createTempFileInstance()
{

}

Config::Config(QObject *parent) :
    QObject(parent)
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

    userPath += "ledcc.ini";

    init(userPath);
}

Config::Config(const QString &fileName, QObject *parent):
    QObject(parent)
{
    init(fileName);
}

void Config::init(const QString &fileName)
{
    m_settings.reset(new QSettings(fileName, QSettings::IniFormat));

    m_defaults.insert(ShowMainToolbar, true);
    m_defaults.insert(ShowAnimationToolbar, true);
    m_defaults.insert(ShowHelpToolbar, true);
    m_defaults.insert(ShowAnimationOptionPreview, true);
}
