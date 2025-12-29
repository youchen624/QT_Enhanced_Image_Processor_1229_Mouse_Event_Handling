#include "mouseevent.h"
#include <QFileDialog>
#include <QVBoxLayout>

MouseEvent::MouseEvent(QWidget *parent)
    : QMainWindow(parent)
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
    //
    /*
    inWin = new QLabel(this);
    // inWin->setScaledContents(true);

    QPixmap *initPixmap= new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));
    inWin->setPixmap(*initPixmap);
*/
    setWindowTitle(QStringLiteral("測試滑鼠事件"));

    // 建立主視窗的中央元件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 按鈕
    importButton = new QPushButton(QStringLiteral("開啟"), this);
    layout->addWidget(importButton);

    // 圖片顯示區域
    inWin = new QLabel(this);
    inWin->setAlignment(Qt::AlignCenter); // 圖片置中
    inWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    inWin->setMinimumSize(1, 1); // 允許縮到很小，避免卡住佈局
    layout->addWidget(inWin);
    // inWin->setSizePolicy(QSizePolicy:: Expanding, QSizePolicy:: Expanding);
    if (srcImg.isNull())
    {
        QPixmap *initPixmap= new QPixmap(300,200);
        initPixmap->fill(QColor(255,255,255));
        inWin->setPixmap(*initPixmap);
    }
    importButton = new QPushButton(QStringLiteral("開啟"),this);
    connect(importButton, SIGNAL(clicked()), this, SLOT(importImage()));
    setMouseTracking(true);
    resize (400,300);
}

MouseEvent::~MouseEvent() {}
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
void MouseEvent::mouseDoubleClickEvent (QMouseEvent *event) {

};
void MouseEvent::mouseMoveEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    // qDebug() << "moved";
    int g = qGray(srcImg.pixel(x,y));
    QString str = "(" + QString::number(x) +
                  ", " + QString::number(y) + ") 灰階值: " + QString::number(g);
    MousePosLabel->setText(str);
};
void MouseEvent::mousePressEvent (QMouseEvent *event) {
    QString str = "(" + QString::number (event->x()) +", "+
                  QString::number (event->y()) +")";
    if (event->button() == Qt::LeftButton)
    {
        statusBar()->showMessage (QStringLiteral ("左鍵:")+str);
    } else if (event->button()== Qt:: RightButton)
            statusBar()->showMessage (QStringLiteral("右鍵:")+str);
    else if (event->button()== Qt::MiddleButton)
            statusBar()->showMessage (QStringLiteral("中鍵:")+str);
};
void MouseEvent::mouseReleaseEvent (QMouseEvent *event) {
    QString str = "("+ QString::number (event->x()) +
    QString::number (event->y()) +")";
    statusBar()->showMessage (QStringLiteral("釋放:")+str);
};
