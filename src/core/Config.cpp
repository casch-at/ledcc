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

Config * Config::m_instance(Q_NULLPTR);
Config::~Config()
{

}

QVariant Config::get(const QString &key)
{

}

QVariant Config::get(const QString &key, const QVariant &defaultValue)
{

}

Config *Config::instance()
{

}

void Config::createConfigFromFile(QString &file)
{

}

void Config::createTempFileInstance()
{

}

Config::Config(QObject *parent) :
    QObject(parent)
{
}

Config::Config(const QString &fileName, QObject *parent)
{

}
