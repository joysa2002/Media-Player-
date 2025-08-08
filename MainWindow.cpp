#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QStyle>
#include <QFileInfo>
#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(new QMediaPlayer(this))
    , videoWidget(nullptr)  // Will be set from UI
    , audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);

    // Use the existing videoWidget from UI file
    videoWidget = ui->videoWidget;

    this->setStyleSheet(R"(
        QMainWindow {
            background: #2d2d2d;
        }
        QVideoWidget {
            background: black;
            border: 2px solid #444;
        }
        QPushButton {
            background: #3a3a3a;
            color: white;
            border: none;
            padding: 8px;
            min-width: 40px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background: #4a4a4a;
        }
        QSlider::groove:horizontal {
            height: 6px;
            background: #444;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #1abc9c;
            width: 12px;
            border-radius: 6px;
        }
        QLabel#timeLabel { color: #aaa; }
        QLabel#fileLabel { color: #ddd; font-weight: bold; }
    )");

    // Setup video widget properties
    videoWidget->setMinimumSize(400, 300);
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setup media player and connect video output
    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5f); // 50% volume

    // Button connections
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::togglePlayback);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->prevButton, &QPushButton::clicked, [this](){ player->setPosition(0); });
    connect(ui->nextButton, &QPushButton::clicked, [this](){ player->setPosition(player->duration()); });
    connect(ui->volumeButton, &QPushButton::clicked, [this](){audioOutput->setMuted(!audioOutput->isMuted());});
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &MainWindow::toggleFullscreen);

    // Media signals - Add more debugging
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::updatePlaybackState);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &MainWindow::seek);

    // Enhanced error handling
    connect(player, &QMediaPlayer::errorOccurred, [this](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "Media Player Error:" << error << errorString;
        ui->fileLabel->setText("Error: " + errorString);
    });

    // Video widget signals
    connect(videoWidget, &QVideoWidget::fullScreenChanged, [this](bool fullScreen) {
        qDebug() << "Video widget fullscreen changed:" << fullScreen;
    });

    // Initial states
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->timeLabel->setText("00:00 / 00:00");

    // Show video widget
    videoWidget->show();

    qDebug() << "MainWindow initialized successfully";
}

MainWindow::~MainWindow()
{
    // Clean shutdown
    player->stop();
    delete ui;
}

void MainWindow::togglePlayback()
{
    if(player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    } else {
        player->play();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void MainWindow::onStopClicked()
{
    player->stop();
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void MainWindow::updatePosition(qint64 position)
{
    if (!ui->progressSlider->isSliderDown()) {  // Prevent slider jumping while dragging
        ui->progressSlider->setValue(position);
    }
    ui->timeLabel->setText(formatTime(position) + " / " + formatTime(player->duration()));
}

void MainWindow::updateDuration(qint64 duration)
{
    ui->progressSlider->setRange(0, duration);
    ui->timeLabel->setText(formatTime(0) + " / " + formatTime(duration));
    qDebug() << "Duration updated:" << duration << "ms";
}

void MainWindow::updatePlaybackState(QMediaPlayer::PlaybackState state)
{
    qDebug() << "Playback state changed:" << state;
    if(state == QMediaPlayer::StoppedState) {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "Media status changed:" << status;
    switch(status) {
    case QMediaPlayer::LoadedMedia:
        qDebug() << "Media loaded successfully";
        // Force video widget to update
        videoWidget->update();
        break;
    case QMediaPlayer::InvalidMedia:
        qDebug() << "Invalid media";
        ui->fileLabel->setText("Invalid media file");
        break;
    case QMediaPlayer::LoadingMedia:
        qDebug() << "Loading media...";
        break;
    default:
        break;
    }
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                "Open Media File",
                                                QDir::homePath(),
                                                "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv);;Audio Files (*.mp3 *.wav *.aac *.ogg);;All Files (*)"
                                                );

    if(!file.isEmpty()) {
        qDebug() << "Opening file:" << file;

        // Stop current playback
        player->stop();

        // Set new source
        player->setSource(QUrl::fromLocalFile(file));
        ui->fileLabel->setText(QFileInfo(file).fileName());

        // Ensure video widget is visible and ready
        videoWidget->show();
        videoWidget->update();

        qDebug() << "File set as source, video widget visible:" << videoWidget->isVisible();
    }
}

void MainWindow::seek(int position)
{
    player->setPosition(position);
}

void MainWindow::toggleFullscreen()
{
    if (isFullscreen) {
        // Return from fullscreen - put back in the UI layout
        videoWidget->setParent(ui->centralwidget);
        // Find the correct position in the vertical layout (should be at index 0)
        ui->verticalLayout->insertWidget(0, videoWidget);
        videoWidget->showNormal();
        this->showNormal();
    } else {
        // Go fullscreen
        videoWidget->setParent(nullptr);
        videoWidget->setWindowFlags(Qt::Window);
        videoWidget->showFullScreen();
    }
    isFullscreen = !isFullscreen;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullscreen) {
        toggleFullscreen();
        event->accept();
    } else if (event->key() == Qt::Key_Space) {
        togglePlayback();
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

QString MainWindow::formatTime(qint64 milliseconds)
{
    QTime time(0, 0, 0);
    time = time.addMSecs(milliseconds);
    return time.toString(milliseconds > 3600000 ? "hh:mm:ss" : "mm:ss");
}
