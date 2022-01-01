#include "mainwindow.h"
#include "imageview.h"
#include "QDebug"
#include <QtWidgets>
#include "QPainter"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *w = new QWidget();
    setCentralWidget(w);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    w->setLayout(mainLayout);

    QHBoxLayout *secondaryLayout = new QHBoxLayout();
    mainLayout->addLayout(secondaryLayout);

    QVBoxLayout *fileBtnsLayout = new QVBoxLayout();
    secondaryLayout->addLayout(fileBtnsLayout);

    QVBoxLayout *edgeLayout = new QVBoxLayout();
    secondaryLayout->addLayout(edgeLayout);

    QHBoxLayout *toleranceLayout = new QHBoxLayout();
    edgeLayout->addLayout(toleranceLayout);

    QSlider *colorTolerance = new QSlider(Qt::Orientation::Horizontal);
    colorTolerance->setRange(0, 100);
    QLabel *slideLabelVal = new QLabel("0");
    QLabel *slideLabel = new QLabel("Tolerance: ");
    QCheckBox *rollTolerance = new QCheckBox("Rolling Tolerance");
    QCheckBox *innerEdgeDetect = new QCheckBox("Inner Edge Detection");
    QCheckBox *fill = new QCheckBox("Fill");


    view = new ImageView(colorTolerance, rollTolerance, innerEdgeDetect, fill);

    QRect screen = QGuiApplication::screens()[0]->geometry();
    maxHeight = screen.height() * 0.8;
    maxWidth = screen.width() * 0.9;
    mainLayout->addWidget(view, 4);
    QPushButton *loadBtn = new QPushButton("Load");
    QPushButton *shapeM = new QPushButton("Shape Mode");
    QPushButton *clearBtn = new QPushButton("Clear");
    QPushButton *cartoon = new QPushButton("Cartoonize");
    saveBtn = new QPushButton("Save");
    saveBtn->setEnabled(false);

    toleranceLayout->addWidget(colorTolerance, 1);
    toleranceLayout->addWidget(slideLabel);
    toleranceLayout->addWidget(slideLabelVal);

    edgeLayout->addWidget(rollTolerance);
    edgeLayout->addWidget(innerEdgeDetect);
    edgeLayout->addWidget(fill);


    fileBtnsLayout->addWidget(loadBtn);
    fileBtnsLayout->addWidget(clearBtn);
    fileBtnsLayout->addWidget(saveBtn);

    secondaryLayout->addWidget(shapeM,1);
    secondaryLayout->addWidget(cartoon,1);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadBtnClk);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearBtnClk);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveBtnClk);
    connect(colorTolerance, &QSlider::valueChanged, this, [=] () {
            slideLabelVal->setText(QString::number(colorTolerance->value()));
    });
    connect(cartoon, &QPushButton::clicked, this, &MainWindow::cartoonBtnClk);


}

MainWindow::~MainWindow() {
}

void MainWindow::loadBtnClk() {
    fName = QFileDialog::getOpenFileName(this, "OpenFile");
    if (fName.isEmpty()) return;
    QImage imageTemp;
    if (!imageTemp.load(fName)) return;
    saveBtn->setEnabled(true);
    view->setImage(imageTemp);
    view->setFName(fName);
    if (imageTemp.width() < maxWidth & imageTemp.height() < maxHeight)
        view->setMinimumSize(imageTemp.width() * 1.01, imageTemp.height() * 1.01);       
    else
        view->setMinimumSize(maxWidth, maxHeight);
}

void MainWindow::clearBtnClk() {
    QImage imageTemp;
    if (!imageTemp.load(fName)) return;
    view->setImage(imageTemp);
}

void MainWindow::saveBtnClk() {
    view->saveEdit();
}
void MainWindow::cartoonBtnClk() {
    view->allShapes();
}
