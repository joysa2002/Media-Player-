#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set multimedia environment variables to help with backend issues
    qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "ffmpeg,directshow");

    // Disable D3D11 debug layer to avoid errors
    qputenv("QT_D3D11_ADAPTER", "-1");  // Use default adapter
    qputenv("QT_ANGLE_PLATFORM", "d3d11");  // Force D3D11 backend

    // Alternative: Force software rendering if hardware fails
    // qputenv("QT_ANGLE_PLATFORM", "warp");  // Uncomment for software rendering

    // Set Qt logging rules to reduce noise (optional)
    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.debug=false");

    qDebug() << "Starting Smart Media Player";
    qDebug() << "Qt Version:" << QT_VERSION_STR;

    MainWindow w;
    w.show();

    return a.exec();
}
