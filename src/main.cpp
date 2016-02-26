/* Copyright (C) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 *
 * This file is part of eta-gestemas.
 *
 * eta-gestemas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eta-gestemas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eta-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xlibwindowmanageradapter.h"
#include "windowmanageradapterlistener.h"
#include "targetfactory.h"
#include "config.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include "libframetouchmanager.h"

bool fileExists(QString path);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(GESTEMAS_APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(GESTEMAS_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription(GESTEMAS_APPLICATION_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("<recognizersfile>",
                            QCoreApplication::translate("main",
                            "Configuration xml file that defines recognizers"));
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    QString recognizersFile = QDir::homePath() +
                              QDir::separator() +
                              GESTEMAS_CONFIG_USER_PATH;
    if (args.size() > 0 && !args.at(0).isEmpty()) {
        recognizersFile = args.at(0);
    }

    if (!fileExists(recognizersFile)) {
        Q_ASSERT(fileExists(GESTEMAS_CONFIG_SYSTEM_PATH));
        QFileInfo fi(recognizersFile);
        QString path = fi.path();
        QDir dir(path);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QFile::copy(GESTEMAS_CONFIG_SYSTEM_PATH, recognizersFile);
    }

    XLibWindowManagerAdapter windowManagerAdapter(&app);
    Display *display = static_cast<Display *>(windowManagerAdapter.display());
    LibFrameTouchManager touchManager(display);
    TargetFactory targetFactory(recognizersFile);
    WindowManagerAdapterListener windowManagerAdapterListener(&touchManager,
                                                              &targetFactory);
    windowManagerAdapter.setListener(&windowManagerAdapterListener);
    windowManagerAdapter.dispatchEvents();

    return app.exec();
}

bool fileExists(QString path)
{
    QFileInfo checkFile(path);
    // Check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}
