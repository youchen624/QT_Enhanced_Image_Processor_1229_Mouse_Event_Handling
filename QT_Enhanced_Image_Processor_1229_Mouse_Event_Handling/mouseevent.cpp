#include "mouseevent.h"
#include "zoomwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MouseEvent::MouseEvent(QWidget *parent)
    : QMainWindow(parent)
    , rubberBand(nullptr)
    , isSelecting(false)
{
    setWindowTitle (QStringLiteral("測試滑鼠事件"));
    statusLabel = new QLabel;
    statusLabel->setText (QStringLiteral("指標位置"));
    statusLabel->setFixedWidth (100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth (100);
    statusBar()->addPermanentWidget (statusLabel);
    statusBar()->addPermanentWidget (MousePosLabel);
    
    setWindowTitle(QStringLiteral("測試滑鼠事件"));

    // 建立主視窗的中央元件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 工具列
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    // 按鈕
    importButton = new QPushButton(QStringLiteral("開啟"), this);
    connect(importButton, &QPushButton::clicked, this, &MouseEvent::importImage);
    toolbarLayout->addWidget(importButton);
    
    // 放大模式複選框
    zoomModeCheckbox = new QCheckBox(QStringLiteral("放大模式"), this);
    connect(zoomModeCheckbox, &QCheckBox::toggled, this, &MouseEvent::toggleZoomMode);
    toolbarLayout->addWidget(zoomModeCheckbox);
    
    // 放大倍率
    zoomRatioLabel = new QLabel(QStringLiteral("放大倍率:"), this);
    toolbarLayout->addWidget(zoomRatioLabel);
    
    zoomRatioSpinBox = new QSpinBox(this);
    zoomRatioSpinBox->setMinimum(1);
    zoomRatioSpinBox->setMaximum(10);
    zoomRatioSpinBox->setValue(2);
    zoomRatioSpinBox->setSuffix("x");
    toolbarLayout->addWidget(zoomRatioSpinBox);
    
    toolbarLayout->addStretch();
    
    layout->addLayout(toolbarLayout);

    // 圖片顯示區域
    inWin = new QLabel(this);
    inWin->setAlignment(Qt::AlignCenter); // 圖片置中
    inWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    inWin->setMinimumSize(1, 1); // 允許縮到很小，避免卡住佈局
    layout->addWidget(inWin);
    
    if (srcImg.isNull())
    {
        QPixmap *initPixmap= new QPixmap(300,200);
        initPixmap->fill(QColor(255,255,255));
        inWin->setPixmap(*initPixmap);
    }
    
    setMouseTracking(true);
    resize (800,600);
}

MouseEvent::~MouseEvent() {
    if (rubberBand) {
        delete rubberBand;
    }
}
void MouseEvent::importImage() {
    // if(srcImg.isNull()) return;
    try {
        QString path = QFileDialog::getOpenFileName(
            this,
            tr("請選擇要開啟的圖片"),
            QDir::homePath(),
            tr("(*.png);;(*.jpg);;(*.jpeg);;All(*)")
            );
        if (srcImg.load(path)) {
            inWin->setPixmap(QPixmap::fromImage(srcImg));
        }
    } catch (QString e) {
        qDebug() << "Catched an error: \n" <<e;
    }
}

void MouseEvent::toggleZoomMode(bool checked) {
    if (checked) {
        inWin->setCursor(Qt::CrossCursor);
        statusBar()->showMessage(QStringLiteral("放大模式已啟用 - 拖曳選取要放大的區域"));
    } else {
        inWin->setCursor(Qt::ArrowCursor);
        if (rubberBand) {
            rubberBand->hide();
        }
        isSelecting = false;
        statusBar()->showMessage(QStringLiteral("放大模式已停用"));
    }
}

void MouseEvent::mouseDoubleClickEvent (QMouseEvent *) {

};
void MouseEvent::mouseMoveEvent(QMouseEvent *event) {
    if (!srcImg.isNull()) {
        // Get the position relative to the image label
        QPoint labelPos = inWin->mapFromParent(event->pos());
        
        // Check if the position is within the image bounds
        if (inWin->rect().contains(labelPos) && 
            labelPos.x() >= 0 && labelPos.y() >= 0 &&
            labelPos.x() < srcImg.width() && labelPos.y() < srcImg.height()) {
            int g = qGray(srcImg.pixel(labelPos.x(), labelPos.y()));
            QString str = "(" + QString::number(labelPos.x()) +
                          ", " + QString::number(labelPos.y()) + ") 灰階值: " + QString::number(g);
            MousePosLabel->setText(str);
        }
    }
    
    // Handle rubber band during selection
    if (isSelecting && rubberBand) {
        QPoint currentPos = inWin->mapFromParent(event->pos());
        rubberBand->setGeometry(QRect(selectionStart, currentPos).normalized());
    }
}
void MouseEvent::mousePressEvent (QMouseEvent *event) {
    QString str = "(" + QString::number (event->x()) +", "+
                  QString::number (event->y()) +")";
    
    if (event->button() == Qt::LeftButton)
    {
        // If zoom mode is enabled, start selection
        if (zoomModeCheckbox->isChecked() && !srcImg.isNull()) {
            isSelecting = true;
            selectionStart = inWin->mapFromParent(event->pos());
            
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, inWin);
            }
            rubberBand->setGeometry(QRect(selectionStart, QSize()));
            rubberBand->show();
        } else {
            statusBar()->showMessage (QStringLiteral ("左鍵:")+str);
        }
    } else if (event->button()== Qt:: RightButton) {
        statusBar()->showMessage (QStringLiteral("右鍵:")+str);
    } else if (event->button()== Qt::MiddleButton) {
        statusBar()->showMessage (QStringLiteral("中鍵:")+str);
    }
};
void MouseEvent::mouseReleaseEvent (QMouseEvent *event) {
    QString str = "("+ QString::number (event->x()) +", "+
    QString::number (event->y()) +")";
    statusBar()->showMessage (QStringLiteral("釋放:")+str);
    
    // Handle zoom mode
    if (isSelecting && event->button() == Qt::LeftButton) {
        isSelecting = false;
        
        if (rubberBand) {
            QRect selection = rubberBand->geometry();
            rubberBand->hide();
            
            // Check if selection is valid (has some size)
            if (selection.width() > 10 && selection.height() > 10) {
                // Get the selected region from the original image
                QRect imageRect = inWin->rect();
                
                // Calculate the scaling factor between displayed and actual image
                double scaleX = static_cast<double>(srcImg.width()) / imageRect.width();
                double scaleY = static_cast<double>(srcImg.height()) / imageRect.height();
                
                // Map selection to actual image coordinates
                int x = static_cast<int>(selection.x() * scaleX);
                int y = static_cast<int>(selection.y() * scaleY);
                int w = static_cast<int>(selection.width() * scaleX);
                int h = static_cast<int>(selection.height() * scaleY);
                
                // Ensure we don't go out of bounds
                x = qMax(0, qMin(x, srcImg.width() - 1));
                y = qMax(0, qMin(y, srcImg.height() - 1));
                w = qMin(w, srcImg.width() - x);
                h = qMin(h, srcImg.height() - y);
                
                // Extract the selected region
                QImage selectedRegion = srcImg.copy(x, y, w, h);
                
                if (!selectedRegion.isNull()) {
                    // Get zoom ratio
                    double zoomRatio = zoomRatioSpinBox->value();
                    
                    // Create and show zoom window
                    ZoomWindow *zoomWin = new ZoomWindow(selectedRegion, zoomRatio, this);
                    zoomWin->show();
                }
            }
        }
    }
};
