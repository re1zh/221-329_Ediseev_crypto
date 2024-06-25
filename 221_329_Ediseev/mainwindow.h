#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonClick();
    void handleReset();
    void handleLoad();

private:
    Ui::MainWindow *ui;
    QGridLayout *gridLayout;
    QPushButton *buttons[4][4];
    QPushButton *resetButton;
    QPushButton *loadButton;
    int clickCount[4][4];
    int totalClicks;
    QString calculateHash(const QString &data);

    void resetGame();
    void loadGame();
    void saveGame();
    void checkHashes(const QJsonArray &moves);
};

#endif // MAINWINDOW_H
