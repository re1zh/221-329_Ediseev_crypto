#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDateTime>
#include <QCryptographicHash>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), totalClicks(0) {
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    resetButton = new QPushButton("Сброс", this);
    loadButton = new QPushButton("Загрузить", this);

    gridLayout->addWidget(resetButton, 4, 0, 1, 2);
    gridLayout->addWidget(loadButton, 4, 2, 1, 2);

    connect(resetButton, &QPushButton::clicked, this, &MainWindow::handleReset);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::handleLoad);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            buttons[i][j] = new QPushButton("", this);
            buttons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(buttons[i][j], i, j);
            clickCount[i][j] = 0;
            connect(buttons[i][j], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::handleButtonClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            if (buttons[i][j] == button) break;
        }
        if (buttons[i][j] == button) break;
    }

    if (clickCount[i][j] == 0) {
        totalClicks++;
        clickCount[i][j] = totalClicks;
        if (totalClicks % 2 == 0) {
            button->setStyleSheet("background-color: green");
        } else {
            button->setStyleSheet("background-color: red");
        }
    }
}


