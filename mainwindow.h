#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void meldung(string meldungstext);
    void performTaskWithWait(QWidget* parent);
    struct TrigParams {
        double a; // Amplitude
        double b; // Frequenz
        double c; // Phasenverschiebung
        double d; // Vertikale Verschiebung
    };

private slots:
    void on_pushButton_38_clicked();
    void on_pushButton_5_clicked();
    void check_dig2();
    QVector<double> para_lesen_pumpe();
    void on_pushButton_3_clicked();
    bool check_koord_p();
    void on_pushButton_9_clicked();
    QVector<double> werte_ermitteln(QVector<double> x, QVector<double> y, int degree);
    void on_pushButton_clicked();
    QVector<double> xkoord_lesen_pumpe();
    QVector<double> ykoord_lesen_pumpe();
    qint32 algo_lesen_pumpe();
    void on_b_berechnen_2_clicked();
    void on_b_kurve_zeigen_3_clicked();
    double fx5(QVector<double> pset, double x);
    double calculateMSE(QVector<double> x_values,
                        QVector<double> y_values,
                        QVector<double> coefficients);
    void beispieldaten(QVector<double> x, QVector<double> y);
    void on_pushButton_4_clicked();
    double computeError(const QVector<double>& x, const QVector<double>& y, const TrigParams& params);
    void on_pushButton_6_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_2_clicked();
    void retranslateUi();

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;
    double x_neu(double xfaktor, double digx, double x_pix_min, double xmin);
    double y_neu(double yfaktor, double digy, double y_pix_min, double ymin);
    QTranslator translator;
};
#endif // MAINWINDOW_H
