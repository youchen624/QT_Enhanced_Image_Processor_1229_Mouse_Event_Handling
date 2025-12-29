#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QDialog>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QSpinBox>
#include <QCheckBox>
#include <QColorDialog>

class ZoomWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ZoomWindow(const QImage &image, double zoomRatio = 2.0, QWidget *parent = nullptr);
    ~ZoomWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void saveImage();
    void togglePenMode(bool checked);
    void selectPenColor();
    void updatePenSize(int size);

private:
    QImage originalImage;
    QImage displayImage;
    QLabel *imageLabel;
    QPushButton *saveButton;
    QPushButton *colorButton;
    QCheckBox *penModeCheckbox;
    QSpinBox *penSizeSpinBox;
    
    bool penMode;
    bool drawing;
    QPoint lastPoint;
    QColor penColor;
    int penSize;
    
    void drawLineTo(const QPoint &endPoint);
    void updateDisplay();
};

#endif // ZOOMWINDOW_H
