#include "zoomwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPainter>
#include <QScrollArea>
#include <QMessageBox>

ZoomWindow::ZoomWindow(const QImage &image, double zoomRatio, QWidget *parent)
    : QDialog(parent)
    , originalImage(image)
    , penMode(false)
    , drawing(false)
    , penColor(Qt::red)
    , penSize(3)
{
    setWindowTitle(QStringLiteral("放大圖片 - 編輯視窗"));
    
    // Scale the image
    int newWidth = static_cast<int>(image.width() * zoomRatio);
    int newHeight = static_cast<int>(image.height() * zoomRatio);
    displayImage = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    // Save button
    saveButton = new QPushButton(QStringLiteral("另存新檔"), this);
    connect(saveButton, &QPushButton::clicked, this, &ZoomWindow::saveImage);
    toolbarLayout->addWidget(saveButton);
    
    // Pen mode checkbox
    penModeCheckbox = new QCheckBox(QStringLiteral("畫筆模式"), this);
    connect(penModeCheckbox, &QCheckBox::toggled, this, &ZoomWindow::togglePenMode);
    toolbarLayout->addWidget(penModeCheckbox);
    
    // Pen size
    QLabel *penSizeLabel = new QLabel(QStringLiteral("筆刷大小:"), this);
    toolbarLayout->addWidget(penSizeLabel);
    
    penSizeSpinBox = new QSpinBox(this);
    penSizeSpinBox->setMinimum(1);
    penSizeSpinBox->setMaximum(50);
    penSizeSpinBox->setValue(penSize);
    penSizeSpinBox->setEnabled(false);
    connect(penSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &ZoomWindow::updatePenSize);
    toolbarLayout->addWidget(penSizeSpinBox);
    
    // Color button
    colorButton = new QPushButton(QStringLiteral("選擇顏色"), this);
    colorButton->setEnabled(false);
    connect(colorButton, &QPushButton::clicked, this, &ZoomWindow::selectPenColor);
    toolbarLayout->addWidget(colorButton);
    
    toolbarLayout->addStretch();
    
    mainLayout->addLayout(toolbarLayout);
    
    // Create scroll area for image
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false);
    
    // Image label
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap::fromImage(displayImage));
    imageLabel->setMouseTracking(true);
    
    scrollArea->setWidget(imageLabel);
    mainLayout->addWidget(scrollArea);
    
    // Set window size
    resize(800, 600);
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (penMode && event->button() == Qt::LeftButton) {
        // Get relative position to image label
        QPoint globalPos = event->globalPos();
        QPoint labelPos = imageLabel->mapFromGlobal(globalPos);
        
        if (imageLabel->rect().contains(labelPos)) {
            drawing = true;
            lastPoint = labelPos;
        }
    }
    QDialog::mousePressEvent(event);
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (penMode && drawing && (event->buttons() & Qt::LeftButton)) {
        QPoint globalPos = event->globalPos();
        QPoint labelPos = imageLabel->mapFromGlobal(globalPos);
        
        if (imageLabel->rect().contains(labelPos)) {
            drawLineTo(labelPos);
        }
    }
    QDialog::mouseMoveEvent(event);
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (penMode && event->button() == Qt::LeftButton && drawing) {
        drawing = false;
    }
    QDialog::mouseReleaseEvent(event);
}

void ZoomWindow::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&displayImage);
    painter.setPen(QPen(penColor, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    
    lastPoint = endPoint;
    updateDisplay();
}

void ZoomWindow::updateDisplay()
{
    imageLabel->setPixmap(QPixmap::fromImage(displayImage));
}

void ZoomWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("另存圖片"),
        QDir::homePath(),
        tr("PNG Files (*.png);;JPEG Files (*.jpg *.jpeg);;All Files (*)")
    );
    
    if (!fileName.isEmpty()) {
        if (displayImage.save(fileName)) {
            QMessageBox::information(this, QStringLiteral("成功"), 
                                   QStringLiteral("圖片已成功儲存！"));
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"), 
                               QStringLiteral("圖片儲存失敗！"));
        }
    }
}

void ZoomWindow::togglePenMode(bool checked)
{
    penMode = checked;
    penSizeSpinBox->setEnabled(checked);
    colorButton->setEnabled(checked);
    
    if (checked) {
        imageLabel->setCursor(Qt::CrossCursor);
    } else {
        imageLabel->setCursor(Qt::ArrowCursor);
        drawing = false;
    }
}

void ZoomWindow::selectPenColor()
{
    QColor color = QColorDialog::getColor(penColor, this, QStringLiteral("選擇畫筆顏色"));
    if (color.isValid()) {
        penColor = color;
    }
}

void ZoomWindow::updatePenSize(int size)
{
    penSize = size;
}
