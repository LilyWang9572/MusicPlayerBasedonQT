#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/interface.jpg"))); //*.jpg
    this->setPalette(palette);
    ui->volumeSlider->setRange(0, 100);
    this->setWindowTitle(tr("MusicPlayer"));
    this->playList = new QMediaPlaylist(this);
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    this->player = new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->setVolume(50);
    ui->volumeSlider->setValue(50);
    ui->addSongsButton->setStyleSheet("QPushButton{border-radius:5px; border-width:0px;}");
    ui->clearButton->setStyleSheet("QPushButton{border-radius:5px; border-width:0px;}");
    ui->deleteButton->setStyleSheet("QPushButton{border-radius:5px; border-width:0px;}");
    ui->volumeButton->setStyleSheet("QPushButton{border-radius:5px; border-width:0px;}");

    QObject::connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVol(int)));
    QObject::connect(ui->addSongsButton, SIGNAL(clicked()), this, SLOT(addSongs()));
    QObject::connect(ui->playPauseButton, SIGNAL(clicked()), this, SLOT(playPause()));
    QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(playStop()));
    QObject::connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(playPrevious()));
    QObject::connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(playNext()));
    QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(durChanged(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(posChanged(qint64)));
    QObject::connect(ui->processSlider, SIGNAL(valueChanged(int)), this, SLOT(setPos(int)));
    QObject::connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearList()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSong()));
    QObject::connect(ui->volumeButton, SIGNAL(clicked()), this, SLOT(volumeSilence()));

    //
    ui->stopButton->setStyleSheet("border:2pxgroovegray; border-radius:10px; padding:2px4px;");
    ui->playPauseButton->setStyleSheet("border:2pxgroovegray; border-radius:10px; padding:2px4px;");
    ui->playPauseButton->setIcon(QIcon(":/images/play.png"));
    ui->playPauseButton->setText(tr("播放"));
    ui->previousButton->setStyleSheet("border:2pxgroovegray; border-radius:10px; padding:2px4px;");
    ui->previousButton->setIcon(QIcon(":/images/previous.png"));
    ui->nextButton->setStyleSheet("border:2pxgroovegray; border-radius:10px; padding:2px4px;");
    ui->nextButton->setIcon(QIcon(":/images/next.png"));
    ui->stopButton->setIcon(QIcon(":/images/stop.png"));
    ui->volumeButton->setIcon(QIcon(":/images/volume.png"));
    ui->presentTimeLabel->setText("0:00");
    ui->totalTimeLabel->setText("0:00");
    ui->volumeButton->setText(tr("音量"));
   //
    action_hideShow = new QAction(tr("hide/show window"),this);
    QObject::connect(action_hideShow, SIGNAL(triggered()), this, SLOT(hideShow()));
    action_addSongs = new QAction(tr("添加本地歌曲"),this);

    QObject::connect(action_addSongs, SIGNAL(triggered()), this, SLOT(addSongs()));
    action_playPause = new QAction(tr("播放/暂停"), this);

    QObject::connect(action_playPause, SIGNAL(triggered()), this, SLOT(playPause()));
    action_stop = new QAction(tr("停止"), this);

    QObject::connect(action_stop, SIGNAL(triggered()), this, SLOT(playStop()));
    action_previous = new QAction(tr("上一曲"), this);

    QObject::connect(action_previous, SIGNAL(triggered()), this, SLOT(playPrevious()));
    action_next = new QAction(tr("下一曲"), this);

    QObject::connect(action_next, SIGNAL(triggered()), this, SLOT(playNext()));
    action_addVolume = new QAction(tr("音量+(5%)"), this);

    QObject::connect(action_addVolume, SIGNAL(triggered()), this, SLOT(addVol()));
    action_reduceVolume = new QAction(tr("音量-(5%)"), this);

    QObject::connect(action_reduceVolume, SIGNAL(triggered()), this, SLOT(reduceVol()));
    action_quit = new QAction(tr("退出"), this);
    QObject::connect(action_quit, SIGNAL(triggered()), this, SLOT(close()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::setVol(int vol)
{
    player->setVolume(vol);
    QString volumePercent=QString::number(ui->volumeSlider->value());
    volumePercent+='%';
    ui->volumeSlider->setStatusTip(volumePercent);
}

void Widget::addSongs()
{
    QStringList songsList=QFileDialog::getOpenFileNames(this,tr("选择文件"),QDir::homePath(),tr("*.mp3"));
    for(int i=0;i<songsList.size();i++)
    {
        QString song=songsList.at(i);
        this->playList->addMedia(QUrl::fromLocalFile(song));
        QStringList songL=song.split('/');
        song=songL.last();
        ui->songsList->addItem(song);
    }


}



void Widget::playPause()
{
    if(tr("播放")==ui->playPauseButton->text())
    {

        int index=ui->songsList->currentRow();
        if(-1!=index)
        {
            playList->setCurrentIndex(index);
            player->play();
            ui->playPauseButton->setText(tr("暂停"));
            ui->playPauseButton->setIcon(QIcon(":/images/pause.png"));

        }

    }
    else
    {
        player->pause();
        ui->playPauseButton->setText(tr("播放"));
        ui->playPauseButton->setIcon(QIcon(":/images/play.png"));
    }
}

void Widget::playStop()
{

    player->stop();
    ui->playPauseButton->setText(tr("播放"));
    ui->playPauseButton->setIcon(QIcon(":/images/play.png"));
}

void Widget::playPrevious()
{
    player->stop();
    int index=playList->currentIndex();
    if(0==index)
        index=playList->mediaCount()-1;
    else
        index--;
    playList->setCurrentIndex(index);
    ui->songsList->setCurrentRow(index);
    player->play();
}

void Widget::playNext()
{
    player->stop();
    int index=playList->currentIndex();
    if(index==playList->mediaCount()-1)
        index=0;
    else
        index++;
    playList->setCurrentIndex(index);
    ui->songsList->setCurrentRow(index);
    player->play();

}

void Widget::durChanged(qint64 dur)
{
    ui->processSlider->setRange(0,dur);
    QString totalTime;
    dur/=1000;
    totalTime+=QString::number(dur/60);
    totalTime+=':';
    if(dur%60<10)
        totalTime+='0';
    totalTime+=QString::number(dur%60);
    ui->totalTimeLabel->setText(totalTime);
}

void Widget::posChanged(qint64 pos)
{
    ui->processSlider->setValue(pos);
    QString presentTime;
    pos/=1000;
    presentTime+=QString::number(pos/60);
    presentTime+=':';
    if(pos%60<10)
        presentTime+='0';
    presentTime+=QString::number(pos%60);
    ui->presentTimeLabel->setText(presentTime);

}

void Widget::setPos(int pos)
{
    player->setPosition(pos);
}

void Widget::clearList()
{
    playList->clear();
    ui->songsList->clear();
}

void Widget::deleteSong()
{
    int index=ui->songsList->currentRow();
    if(-1!=index)
    {
        ui->songsList->takeItem(index);
        playList->removeMedia(index);
    }
}

void Widget::volumeSilence()
{
    if(ui->volumeButton->text()==tr("音量"))
    {
        ui->volumeButton->setText(tr("静音"));
        ui->volumeSlider->setValue(0);
        ui->volumeButton->setIcon(QIcon(":/images/silence.png"));

    }
    else
    {
        ui->volumeButton->setText(tr("音量"));
        ui->volumeButton->setIcon(QIcon(":/images/volume.png"));
        ui->volumeSlider->setValue(50);
    }

}

void Widget::contextMenuEvent(QContextMenuEvent*)
{
    QMenu* menu=new QMenu(this);

    menu->addAction(action_hideShow);
    menu->addAction(action_addSongs);
    menu->addSeparator();
    menu->addAction(action_playPause);
    menu->addAction(action_stop);
    menu->addAction(action_previous);
    menu->addAction(action_next);
    menu->addSeparator();
    menu->addAction(action_addVolume);
    menu->addAction(action_reduceVolume);
    menu->addSeparator();
    menu->addAction(action_quit);

    menu->move(cursor().pos());
    menu->show();
}

void Widget::hideShow()
{
    if(ui->songsList->isVisible())
    {
        ui->addSongsButton->setVisible(false);
        ui->clearButton->setVisible(false);
        ui->deleteButton->setVisible(false);
        ui->songsList->setVisible(false);
        ui->processSlider->setVisible(false);
        ui->presentTimeLabel->setVisible(false);
        ui->totalTimeLabel->setVisible(false);
        ui->volumeButton->setVisible(false);
        ui->volumeSlider->setVisible(false);

    }
    else
    {
        ui->addSongsButton->setVisible(true);
        ui->clearButton->setVisible(true);
        ui->deleteButton->setVisible(true);
        ui->songsList->setVisible(true);
        ui->processSlider->setVisible(true);
        ui->presentTimeLabel->setVisible(true);
        ui->totalTimeLabel->setVisible(true);
        ui->volumeButton->setVisible(true);
        ui->volumeSlider->setVisible(true);
    }

}

void Widget::addVol()
{
    int vol=ui->volumeSlider->value();
    vol+=5;
    if(vol>100)
        vol=100;
    ui->volumeSlider->setValue(vol);
}


void Widget::reduceVol()
{
    int vol=ui->volumeSlider->value();
    vol-=5;
    if(vol<0)
        vol=0;
    ui->volumeSlider->setValue(vol);
}

















