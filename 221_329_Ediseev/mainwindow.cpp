#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDateTime>
#include <QCryptographicHash>

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

        QDateTime dateTime = QDateTime::currentDateTime();
        QString dateTimeString = dateTime.toString("yyyy.MM.dd_hh:mm:ss");

        QString previousHash = getLastMoveHash();

        QString data = QString::number(i) + QString::number(j) + dateTimeString + previousHash;
        QString currentHash = calculateHash(data);

        QJsonObject move;
        move["i"] = i;
        move["j"] = j;
        move["datetime"] = dateTimeString;
        move["hash"] = currentHash;

        QFile file("moves.json");
        if (file.open(QIODevice::ReadWrite)) {
            QByteArray fileData = file.readAll();
            QJsonDocument doc;
            QJsonArray moves;
            if (!fileData.isEmpty()) {
                doc = QJsonDocument::fromJson(fileData);
                moves = doc.array();
            }
            moves.append(move);
            doc.setArray(moves);
            file.resize(0);
            file.write(doc.toJson());
            file.close();
        }
    }
}

QString MainWindow::calculateHash(const QString &data) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(data.toUtf8());
    return hash.result().toHex();
}

void MainWindow::resetGame() {
    totalClicks = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            buttons[i][j]->setText("");
            buttons[i][j]->setStyleSheet("");
            clickCount[i][j] = 0;
        }
    }
}

void MainWindow::handleReset() {
    resetGame();
}

void MainWindow::handleLoad() {
    loadGame();
}

void MainWindow::loadGame() {
    QFile file("moves.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    QJsonArray moves = doc.array();

    if (!checkHashes(moves)) {
        QMessageBox::warning(this, "Ошибка", "Файл данных поврежден.");
        return;
    }

    resetGame();

    for (int k = 0; k < moves.size(); ++k) {
        QJsonObject move = moves[k].toObject();
        int i = move["i"].toInt();
        int j = move["j"].toInt();
        clickCount[i][j] = k + 1;
        if (clickCount[i][j] % 2 == 0) {
            buttons[i][j]->setStyleSheet("background-color: green");
        } else {
            buttons[i][j]->setStyleSheet("background-color: red");
        }
    }
}

bool MainWindow::checkHashes(const QJsonArray &moves) {
    QString previousHash = "";
    for (int k = 0; k < moves.size(); ++k) {
        QJsonObject move = moves[k].toObject();
        QString data = QString::number(move["i"].toInt()) +
                       QString::number(move["j"].toInt()) +
                       move["datetime"].toString() +
                       previousHash;

        QString calculatedHash = calculateHash(data);
        if (calculatedHash != move["hash"].toString()) {
            QMessageBox::warning(this, "Ошибка", QString("Хеш-сумма не соответствует в ходе %1").arg(k + 1));
            return false;
        }

        previousHash = move["hash"].toString();
    }

    return true;
}


QString MainWindow::getLastMoveHash() {
    QFile file("moves.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();
        if (!fileData.isEmpty()) {
            QJsonDocument doc = QJsonDocument::fromJson(fileData);
            QJsonArray moves = doc.array();
            if (!moves.isEmpty()) {
                QJsonObject lastMove = moves.last().toObject();
                return lastMove["hash"].toString();
            }
        }
    }
    return "";
}
