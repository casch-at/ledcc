#########################################README##########################################################

Sie benötigen Qt4.8, Qt libraries finden Sie hier: http://qt-project.org/downloads
bzw. ist Qt auch in Ihrer verwendeten Linux Distribution verfügbar.


Am einfachsten wird es sein wenn sie außerdem Qt-Creator Installieren so können sie das Projekt leicht
Importieren und Kompilieren! (.pro File Doppel klicken oder über Qt-Creator Importieren)


Zusätzlich benötigen Sie außerdem die QSerielPort Files welche Sie selbst Kompilieren müssen,
da QSerialPort erst mit Qt5 Einzug findet.

Sie benötigen "qmake" zum Kompilieren!

Info dazu finden sie hier: http://qt-project.org/wiki/QtSerialPort
und hier der download Link: http://qt.gitorious.org/qtplayground/qtserialport/archive-tarball/master

Kopiert von http://qt-project.org/wiki/QtSerialPort
---------------------------------------------------------------------------------------------------------
Building and Installing

Note: At build process a better use of the “shadow build” when the results of building put out to a separate build directory. This allows us
to keep the source directory intact, without clogging of products of build.

Before building the need to:

    ensure that the environment variables are set correctly: correctly specified the path to the installed Qt4/Qt5 and path to used the
compiler
    first create a build directory that is on the same level as the directory with the source code
        /
        |- /serialport-src
        |- /serialport-build

The following are recommended procedures for the build of QtSerialPort library on Qt4/Qt5.

    cd serialport-build
    qmake ../serialport-src/qtserialport.pro
    make [or 'nmake' for MSVC compiler, or 'mingw32-make' for MinGW compiler]
    make install [or 'nmake install' for MSVC compiler, or 'mingw32-make install' for MinGW compiler]

Note: in Unix systems may be required for the installation a super user privileges:

    sudo make install
----------------------------------------------------------------------------------------------------------
    Das wäre alles, hoffe ich habe nichts vergessen! 


 ########################################################################################################   
