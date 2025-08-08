#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QFileDialog>
#include <QSlider>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void togglePlayback();
    void onStopClicked();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void openFile();
    void seek(int position);
    void updatePlaybackState(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void toggleFullscreen();

protected:
    void keyPressEvent(QKeyEvent *event) override;


private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
    QString formatTime(qint64 milliseconds);
    bool isFullscreen = false;
};
#endif
