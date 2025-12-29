#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QRubberBand>
#include <QSpinBox>
#include <QCheckBox>

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
    
    // Region selection for zoom
    QRubberBand     *rubberBand;
    QPoint          selectionStart;
    bool            isSelecting;
    QCheckBox       *zoomModeCheckbox;
    QSpinBox        *zoomRatioSpinBox;
    QLabel          *zoomRatioLabel;

private slots:
    void importImage();
    void toggleZoomMode(bool checked);
};
#endif // MOUSEEVENT_H
