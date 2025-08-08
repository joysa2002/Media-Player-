Smart Media Player (Qt 6.8)
A simple yet functional media player built with C++ and QML using Qt 6.8.
This project demonstrates audio/video playback with basic controls like Play, Pause, Stop, file selection, and volume adjustment.
It’s a perfect starting point for beginners learning Qt Multimedia.

Features
Play/Pause/Stop functionality for media files.

Open file chooser to load local audio or video files.

Volume slider for adjusting sound levels.

Video output area for displaying video files.

Written with C++ backend and QML-based UI.

Works on Qt 6.8 and cross-platform (Windows/Linux/macOS).

Tech Stack
Language: C++17, QML

Framework: Qt 6.8

Modules Used:

QtQuick

QtQuick.Controls

QtMultimedia

Project Structure
bash
Copy
Edit
SmartMediaPlayer/
│── CMakeLists.txt         # Build configuration
│── main.cpp               # Application entry point
│── MainWindow.cpp/.h/.ui  # Optional UI logic (if QWidget-based)
│── MediaManager.cpp/.h    # C++ backend handling media control
│── Main.qml               # QML UI layout
│── resources.qrc          # Resource file for QML
🚀 How to Build & Run

Install Qt 6.8
Download from Qt Downloads

Install components:

Qt 6.8.x

Qt Multimedia

MinGW (Windows) or your system compiler

Clone the Repository
bash
Copy
Edit
git clone https://github.com/your-username/SmartMediaPlayer.git
cd SmartMediaPlayer

Build with CMake
bash
Copy
Edit
mkdir build && cd build
cmake ..
cmake --build .

Run the Application
bash
Copy
Edit
./SmartMediaPlayer   # Linux/macOS
SmartMediaPlayer.exe # Windows

Usage Instructions
Launch the app

Click "Open File" to select a local audio/video file

Use:

▶️/⏸️ Play — start playback / Pause — pause playback

⏹️ Stop — stop playback

🔊 Volume slider — adjust sound level

If playing video, it will display in the video output area.

📖 Learning Goals
This project will help you:

Understand Qt Multimedia for audio/video playback

Learn integrating C++ and QML

Use QMediaPlayer and related classes in Qt 6

Build cross-platform desktop apps with Qt

for more refrence a Demo is available in readme and output file.

📝 Notes
QMediaPlaylist is not available in Qt 6.8 — this project uses only QMediaPlayer.

Make sure the file paths are correct, especially on Windows (escape \ as \\ or use /).

This is a beginner-friendly base — you can extend it with playlists, seek bar, metadata display, etc.

📄 License
This project is licensed under the MIT License — you can freely use and modify it.
