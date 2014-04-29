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
// Application Includes
#include "System.hpp"

// Qt Includes
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
    System system;
    init( system.getConfigPath() + "ledcc.ini");
}

Config::Config(const QString &fileName, QObject *parent):
    QObject(parent)
{
    init(fileName);
}

void Config::init(const QString &fileName)
{
    m_settings.reset(new QSettings(fileName, QSettings::IniFormat));

    m_defaults.insert(IsMainToolbarHidden, true);
    m_defaults.insert(IsAnimationToolbarHidden, true);
    m_defaults.insert(IsHelpToolbarHidden, true);
    m_defaults.insert(IsAnimationOptionPreviewHidden, true);
}
