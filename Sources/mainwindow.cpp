#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

void MainWindow::init(){
    //窗口标题
    this->setWindowTitle("E-Card");
    //贺卡部分
    ui->stackedWidget->setCurrentIndex(0);
    ui->page_4_cartInfo_Text->setPlaceholderText("贺卡问候语...");
    ui->page_4_cartInfo_Text->setStyleSheet("background:transparent");

    //音乐播放部分
    timer = new QTimer();
    timer->setInterval(2000);  //定时器两秒发送一次信号 更新音乐播放进度条
    timer->start();
    ui->music_volumn_VSlider->setVisible(false);
    this->musicCurrentRow = 0;

    if(loadData()){
        loadData_ECard();
        loadData_Color();
        loadData_Font();
        loadData_Music();
        loadData_MusicNameList();
    }
}

void MainWindow::nextPage(){
    int nextPageIndex = ui->stackedWidget->currentIndex() + 1;
    if(nextPageIndex < ui->stackedWidget->count())
        ui->stackedWidget->setCurrentIndex(nextPageIndex);
}

void MainWindow::previousPage(){
    int previousPageIndex = ui->stackedWidget->currentIndex() - 1;
    if(previousPageIndex >= 0)
        ui->stackedWidget->setCurrentIndex(previousPageIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_Btn_clicked()
{
    nextPage();
}

void MainWindow::on_page_2_previous_Btn_clicked()
{
    previousPage();
}

void MainWindow::on_page_2_next_Btn_clicked()
{
    nextPage();
    ui->page_3_imageBackground_Label->setPixmap(QPixmap::fromImage(this->chooseImage));
}

void MainWindow::setBackgroundImage(){
    //图片初始化
    QImage image = QImage(filename);
    if(image.isNull())
        ui->statusBar->showMessage("open Image Failed!",3000);
    else{
        ui->statusBar->showMessage("open Image Success!",3000);

        //将图片转化为自适应Label的大小,再加进Label:E_Card_BackgroundImage_Label
        QLabel* label = ui->E_Card_BackgroundImage_Label;
        chooseImage = image.scaled(label->width(),label->height(),Qt::IgnoreAspectRatio);
        label->setPixmap(QPixmap::fromImage(chooseImage));
    }
}

void MainWindow::on_chooseImage_Btn_clicked()
{
    //获得图片的绝对路径
    this->filename = QFileDialog::getOpenFileName(this,tr("chooseImage"),
                                                "/home",tr("Images(*jpg *xpm *png)"));
    setBackgroundImage();
}

void MainWindow::on_page_3_previous_Btn_clicked()
{
    previousPage();
}

void MainWindow::copyECardTitle(QLabel* title1,QLabel* title2){
    title1->setText(title2->text());
    title1->setAlignment(Qt::AlignCenter);
    /*
    此方法也可以设置颜色
    QPalette palette;
    palette.setColor(QPalette::Background,title2->palette().background().color());
    palette.setColor(QPalette::WindowText,title2->palette().windowText().color());
    title1->setAutoFillBackground(true);
    title1->setPalette(palette);
    */
    title1->setStyleSheet(title2->styleSheet());
    title1->setFont(title2->font());
}

bool MainWindow::loadData()
{
    QFile file("loadData.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    bool flag = false;
    if(!file.isOpen()){
        flag = false;
    }
    else{
        QTextStream in(&file);
        QString str;
        if (!in.atEnd()){
            in >> str;
        }
        flag = (str.toInt() == 1);
    }
    return flag;
}

void MainWindow::loadData_ECard()
{
    card = dao.getECard();
    ui->E_Card_Title_Label->setText(card.getTitle());
    ui->E_Card_Title_Label->setAlignment(Qt::AlignCenter);
    ui->page_4_cartInfo_Text->setPlainText(card.getCardInfo());
    ui->stackedWidget->setCurrentIndex(card.getCurrentStep());
    filename = card.getImage();
    setBackgroundImage();
    ui->page_3_imageBackground_Label->setPixmap(QPixmap::fromImage(chooseImage));
    ui->page_4_imageBackground_Label->setPixmap(QPixmap::fromImage(chooseImage));
}

void MainWindow::loadData_Color()
{
    QStringList list = dao.getColorNames(card.getId());
    QString str = "background-color:" + list[0] + ";color:" + list[1];
    ui->E_Card_Title_Label->setStyleSheet(str);
    str = "background:transparent;color:" + list[2];
    ui->page_4_cartInfo_Text->setStyleSheet(str);
}

void MainWindow::loadData_Font()
{
    int cardId = card.getId();
    ui->E_Card_Title_Label->setFont(dao.getFont(cardId,3));
    ui->page_4_cartInfo_Text->setFont(dao.getFont(cardId,4));
    copyECardTitle(ui->page_4_title_Label,ui->E_Card_Title_Label);
}

void MainWindow::loadData_Music()
{
    music = dao.getMusic(card.getId());
    musicPath = music.getMusicPath();
    ui->music_volumn_VSlider->setValue(music.getVolume());
}

void MainWindow::loadData_MusicNameList()
{
    QStringList list = dao.getMusicNames(card.getId());
    ui->music_musicList_ListWidget->addItems(list);
    QListWidgetItem* currentItem = ui->music_musicList_ListWidget->item(music.getCurrentMusic());
    currentItem->setSelected(true);
    ui->music_musicList_ListWidget->itemDoubleClicked(currentItem);
    QEventLoop eventloop;
    QTimer::singleShot(500, &eventloop, SLOT(quit()));
    eventloop.exec();
    m_mediaPlayer.setPosition(m_mediaPlayer.duration() * music.getPlayingProgress() / 100);
    on_music_start_Btn_clicked();
    /*
    此处暂停500毫秒的原因: 为了让m_mediaPlayer.setPosition起作用。 不暂停的话不知什么原因 代码会失效。
    m_mediaPlayer.setPosition的作用: 复现用户上次操作时播放音乐的进度条。
    不同机器这里设置的时间可能会不同 当我设置休眠时间较短时,m_mediaPlayer.setPosition仍不起作用 500毫秒就能正常运行了。
    猜测代码失效的原因:
        QMediaPlayer 内部信号发射的问题??
    QEventLoop eventloop;
    QTimer::singleShot(300, &eventloop, SLOT(quit()));
    eventloop.exec();
    */
}

void MainWindow::beforeClose()
{
    QFile file("loadData.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    if(!file.isOpen()){
        QMessageBox::about(NULL,"反馈","数据文件打开失败!");
        return;
    }

    QTextStream out(&file);
    out << saveFlag;
    file.close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(saveFlag == false){
        int choose;
        choose = QMessageBox::question(this, tr("退出程序"),
                                       QString(tr("退出程序前是否保存此次登录的操作?")),
                                       QMessageBox::Yes | QMessageBox::No);
        if(choose == QMessageBox::Yes)
            on_page_4_save_Btn_clicked();
    }
    beforeClose();
}

void MainWindow::on_page_3_next_Btn_clicked()
{
    nextPage();
    copyECardTitle(ui->page_4_title_Label,ui->E_Card_Title_Label);
    ui->page_4_imageBackground_Label->setPixmap(QPixmap::fromImage(chooseImage));
}

void MainWindow::on_page_3_setTitle_Btn_clicked()
{
    bool flag = false;
    QString title = QInputDialog::getText(this,"设置标题","贺卡的标题",QLineEdit::Normal,"",&flag).trimmed();
    if(flag && !title.isEmpty()){
        QLabel* label = ui->E_Card_Title_Label;
        label->setText(title);
        label->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_page_3_backgroundColor_Btn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        QLabel* label = ui->E_Card_Title_Label;
        QString str = "background-color:" + color.name() + ";color:" + label->palette().windowText().color().name();
        label->setStyleSheet(str);
    }
}

void MainWindow::on_page_3_fontStyle_Btn_clicked()
{
    bool flag = false;
    QFont font = QFontDialog::getFont(&flag, this);
    if(flag){
        ui->E_Card_Title_Label->setFont(font);
    }
}

void MainWindow::on_page_3_fontColor_Btn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        QLabel* label = ui->E_Card_Title_Label;
        QString str = "background-color:" + label->palette().background().color().name() + ";color:" + color.name();
        label->setStyleSheet(str);
    }
}

void MainWindow::on_page_4_previous_Btn_clicked()
{
    previousPage();
}

void MainWindow::on_page_4_cardInfoFont_Btn_clicked()
{
    bool flag = false;
    QFont font = QFontDialog::getFont(&flag, this);
    if(flag){
        ui->page_4_cartInfo_Text->setFont(font);
    }
}

void MainWindow::on_page_4_cartInfoColor_Btn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        QString str = "background:transparent;color:";
        str.append(color.name());
        ui->page_4_cartInfo_Text->setStyleSheet(str);
    }
}

void MainWindow::setPlayingSliderValue(){
    /*
     * m_mediaPlayer.position() 当前音乐播放的毫秒数
     * m_mediaPlayer.duration() 音乐的总时长（毫秒）
     */
    int currentTime = m_mediaPlayer.position();
    int allTime = m_mediaPlayer.duration();
    if(currentTime >= allTime)
        on_music_next_Btn_clicked();
    else
        ui->music_Playing_HSlider->setValue(currentTime * 100 / allTime);
}

void MainWindow::on_music_start_Btn_clicked()
{
    if(QMediaPlayer::PausedState == m_mediaPlayer.state()){
        m_mediaPlayer.play();
        ui->music_start_Btn->setText("暂停");
    }
    else{
        m_mediaPlayer.pause();
        ui->music_start_Btn->setText("播放");
    }
}

void MainWindow::on_music_Playing_HSlider_sliderMoved(int position)
{
    /*
        m_mediaPlayer.duration()：音频文件的总帧数（当然理解成时长(总毫秒数)也可以）
        position：是当前滑动条的位置，默认滑动条的总长度是0到99。
        m_mediaPlayer.duration()*position/100：将音乐时长100等分，用于匹配进度条。
        m_mediaPlayer.setPosition():设置从哪一帧开始播放。
    */
    m_mediaPlayer.setPosition(m_mediaPlayer.duration() * position / 100);
}

void MainWindow::on_music_volumn_VSlider_sliderMoved(int position)
{
    m_mediaPlayer.setVolume(position);
}

void MainWindow::on_music_addMusic_Btn_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(NULL,"Select Music Dir",".",NULL);
    QDir dir(filename);
    QStringList nameFilters;
    nameFilters << "*.mp3";
    QStringList files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Name);
    ui->music_musicList_ListWidget->addItems(files);
    this->musicPath = filename;
}

void MainWindow::on_music_musicList_ListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_mediaPlayer.stop();
    m_mediaPlayer.setMedia(QUrl::fromLocalFile(this->musicPath+ "/" + item->text()));
    m_mediaPlayer.play();
    ui->music_Playing_HSlider->setValue(0);
    this->musicCurrentRow = ui->music_musicList_ListWidget->row(item);
    ui->music_start_Btn->setText("暂停");
    connect(timer,SIGNAL(timeout()),this,SLOT(setPlayingSliderValue()));
    connect(&m_mediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(setAllTime()));   //音乐播放的同时设置总时长
    connect(&m_mediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(setPlayTime()));  //设置音乐播放了多久
}

void MainWindow::on_music_previous_Btn_clicked()
{
    this->musicCurrentRow = (this->musicCurrentRow == 0 ? ui->music_musicList_ListWidget->count() - 1 : this->musicCurrentRow - 1);
    QListWidgetItem* item = ui->music_musicList_ListWidget->item(this->musicCurrentRow);
    item->setSelected(true);
    ui->music_musicList_ListWidget->itemDoubleClicked(item);
}

void MainWindow::on_music_next_Btn_clicked()
{
    this->musicCurrentRow = (this->musicCurrentRow + 1) % ui->music_musicList_ListWidget->count();
    QListWidgetItem* item = ui->music_musicList_ListWidget->item(this->musicCurrentRow);
    item->setSelected(true);
    ui->music_musicList_ListWidget->itemDoubleClicked(item);
}

void MainWindow::on_music_volumn_Btn_clicked()
{
    ui->music_volumn_VSlider->setVisible(true);
}

void MainWindow::on_music_volumn_VSlider_sliderReleased()
{
    ui->music_volumn_VSlider->setVisible(false);
}

QString MainWindow::getTime(int time){
    time /= 1000;   //毫秒转秒
    time %= 3600;
    int m = time / 60;
    int s = time % 60;
    //arg(整数类型,补位数,进制,补位字符);
    return QString("%1:%2").arg(m,2,10,QLatin1Char('0')).arg(s,2,10,QLatin1Char('0'));
}

void MainWindow::setPlayTime(){
    ui->music_playTime_Label->setText(getTime(m_mediaPlayer.position()));
}

void MainWindow::setAllTime(){
    ui->music_allTime_Label->setText(getTime(m_mediaPlayer.duration()));
}

void MainWindow::updateECardEntity(){
    card.setTitle(ui->E_Card_Title_Label->text());
    card.setImage(filename);
    card.setCardInfo(ui->page_4_cartInfo_Text->toPlainText());
    card.setDate(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    card.setCurrentStep(ui->stackedWidget->currentIndex());
}

void MainWindow::updateMusicEntity(int cardId)
{
    music.setCardId(cardId);
    music.setMusicPath(musicPath);
    music.setPlayTime(ui->music_playTime_Label->text());
    music.setAllTime(ui->music_allTime_Label->text());
    music.setPlayingProgress(ui->music_Playing_HSlider->value());
    music.setVolume(ui->music_volumn_VSlider->value());
    music.setCurrentMusic(musicCurrentRow);
}

void MainWindow::on_page_4_save_Btn_clicked()
{
    int cardId = card.getId();     //贺卡编号

    //更新贺卡的实例对象
    updateECardEntity();
    //更新数据库
    if(cardId != 0)
        dao.updateECard(card); //存在贺卡,更新它
    else
        dao.insertECard(card); //不存在贺卡,新建

    //更新贺卡实例对象的id值
    if(cardId == 0){
        cardId = dao.getCardId();
        card.setId(cardId);
    }

    //若颜色库中有贺卡id的记录 则更新
    if(dao.table_hasCardId("t_color", cardId)){
        QPalette p = ui->E_Card_Title_Label->palette();
        dao.updateColor(cardId,1,p.background().color().name());  //标题背景
        dao.updateColor(cardId,2,p.windowText().color().name());  //标题字体
        dao.updateColor(cardId,3,ui->page_4_cartInfo_Text->palette().text().color().name()); //贺卡问候语
    }
    //否则插入数据
    else{
        QPalette p = ui->E_Card_Title_Label->palette();
        dao.insertColor(cardId,1,p.background().color().name());  //标题背景
        dao.insertColor(cardId,2,p.windowText().color().name());  //标题字体
        dao.insertColor(cardId,3,ui->page_4_cartInfo_Text->palette().text().color().name()); //贺卡问候语
    }

    //若字体库中有贺卡id的记录 则更新
    if(dao.table_hasCardId("t_font", cardId)){
        dao.updateFont(cardId,3,ui->E_Card_Title_Label->font());
        dao.updateFont(cardId,4,ui->page_4_cartInfo_Text->font());
    }
    //否则 插入数据
    else{
        dao.insertFont(cardId,3,ui->E_Card_Title_Label->font());
        dao.insertFont(cardId,4,ui->page_4_cartInfo_Text->font());
    }

    //音乐部分
    updateMusicEntity(cardId);
    //若音乐库中有贺卡id的记录 则更新
    if(dao.table_hasCardId("t_music", cardId))
        dao.updateMusic(music);
    //否则 插入数据
    else
        dao.insertMusic(music);

    if(dao.table_hasCardId("t_musicNameList",cardId))
        dao.deleteMusicNameList(cardId);
    dao.insertMusicNameList(cardId,ui->music_musicList_ListWidget);

    saveFlag = true;

    QMessageBox::about(this,"提示","保存成功!");
}
