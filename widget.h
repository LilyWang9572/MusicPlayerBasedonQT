#ifndef WIDGET_H
#define WIDGET_H
#include<QWidget>
#include<QMediaPlayer>
#include<QMediaPlaylist>
namespace Ui {
class Widget;
}
class Widget:public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
        ~Widget();
private slots:
    void addSongs();
    void playPause();
    void playStop();
    void playNext();
    void playPrevious();
    void posChanged(qint64 pos);
    void durChanged(qint64 dur);
    void setPos(int pos);
    void clearList();
    void deleteSong();
    void volumeSilence();
    void setVol(int vol);
    void hideShow();
    void reduceVol();
    void addVol();
private:
    Ui::Widget *ui;
private:
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    QAction *action_hideShow;
    QAction *action_addSongs;
    QAction *action_playPause;
    QAction *action_stop;
    QAction *action_previous;
    QAction *action_next;
    QAction *action_addVolume;
    QAction *action_reduceVolume;
    QAction *action_quit;
protected:
    void contextMenuEvent(QContextMenuEvent*);


};

#endif // WIDGET_H
