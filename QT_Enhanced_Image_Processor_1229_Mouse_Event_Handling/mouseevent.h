#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>

class MouseEvent : public QMainWindow
{
    Q_OBJECT

public:
    MouseEvent(QWidget *parent = nullptr);
    ~MouseEvent();
protected:
    void mouseDoubleClickEvent (QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

private:
    QPushButton     *importButton;
    QLabel          *inWin;
    QLabel * statusLabel;
    QLabel * MousePosLabel;
    QImage          srcImg;
    QImage          dstImg;

private slots:
    void importImage();
};
#endif // MOUSEEVENT_H
