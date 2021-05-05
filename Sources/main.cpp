#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    /*
     * 音乐播放的Demo
    QMediaPlayer m_mediaPlayer;
    m_mediaPlayer.setMedia(QUrl::fromLocalFile("C:\\CloudMusic\\リテラチュア TV动画《魔女之旅》片头曲.mp3"));
    m_mediaPlayer.play();
    while(1){

    }
    */
}
