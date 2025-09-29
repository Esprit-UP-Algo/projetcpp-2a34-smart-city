// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addLocal();

private:
    QWidget *createLocauxInterface();

    QLineEdit *nomEdit;
    QLineEdit *numEdit;
    QSpinBox  *aireSpin;
    QLineEdit *locEdit;
    QSpinBox  *chambresSpin;
    QComboBox *typeCombo;
    QComboBox *statutCombo;
    QSpinBox  *prixSpin;

    QTableWidget *table;
};

#endif // MAINWINDOW_H
