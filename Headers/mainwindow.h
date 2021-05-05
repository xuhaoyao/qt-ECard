#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QtMultimedia/QMediaPlayer>
#include <QTimer>
#include <QListWidgetItem>
#include <QLatin1Char>
#include <QFile>
#include <QTextStream>
#include "Dao.h"
#include "ecard.h"
#include "music.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_start_Btn_clicked();

    void on_page_2_previous_Btn_clicked();

    void on_page_2_next_Btn_clicked();

    void on_chooseImage_Btn_clicked();

    void on_page_3_previous_Btn_clicked();

    void on_page_3_next_Btn_clicked();

    void on_page_3_setTitle_Btn_clicked();

    void on_page_3_backgroundColor_Btn_clicked();

    void on_page_3_fontStyle_Btn_clicked();

    void on_page_3_fontColor_Btn_clicked();

    void on_page_4_previous_Btn_clicked();

    void on_page_4_cardInfoFont_Btn_clicked();

    void on_page_4_cartInfoColor_Btn_clicked();

    void on_music_start_Btn_clicked();

    void on_music_Playing_HSlider_sliderMoved(int position);

    void setPlayingSliderValue();  //音乐播放进度条匹配音乐播放进度。

    void on_music_volumn_VSlider_sliderMoved(int position);

    void on_music_addMusic_Btn_clicked();

    void on_music_musicList_ListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_music_previous_Btn_clicked();

    void on_music_next_Btn_clicked();

    void on_music_volumn_Btn_clicked();

    void on_music_volumn_VSlider_sliderReleased();

    void setPlayTime();           //设置播放音乐当前播放的时间

    void setAllTime();            //设置目前播放音乐的总时长

    void on_page_4_save_Btn_clicked();

private:
    Ui::MainWindow *ui;

    ECard card;                   //电子贺卡的实例对象

    Music music;                  //音乐信息的实例对象

    Dao dao;                      //此对象用来操作数据库

    QMediaPlayer m_mediaPlayer;   //音乐播放

    int musicCurrentRow;          //现在播放的是第几首歌

    QTimer* timer;                //定时器

    QString musicPath;            //存放音乐文件夹的路径。

    QString getTime(int time);    //获得播放歌曲此时刻的毫秒数

    QString filename;             //贺卡图片的存放绝对路径

    QImage chooseImage;           //所选择的贺卡图片

    bool saveFlag;                //判断用户是否进行过保存

    void init();                  //初始化操作

    void nextPage();              //下一步

    void previousPage();          //上一步

    void setBackgroundImage();    //设置贺卡图片同时为chooseImage赋值。

    void updateECardEntity();     //更新贺卡的实例对象

    void updateMusicEntity(int cardId);     //更新音乐的实例对象

    void copyECardTitle(QLabel* title1,QLabel* title2);  //title2的一些重要内容复制给title1

    bool loadData();                    //判断是否要从数据库载入数据

    void loadData_ECard();              //从数据库载入贺卡记录

    void loadData_Color();              //从数据库载入颜色记录

    void loadData_Font();               //从数据库载入字体记录

    void loadData_Music();              //从数据库载入音乐相关记录

    void loadData_MusicNameList();      //从数据库载入音乐名字列表

    void beforeClose();                 //关闭程序前做的一些操作

    void closeEvent(QCloseEvent *event);  //重写关闭窗口的函数

};

#endif // MAINWINDOW_H
