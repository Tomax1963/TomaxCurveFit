// CurveFit: A program to digitize coordinates and fit them to mathematical funktion curves.
// Copyright (C) 2025  Thomas Singler
// EMail: tomax63@web.de
// PayPal: tomax63@web.de

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "QuantileRegression.h"
#include <cmath>
#include <QtMath>
#include <QMovie>
#include <QTimer>
#include <QTranslator>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_38_clicked(){
    QVector<QVector<double>> sortarray;
    sortarray.append({ui->xp1_3->value(), ui->yp1_3->value()});
    sortarray.append({ui->xp2_3->value(), ui->yp2_3->value()});
    sortarray.append({ui->xp3_3->value(), ui->yp3_3->value()});
    sortarray.append({ui->xp4_3->value(), ui->yp4_3->value()});
    sortarray.append({ui->xp5_3->value(), ui->yp5_3->value()});
    sortarray.append({ui->xp6_3->value(), ui->yp6_3->value()});
    sortarray.append({ui->xp7_3->value(), ui->yp7_3->value()});
    std::sort(sortarray.begin(),sortarray.end() ,[](const QVector<double>& left, const QVector<double>& right)->bool{
        if(left.empty() && right.empty()) return false;
        if(left.empty()) return true;
        if(right.empty()) return false;
        return left.first()<right.first();
    }
              );
    ui->xp1_3->setValue(sortarray[0][0]);
    ui->yp1_3->setValue(sortarray[0][1]);
    ui->xp2_3->setValue(sortarray[1][0]);
    ui->yp2_3->setValue(sortarray[1][1]);
    ui->xp3_3->setValue(sortarray[2][0]);
    ui->yp3_3->setValue(sortarray[2][1]);
    ui->xp4_3->setValue(sortarray[3][0]);
    ui->yp4_3->setValue(sortarray[3][1]);
    ui->xp5_3->setValue(sortarray[4][0]);
    ui->yp5_3->setValue(sortarray[4][1]);
    ui->xp6_3->setValue(sortarray[5][0]);
    ui->yp6_3->setValue(sortarray[5][1]);
    ui->xp7_3->setValue(sortarray[6][0]);
    ui->yp7_3->setValue(sortarray[6][1]);
}

void MainWindow::check_dig2(){
    bool pruefung = true;
    if (ui->i_dig2_0xx->text().toDouble() < 1){
        pruefung = false;
    }
    if (ui->i_dig2_0yx->text().toDouble() < 1){
        pruefung = false;
    }
    if (ui->i_dig2_100xx->text().toDouble() < 1){
        pruefung = false;
    }
    if (ui->i_dig2_100yx->text().toDouble() < 1){
        pruefung = false;
    }
    if (ui->i_dig2_xmax->value() < 1){
        pruefung = false;
    }
    if (ui->i_dig2_ymax->value() < 1){
        pruefung = false;
    }
    if (pruefung){
        ui->rb_dig2_ko1->setEnabled(true);
        ui->rb_dig2_ko2->setEnabled(true);
        ui->rb_dig2_ko3->setEnabled(true);
        ui->rb_dig2_ko4->setEnabled(true);
        ui->rb_dig2_ko5->setEnabled(true);
        ui->rb_dig2_ko6->setEnabled(true);
        ui->rb_dig2_ko7->setEnabled(true);
    }
}

void MainWindow::meldung(string meldungstext = " "){
    QMessageBox msgBox;
    QString mtext = QString::fromStdString(meldungstext);
    msgBox.setText(mtext);
    msgBox.exec();
}

void MainWindow::on_pushButton_5_clicked(){
    double xmin = ui->i_dig2_xmin->value();
    double xmax = ui->i_dig2_xmax->value();
    double ymin = ui->i_dig2_ymin->value();
    double ymax = ui->i_dig2_ymax->value();
    double xx0 = ui->i_dig2_0xx->text().toDouble();
    double xx100 = ui->i_dig2_100xx->text().toDouble();
    double yy0 = ui->i_dig2_0yy->text().toDouble();
    double yy100 = ui->i_dig2_100yy->text().toDouble();
    if (xmin < xmax && ymin < ymax){
        QPoint position = ui->pushButton_5->mapFromGlobal(QCursor::pos());
        double digx, digy;
        double xfaktor, yfaktor, xneu, yneu;
        digx = double(position.x());
        double hoehenfaktor = 1.0;
        digy = double(ui->pushButton_5->height() - position.y());
        if (ui->cb_logx->isChecked()){
            if (!(ui->i_dig2_xmin->value() > 0)){
                meldung("Minimalwert X muss bei logarithmischer Digitalisierung > 0 sein.");
                return;
            }
        }
        if (ui->cb_logy->isChecked()){
            if (!(ui->i_dig2_ymin->value() > 0)){
                meldung("Minimalwert Y muss bei logarithmischer Digitalisierung > 0 sein.");
                return;
            }
        }
        if (!ui->rb_dig2_ko1->isEnabled()){
            if (ui->rb_dig2_0x->isChecked()){
                ui->i_dig2_0xx->setText(QString::number(digx));
                ui->i_dig2_0xy->setText(QString::number(digy));
                ui->i_dig2_0yx->setText(QString::number(digx));
                ui->i_dig2_0yy->setText(QString::number(digy));
                check_dig2();
                ui->rb_dig2_100x->setChecked(true);
                ui->ck_xmin->setChecked(true);
                ui->ck_ymin->setChecked(true);
                return;
            }
            if (ui->rb_dig2_100x->isChecked()){
                ui->i_dig2_100xx->setText(QString::number(digx));
                ui->i_dig2_100xy->setText(QString::number(digy));
                check_dig2();
                ui->rb_dig2_100y->setChecked(true);
                ui->ck_xmax->setChecked(true);
                return;
            }
            if (ui->rb_dig2_0y->isChecked()){
                ui->i_dig2_0yx->setText(QString::number(digx));
                ui->i_dig2_0yy->setText(QString::number(digy));
                check_dig2();
                return;
            }
            if (ui->rb_dig2_100y->isChecked()){
                ui->i_dig2_100yx->setText(QString::number(digx));
                ui->i_dig2_100yy->setText(QString::number(digy));
                check_dig2();
                ui->rb_dig2_ko1->setChecked(true);
                ui->ck_ymax->setChecked(true);
                return;
            }
        }
        if (ui->cb_logx->isChecked()){
            xfaktor = (log10(xmax) - log10(xmin))
            / (xx100 - xx0);
        }else{
            xfaktor = (xmax - xmin)
            / (xx100 - xx0);
        }
        if (ui->cb_logy->isChecked()){
            yfaktor = (log10(ymax) - log10(ymin))
            / (yy100 - yy0);
        }else{
            yfaktor = (ymax - ymin)
            / (yy100 - yy0);
        }
        if (ui->rb_dig2_ko1->isChecked() && ui->rb_dig2_ko1->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko1q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko1h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko2->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko2->isChecked() && ui->rb_dig2_ko2->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko2q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko2h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko3->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko3->isChecked() && ui->rb_dig2_ko3->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko3q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko3h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko4->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko4->isChecked() && ui->rb_dig2_ko4->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko4q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko4h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko5->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko5->isChecked() && ui->rb_dig2_ko5->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko5q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko5h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko6->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko6->isChecked() && ui->rb_dig2_ko6->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko6q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko6h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            ui->rb_dig2_ko7->setChecked(true);
            return;
        }
        if (ui->rb_dig2_ko7->isChecked() && ui->rb_dig2_ko7->isEnabled()){
            xneu = x_neu(xfaktor, digx, xx0, xmin);
            yneu = y_neu(yfaktor, digy, yy0, ymin);
            ui->i_dig2_ko7q->setText(QString::number(xneu, 'f', 2));
            ui->i_dig2_ko7h->setText(QString::number(yneu / hoehenfaktor, 'f', 2));
            return;
        }
    }else{
        meldung(tr("Vor der Kalibrierung müssen die Minima und Maxima angegeben werden").toStdString());
    }
}

double MainWindow::x_neu(double xfaktor, double digx, double x_pix_min, double xmin){
    double xneu;
    if (ui->cb_logx->isChecked()){
        xneu = pow(10, xfaktor * (digx - x_pix_min));
    }else{
        xneu = xfaktor * (digx - x_pix_min) + xmin;
    }
    return xneu;
}

double MainWindow::y_neu(double yfaktor, double digy, double y_pix_min, double ymin){
    double yneu;
    if (ui->cb_logy->isChecked()){
        yneu = pow(10, yfaktor * (digy - y_pix_min));
    }else{
        yneu = yfaktor * (digy - y_pix_min) + ymin;
    }
    return yneu;
}

void MainWindow::on_pushButton_3_clicked(){
    const QClipboard *cb = QApplication::clipboard();
    QImage img(cb->image());
    if (img.isNull()){
        meldung(tr("Kein Bild in der Zwischenablage!").toStdString());
    }else{
        double breitenfaktor = double(img.width()) / double(ui->pushButton_5->width());
        double hoehenfaktor = double(img.height()) / double(ui->pushButton_5->height());
        double zoomfaktor = max(breitenfaktor, hoehenfaktor);
        QSize zoom;
        if (ui->cb_propo->isChecked()){
            zoom = QSize(img.width() / zoomfaktor, img.height() / zoomfaktor);
        }else{
            zoom = QSize(img.width() / breitenfaktor, img.height() / hoehenfaktor);
        }
        QIcon ButtonIcon(QPixmap::fromImage(img).scaled(zoom));
        ui->pushButton_5->setIcon(ButtonIcon);
        ui->rb_dig2_0x->setChecked(true);
        ui->rb_dig2_ko1->setEnabled(false);
        ui->rb_dig2_ko2->setEnabled(false);
        ui->rb_dig2_ko3->setEnabled(false);
        ui->rb_dig2_ko4->setEnabled(false);
        ui->rb_dig2_ko5->setEnabled(false);
        ui->rb_dig2_ko6->setEnabled(false);
        ui->rb_dig2_ko7->setEnabled(false);
        ui->i_dig2_0xx->setText("0");
        ui->i_dig2_100xx->setText("0");
        ui->i_dig2_0xy->setText("0");
        ui->i_dig2_100xy->setText("0");
        ui->i_dig2_0yx->setText("0");
        ui->i_dig2_100yx->setText("0");
        ui->i_dig2_0yy->setText("0");
        ui->i_dig2_100yy->setText("0");
    }
}


void MainWindow::on_pushButton_9_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Bildquelle wählen"), QDir::currentPath(), "Images (*.jpg *.png)");
    if (fileName.isEmpty()) {
        meldung(tr("Keine Datei ausgewählt!").toStdString());
        return;
    }

    QPixmap pixmap(fileName);
    if (pixmap.isNull()) {
        meldung(tr("Bild konnte nicht geladen werden!").toStdString());
        return;
    }

    double breitenfaktor = double(pixmap.width()) / double(ui->pushButton_5->width());
    double hoehenfaktor = double(pixmap.height()) / double(ui->pushButton_5->height());
    double zoomfaktor = std::max(breitenfaktor, hoehenfaktor);

    QSize zoom;
    if (ui->cb_propo->isChecked()) {
        zoom = QSize(pixmap.width() / zoomfaktor, pixmap.height() / zoomfaktor);
    } else {
        zoom = QSize(ui->pushButton_5->width(), ui->pushButton_5->height());
    }

    QPixmap scaledPixmap = pixmap.scaled(zoom, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon ButtonIcon(scaledPixmap);
    ui->pushButton_5->setIcon(ButtonIcon);
    ui->pushButton_5->setIconSize(zoom);

    ui->rb_dig2_0x->setChecked(true);
    ui->rb_dig2_ko1->setEnabled(false);
    ui->rb_dig2_ko2->setEnabled(false);
    ui->rb_dig2_ko3->setEnabled(false);
    ui->rb_dig2_ko4->setEnabled(false);
    ui->rb_dig2_ko5->setEnabled(false);
    ui->rb_dig2_ko6->setEnabled(false);
    ui->rb_dig2_ko7->setEnabled(false);
    ui->i_dig2_0xx->setText("0");
    ui->i_dig2_100xx->setText("0");
    ui->i_dig2_0xy->setText("0");
    ui->i_dig2_100xy->setText("0");
    ui->i_dig2_0yx->setText("0");
    ui->i_dig2_100yx->setText("0");
    ui->i_dig2_0yy->setText("0");
    ui->i_dig2_100yy->setText("0");
}


void MainWindow::on_pushButton_clicked(){
    ui->xp1_3->setValue(ui->i_dig2_ko1q->text().toDouble());
    ui->xp2_3->setValue(ui->i_dig2_ko2q->text().toDouble());
    ui->xp3_3->setValue(ui->i_dig2_ko3q->text().toDouble());
    ui->xp4_3->setValue(ui->i_dig2_ko4q->text().toDouble());
    ui->xp5_3->setValue(ui->i_dig2_ko5q->text().toDouble());
    ui->xp6_3->setValue(ui->i_dig2_ko6q->text().toDouble());
    ui->xp7_3->setValue(ui->i_dig2_ko7q->text().toDouble());
    ui->yp1_3->setValue(ui->i_dig2_ko1h->text().toDouble());
    ui->yp2_3->setValue(ui->i_dig2_ko2h->text().toDouble());
    ui->yp3_3->setValue(ui->i_dig2_ko3h->text().toDouble());
    ui->yp4_3->setValue(ui->i_dig2_ko4h->text().toDouble());
    ui->yp5_3->setValue(ui->i_dig2_ko5h->text().toDouble());
    ui->yp6_3->setValue(ui->i_dig2_ko6h->text().toDouble());
    ui->yp7_3->setValue(ui->i_dig2_ko7h->text().toDouble());
}

void MainWindow::on_b_berechnen_2_clicked(){
    // Fortschrittsdialog erstellen
    QProgressDialog progressDialog("Berechnungen werden durchgeführt...", "Abbrechen", 0, 0, this);
    progressDialog.setWindowModality(Qt::ApplicationModal);
    progressDialog.setCancelButton(nullptr); // Kein Abbrechen möglich

    // Ladeanzeige
    QLabel* loadingLabel = new QLabel(&progressDialog);
    // QMovie* loadingMovie = new QMovie(":/loading.gif"); // GIF aus Ressourcen
    // QSize moviesize(50,50);
    // loadingMovie->setScaledSize(moviesize);
    // loadingLabel->setMovie(loadingMovie);
    // loadingMovie->start();

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(loadingLabel);
    progressDialog.setLayout(layout);

    // Dialog anzeigen
    progressDialog.show();
    QApplication::processEvents(); // UI-Update sicherstellen

    int stellen = 12;
    if (check_koord_p()){
        QVector<double> xp = xkoord_lesen_pumpe();
        QVector<double> yp = ykoord_lesen_pumpe();
        qint32 algop = algo_lesen_pumpe();
        if (ui->rb_poly3g->isChecked() || ui->rb_poly4g->isChecked() || ui->rb_poly5g->isChecked()){ // Polynomregression 3., 4. und 5. Grades
            QVector<double> kurvenparameter = werte_ermitteln(xp, yp, algop);
            ui->ap_3->setText(QString::number(kurvenparameter[5], 'e', stellen));
            ui->bp_3->setText(QString::number(kurvenparameter[4], 'e', stellen));
            ui->cp_3->setText(QString::number(kurvenparameter[3], 'e', stellen));
            ui->dp_3->setText(QString::number(kurvenparameter[2], 'e', stellen));
            ui->ep_3->setText(QString::number(kurvenparameter[1], 'e', stellen));
            ui->fp_3->setText(QString::number(kurvenparameter[0], 'e', stellen));
        }else if(ui->rb_linear->isChecked()){ // lineare Regression
            double slope = 0.0, intercept = 0.0;
            int n = xp.size();
            double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
            // Berechnungen für die Summen
            for (int i = 0; i < n; ++i) {
                sumX += xp[i];
                sumY += yp[i];
                sumXY += xp[i] * yp[i];
                sumX2 += xp[i] * xp[i];
            }
            // Berechnung von Steigung (m) und Achsenabschnitt (b)
            slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
            intercept = (sumY - slope * sumX) / n;
            ui->fp_3->setText(QString::number(intercept, 'e', stellen));
            ui->ep_3->setText(QString::number(slope, 'e', stellen));
            ui->dp_3->setText(QString::number(0, 'e', stellen));
            ui->cp_3->setText(QString::number(0, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }else if(ui->rb_logarithmisch->isChecked()){ // logarithmische Regression
            double a = 0.0, b = 0.0;
            int n = xp.size();
            double sumLogX = 0, sumY = 0, sumLogX2 = 0, sumLogXY = 0;

            // Berechnung der Summen für log(x), log(x)^2 und y*log(x)
            for (int i = 0; i < n; ++i) {
                if (xp[i] <= 0) {
                    meldung("Fehler: x-Werte müssen größer als 0 sein, da log(x) definiert sein muss.");
                    return;
                }
                double logX = std::log(xp[i]);
                sumLogX += logX;
                sumY += yp[i];
                sumLogX2 += logX * logX;
                sumLogXY += logX * yp[i];
            }

            // Berechnung der Koeffizienten a und b
            a = (n * sumLogXY - sumLogX * sumY) / (n * sumLogX2 - sumLogX * sumLogX);
            b = (sumY - a * sumLogX) / n;
            ui->fp_3->setText(QString::number(b, 'e', stellen));
            ui->ep_3->setText(QString::number(a, 'e', stellen));
            ui->dp_3->setText(QString::number(0, 'e', stellen));
            ui->cp_3->setText(QString::number(0, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }else if (ui->rb_exponentiell->isChecked()){ // exponentielle Regression
            int n = xp.size();
            if (n != yp.size() || n < 2) {
                throw std::invalid_argument("x and y must have the same size and contain at least two elements.");
            }

            // Transform y -> ln(y) for linearization
            QVector<double> lnY(n);
            for (int i = 0; i < n; ++i) {
                if (yp[i] <= 0) {
                    throw std::invalid_argument("y values must be positive for logarithm transformation.");
                }
                lnY[i] = qLn(yp[i]);
            }

            // Calculate averages
            double sumX = 0.0, sumLnY = 0.0, sumX2 = 0.0, sumXLnY = 0.0;
            for (int i = 0; i < n; ++i) {
                sumX += xp[i];
                sumLnY += lnY[i];
                sumX2 += xp[i] * xp[i];
                sumXLnY += xp[i] * lnY[i];
            }

            double avgX = sumX / n;
            double avgLnY = sumLnY / n;

            // Calculate slope (b) and intercept (ln(a))
            double b = (sumXLnY - n * avgX * avgLnY) / (sumX2 - n * avgX * avgX);
            double lnA = avgLnY - b * avgX;

            // Convert ln(a) back to a
            double a = qExp(lnA);
            ui->fp_3->setText(QString::number(b, 'e', stellen));
            ui->ep_3->setText(QString::number(a, 'e', stellen));
            ui->dp_3->setText(QString::number(0, 'e', stellen));
            ui->cp_3->setText(QString::number(0, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }else if (ui->rb_logistisch->isChecked()){ // logistische Regression
            double L = *std::max_element(yp.begin(), yp.end());  // L ist die maximale y-Wert
            double x0 = xp[xp.size() / 2];                      // Mitte von x als Startwert für x0
            double k = 1.0;                                   // Startwert für k

            // Gradient-Descent-ähnliche Optimierung (einfacher Ansatz)
            double learningRate = 0.01;
            int maxIterations = 1000;

            for (int iter = 0; iter < maxIterations; ++iter) {
                double dL = 0, dk = 0, dx0 = 0;

                // Gradienten berechnen
                for (int i = 0; i < xp.size(); ++i) {
                    double y_pred = L / (1 + qExp(-k * (xp[i] - x0)));
                    double error = yp[i] - y_pred;

                    // Ableitungen
                    dL += -2 * error / (1 + qExp(-k * (xp[i] - x0)));
                    dk += -2 * error * L * (xp[i] - x0) * qExp(-k * (xp[i] - x0)) / qPow(1 + qExp(-k * (xp[i] - x0)), 2);
                    dx0 += 2 * error * L * k * qExp(-k * (xp[i] - x0)) / qPow(1 + qExp(-k * (xp[i] - x0)), 2);
                }

                // Parameter aktualisieren
                L -= learningRate * dL;
                k -= learningRate * dk;
                x0 -= learningRate * dx0;
            }
            ui->fp_3->setText(QString::number(L, 'e', stellen));
            ui->ep_3->setText(QString::number(k, 'e', stellen));
            ui->dp_3->setText(QString::number(x0, 'e', stellen));
            ui->cp_3->setText(QString::number(0, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }else if(ui->rb_trigonometrisch->isChecked()){ // trigonometrische Regression
            TrigParams bestParams = {1.0, 1.0, 0.0, 0.0};
            double bestError = computeError(xp, yp, bestParams);
            // Grobes Raster für Parameteroptimierung
            for (int ai = 1; ai <= 100; ++ai) {       // ai acts as the counter for 'a'
                double a = 0.1 * ai;                  // Compute 'a' as a floating-point value
                for (int bi = 1; bi <= 100; ++bi) {   // bi acts as the counter for 'b'
                    double b = 0.1 * bi;              // Compute 'b' as a floating-point value
                    for (int ci = 0; ci <= 63; ++ci) { // ci acts as the counter for 'c'
                        double c = 0.1 * ci;          // Compute 'c' as a floating-point value
                        for (int di = -100; di <= 100; ++di) { // di acts as the counter for 'd'
                            double d = 0.1 * di;      // Compute 'd' as a floating-point value

                            TrigParams currentParams = {a, b, c, d};
                            double error = computeError(xp, yp, currentParams);
                            if (error < bestError) {
                                bestError = error;
                                bestParams = currentParams;
                            }
                        }
                    }
                }
            }
            ui->fp_3->setText(QString::number(bestParams.a, 'e', stellen));
            ui->ep_3->setText(QString::number(bestParams.b, 'e', stellen));
            ui->dp_3->setText(QString::number(bestParams.c, 'e', stellen));
            ui->cp_3->setText(QString::number(bestParams.d, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }else if(ui->rb_quantil->isChecked()){ // quantile Regression
            double quantile = 0.5; // Median-Quantil
            QVector<double> coefficients;

            QuantileRegression regression(quantile);
            regression.fit(xp, yp, coefficients);
            ui->fp_3->setText(QString::number(coefficients[0], 'e', stellen));
            ui->ep_3->setText(QString::number(coefficients[1], 'e', stellen));
            ui->dp_3->setText(QString::number(0, 'e', stellen));
            ui->cp_3->setText(QString::number(0, 'e', stellen));
            ui->bp_3->setText(QString::number(0, 'e', stellen));
            ui->ap_3->setText(QString::number(0, 'e', stellen));
        }
    }
    // Fortschrittsdialog schließen
    progressDialog.close();
}

double MainWindow::computeError(const QVector<double>& x, const QVector<double>& y, const TrigParams& params) {
    double error = 0.0;
    for (int i = 0; i < x.size(); ++i) {
        double predicted = params.a * qSin(params.b * x[i] + params.c) + params.d;
        error += qPow(predicted - y[i], 2);
    }
    return error;
}

QVector<double> MainWindow::xkoord_lesen_pumpe(){
    QVector<double> koordx_pumpe;
    koordx_pumpe.append(ui->xp1_3->value());
    koordx_pumpe.append(ui->xp2_3->value());
    koordx_pumpe.append(ui->xp3_3->value());
    koordx_pumpe.append(ui->xp4_3->value());
    koordx_pumpe.append(ui->xp5_3->value());
    koordx_pumpe.append(ui->xp6_3->value());
    koordx_pumpe.append(ui->xp7_3->value());
    return(koordx_pumpe);
}

QVector<double> MainWindow::ykoord_lesen_pumpe(){
    QVector<double> koordy_pumpe;
    koordy_pumpe.append(ui->yp1_3->value());
    koordy_pumpe.append(ui->yp2_3->value());
    koordy_pumpe.append(ui->yp3_3->value());
    koordy_pumpe.append(ui->yp4_3->value());
    koordy_pumpe.append(ui->yp5_3->value());
    koordy_pumpe.append(ui->yp6_3->value());
    koordy_pumpe.append(ui->yp7_3->value());
    return(koordy_pumpe);
}

qint32 MainWindow::algo_lesen_pumpe(){
    qint32 algo_p = 0;
    if (ui->rb_poly3g->isChecked()) {
        algo_p = 3;
    }
    if (ui->rb_poly4g->isChecked()) {
        algo_p = 4;
    }
    if (ui->rb_poly5g->isChecked()) {
        algo_p = 5;
    }
    return algo_p;
}

// QVector<double> MainWindow::werte_ermitteln(QVector<double> x, QVector<double> y, int degree){
//     int i,j,k,N,n;
//     N = x.size();
//     n = degree;
//     long double X[2*n+1];
//     long double B[n+1][n+2];
//     double a[n+1];
//     for (i=0;i<2*n+1;i++){
//         X[i]=0;
//         for (j=0;j<N;j++)
//             X[i]=X[i]+pow(x[j],i);
//     }
//     for (i=0;i<=n;i++)
//         for (j=0;j<=n;j++)
//             B[i][j]=X[i+j];
//     long double Y[n+1];
//     for (i=0;i<n+1;i++){
//         Y[i]=0;
//         for (j=0;j<N;j++)
//             Y[i]=Y[i]+pow(x[j],i)*y[j];
//     }
//     for (i=0;i<=n;i++)
//         B[i][n+1]=Y[i];
//     n=n+1;

//     for (i=0;i<n;i++)
//         for (k=i+1;k<n;k++)
//             if (B[i][i]<B[k][i])
//                 for (j=0;j<=n;j++){
//                     long double temp=B[i][j];
//                     B[i][j]=B[k][j];
//                     B[k][j]=temp;
//                 }

//     for (i=0;i<n-1;i++)
//         for (k=i+1;k<n;k++){
//             long double t=B[k][i]/B[i][i];
//             for (j=0;j<=n;j++)
//                 B[k][j]=B[k][j]-t*B[i][j];
//         }
//     for (i=n-1;i>=0;i--){
//         a[i]=B[i][n];
//         for (j=0;j<n;j++)
//             if (j!=i)
//                 a[i]=a[i]-B[i][j]*a[j];
//         a[i]=a[i]/B[i][i];
//     }
//     QVector<double> transwerte(6);
//     copy(a, a+n, transwerte.begin());
//     return transwerte;
// }

QVector<double> MainWindow::werte_ermitteln(QVector<double> x, QVector<double> y, int degree) {
    int i, j, k, N, n;
    N = x.size();
    n = degree;

    std::vector<long double> X(2 * n + 1, 0.0);
    std::vector<std::vector<long double>> B(n + 1, std::vector<long double>(n + 2, 0.0));
    std::vector<double> a(n + 1, 0.0);

    // Calculate sums for X
    for (i = 0; i < 2 * n + 1; i++) {
        for (j = 0; j < N; j++) {
            X[i] += pow(x[j], i);
        }
    }

    // Fill matrix B with coefficients
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++) {
            B[i][j] = X[i + j];
        }
    }

    // Calculate Y values
    std::vector<long double> Y(n + 1, 0.0);
    for (i = 0; i < n + 1; i++) {
        for (j = 0; j < N; j++) {
            Y[i] += pow(x[j], i) * y[j];
        }
    }
    for (i = 0; i <= n; i++) {
        B[i][n + 1] = Y[i];
    }

    // Gaussian elimination
    n = n + 1; // Adjust n for the augmented matrix
    for (i = 0; i < n; i++) {
        for (k = i + 1; k < n; k++) {
            if (B[i][i] < B[k][i]) {
                for (j = 0; j <= n; j++) {
                    std::swap(B[i][j], B[k][j]);
                }
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (k = i + 1; k < n; k++) {
            long double t = B[k][i] / B[i][i];
            for (j = 0; j <= n; j++) {
                B[k][j] -= t * B[i][j];
            }
        }
    }

    // Back substitution
    for (i = n - 1; i >= 0; i--) {
        a[i] = B[i][n];
        for (j = 0; j < n; j++) {
            if (j != i) {
                a[i] -= B[i][j] * a[j];
            }
        }
        a[i] /= B[i][i];
    }

    // Copy result to QVector
    QVector<double> transwerte(n);
    std::copy(a.begin(), a.end(), transwerte.begin());

    return transwerte;
}

bool MainWindow::check_koord_p(){
    bool alles_ok = true;
    if (ui->xp1_3->text() == ""){alles_ok = false;}
    if (ui->xp2_3->text() == ""){alles_ok = false;}
    if (ui->xp3_3->text() == ""){alles_ok = false;}
    if (ui->xp4_3->text() == ""){alles_ok = false;}
    if (ui->xp5_3->text() == ""){alles_ok = false;}
    if (ui->xp6_3->text() == ""){alles_ok = false;}
    if (ui->xp7_3->text() == ""){alles_ok = false;}
    if (ui->yp1_3->text() == ""){alles_ok = false;}
    if (ui->yp2_3->text() == ""){alles_ok = false;}
    if (ui->yp3_3->text() == ""){alles_ok = false;}
    if (ui->yp4_3->text() == ""){alles_ok = false;}
    if (ui->yp5_3->text() == ""){alles_ok = false;}
    if (ui->yp6_3->text() == ""){alles_ok = false;}
    if (ui->yp7_3->text() == ""){alles_ok = false;}
    if (!alles_ok){
        meldung("Pumpenkoordinaten nicht erfasst!");
    }
    return(alles_ok);
}

void MainWindow::on_b_kurve_zeigen_3_clicked(){
    QVector<double> psetp = para_lesen_pumpe();
    // create graph and assign data to it:
    ui->dia_darstellung->clearGraphs();
    ui->dia_darstellung->clearItems();
    ui->dia_darstellung->legend->clear();
    ui->dia_darstellung->addGraph();
    QCPAxis *xachse = ui->dia_darstellung->xAxis;
    QCPAxis *yachse = ui->dia_darstellung->yAxis;
    QVector<double> x_werte = {}, y_werte = {}, vorgabewerte_x = {}, vorgabewerte_y = {}, verlaufwerte_x = {}, verlaufwerte_y = {};
    qint32 werte_anzahl = 50, verlauf_anzahl = 200;
    double verlauf_beginn, verlauf_ende;
    double mitte_xwerte = (ui->xp1_3->value() + ui->xp7_3->value()) / 2;
    double bereich_erweiterung = (ui->xp7_3->value() - ui->xp1_3->value()) * (ui->x_breitenfaktor->value() / 2);
    verlauf_beginn = mitte_xwerte - bereich_erweiterung;
    verlauf_ende = mitte_xwerte + bereich_erweiterung;
    double step = (ui->xp7_3->value() - ui->xp1_3->value()) / (werte_anzahl - 1); // Schrittweite
    double step_v = (verlauf_ende - verlauf_beginn) / (verlauf_anzahl - 1);
    vorgabewerte_x = {ui->xp1_3->value(),
                      ui->xp2_3->value(),
                      ui->xp3_3->value(),
                      ui->xp4_3->value(),
                      ui->xp5_3->value(),
                      ui->xp6_3->value(),
                      ui->xp7_3->value()};
    vorgabewerte_y = {ui->yp1_3->value(),
                      ui->yp2_3->value(),
                      ui->yp3_3->value(),
                      ui->yp4_3->value(),
                      ui->yp5_3->value(),
                      ui->yp6_3->value(),
                      ui->yp7_3->value()};
    for (int i = 0; i < werte_anzahl; ++i) {
        x_werte.append(ui->xp1_3->value() + (i * step));
        if (ui->rb_logarithmisch->isChecked()){
            y_werte.append(ui->ep_3->text().toDouble() * log(x_werte[i]) + ui->fp_3->text().toDouble()); // Logarithmische Regression
        }else if (ui->rb_linear->isChecked()){
            y_werte.append((ui->ep_3->text().toDouble() * x_werte[i]) + ui->fp_3->text().toDouble()); // Lineare Regression
        }else if (ui->rb_exponentiell->isChecked()){
            y_werte.append(ui->ep_3->text().toDouble() * qExp(ui->fp_3->text().toDouble() * x_werte[i])); // Exponentielle Regression
        }else if (ui->rb_logistisch->isChecked()){
            y_werte.append(ui->fp_3->text().toDouble() / (1 + qExp(-ui->ep_3->text().toDouble() * (x_werte[i] - ui->dp_3->text().toDouble())))); // Logistische Regression
        }else if (ui->rb_trigonometrisch->isChecked()){
            y_werte.append(ui->fp_3->text().toDouble() * qSin(ui->ep_3->text().toDouble() * x_werte[i] + ui->dp_3->text().toDouble()) + ui->cp_3->text().toDouble()); // Trigonometrische Regression
        }else{
            y_werte.append(fx5(psetp, x_werte[i]));
        }
    }
    for (int j = 0; j < verlauf_anzahl; ++j) {
        verlaufwerte_x.append(verlauf_beginn + (j * step_v));
        if (ui->rb_logarithmisch->isChecked()){
            verlaufwerte_y.append(ui->ep_3->text().toDouble() * log(verlaufwerte_x[j]) + ui->fp_3->text().toDouble()); // Logarithmische Regression
            ui->funktion->setText("y = " + QString::number(ui->ep_3->text().toDouble(),'f') + " * e ^(" + QString::number(ui->fp_3->text().toDouble(),'f') + "*x)");
        }else if (ui->rb_linear->isChecked()){
            verlaufwerte_y.append((ui->ep_3->text().toDouble() * verlaufwerte_x[j]) + ui->fp_3->text().toDouble()); // Lineare Regression
            ui->funktion->setText("y = " + QString::number(ui->ep_3->text().toDouble(),'f') + " * x + " + QString::number(ui->fp_3->text().toDouble(),'f'));
        }else if (ui->rb_exponentiell->isChecked()){
            verlaufwerte_y.append(ui->ep_3->text().toDouble() * qExp(ui->fp_3->text().toDouble() * verlaufwerte_x[j])); // Exponentielle Regression
            ui->funktion->setText("y = " + QString::number(ui->ep_3->text().toDouble(),'f') + " + " + QString::number(ui->fp_3->text().toDouble(),'f') + " * ln(x)");
        }else if (ui->rb_logistisch->isChecked()){
            verlaufwerte_y.append(ui->fp_3->text().toDouble() / (1 + qExp(-ui->ep_3->text().toDouble() * (verlaufwerte_x[j] - ui->dp_3->text().toDouble())))); // Logistische Regression
            ui->funktion->setText("y = " + QString::number(ui->fp_3->text().toDouble(),'f') + " / (1 + e^(-" + QString::number(ui->ep_3->text().toDouble(),'f') + " * (x-" + QString::number(ui->dp_3->text().toDouble(),'f') + ")))");
        }else if (ui->rb_trigonometrisch->isChecked()){
            verlaufwerte_y.append(ui->fp_3->text().toDouble() * qSin(ui->ep_3->text().toDouble() * verlaufwerte_x[j] + ui->dp_3->text().toDouble()) + ui->cp_3->text().toDouble()); // Trigonometrische Regression
            ui->funktion->setText("y = " + QString::number(ui->fp_3->text().toDouble(),'f') + "⋅sin("
                                  + QString::number(ui->ep_3->text().toDouble(),'f') + "x+"
                                  + QString::number(ui->dp_3->text().toDouble(),'f') + ")+"
                                  + QString::number(ui->cp_3->text().toDouble(),'f') + "");
        }else if (ui->rb_quantil->isChecked()){
            verlaufwerte_y.append((ui->ep_3->text().toDouble() * verlaufwerte_x[j]) + ui->fp_3->text().toDouble()); // Quantile Regression
            ui->funktion->setText("y = " + QString::number(ui->ep_3->text().toDouble(),'f') + " * x + " + QString::number(ui->fp_3->text().toDouble(),'f'));
        }else{
            verlaufwerte_y.append(fx5(psetp, verlaufwerte_x[j])); // Polynomregression
            ui->funktion->setText("y = " + QString::number(ui->ap_3->text().toDouble(),'f') + "*x^5 +\n"
                                  + QString::number(ui->bp_3->text().toDouble(),'f') + "*x^4 +\n"
                                  + QString::number(ui->cp_3->text().toDouble(),'f') + "*x^3 +\n"
                                  + QString::number(ui->dp_3->text().toDouble(),'f') + "*x^2 +\n"
                                  + QString::number(ui->ep_3->text().toDouble(),'f') + "*x +\n"
                                  + QString::number(ui->fp_3->text().toDouble(),'f'));
        }
    }
    ui->mse->setValue( calculateMSE(vorgabewerte_x, vorgabewerte_y, psetp));
    ui->dia_darstellung->addGraph(xachse, yachse);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setData(verlaufwerte_x, verlaufwerte_y);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setPen(QPen(Qt::black));
    ui->dia_darstellung->addGraph(xachse, yachse);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setData(x_werte, y_werte);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setPen(pen);
    ui->dia_darstellung->addGraph(xachse, yachse);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setData(vorgabewerte_x, vorgabewerte_y);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setPen(QPen(Qt::black));
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setLineStyle(QCPGraph::lsNone);
    ui->dia_darstellung->graph(ui->dia_darstellung->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    ui->dia_darstellung->xAxis->setRange(verlauf_beginn, verlauf_ende);
    // qDebug() << y_werte;
    double y_max = *max_element(y_werte.begin(),y_werte.end()) * ui->y_hoehenfaktor->value();
    ui->dia_darstellung->yAxis->setRange(0,y_max);
    ui->dia_darstellung->setInteraction(QCP::iRangeZoom, true);      // Zoomen mit Mausrad
    ui->dia_darstellung->setInteraction(QCP::iRangeDrag, true);      // Verschieben mit linker Maustaste
    ui->dia_darstellung->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->dia_darstellung->replot();
}

QVector<double> MainWindow::para_lesen_pumpe(){
    QVector<double> para_pumpe;
    para_pumpe.append(ui->fp_3->text().toDouble());
    para_pumpe.append(ui->ep_3->text().toDouble());
    para_pumpe.append(ui->dp_3->text().toDouble());
    para_pumpe.append(ui->cp_3->text().toDouble());
    para_pumpe.append(ui->bp_3->text().toDouble());
    para_pumpe.append(ui->ap_3->text().toDouble());
    return(para_pumpe);
}

double MainWindow::fx5(QVector<double> pset, double x){
    return pset[5] * pow(x, 5)
        + pset[4] * pow(x, 4)
        + pset[3] * pow(x, 3)
        + pset[2] * pow(x, 2)
        + pset[1] * pow(x, 1)
        + pset[0];
}

double MainWindow::calculateMSE(QVector<double> x_values,
                    QVector<double> y_values,
                    QVector<double> coefficients) {
    double mse = 0.0;
    int n = x_values.size();
    for (int i = 0; i < n; ++i) {
        double y_actual = y_values[i];
        double y_pred = fx5( coefficients, x_values[i]);
        mse += pow(y_actual - y_pred, 2);
    }
    mse /= n; // Mittelwert bilden
    return mse;
}

void MainWindow::on_pushButton_4_clicked(){
    if (ui->rb_logarithmisch->isChecked()){
        beispieldaten({10,20,30,40,50,60,70},{55,50,44,41,39,35,34});
    }else if (ui->rb_linear->isChecked()){
        beispieldaten({10,20,30,40,50,60,80},{51,49,46,44,40,35,28});
    }else if (ui->rb_poly3g->isChecked()){
        beispieldaten({10,20,30,40,50,60,70},{50,49,46,44,40,35,28});
    }else if (ui->rb_poly4g->isChecked()){
        beispieldaten({-10,20,30,40,50,60,72},{50,49,46,44,40,35,28});
    }else if (ui->rb_poly5g->isChecked()){
        beispieldaten({-10,20,30,40,50,60,74},{50,49,46,44,40,35,28});
    }else if (ui->rb_exponentiell->isChecked()){
        beispieldaten({10,20,30,40,50,60,90},{60,50,44,41,39,35,30});
    }else if (ui->rb_logistisch->isChecked()){
        beispieldaten({10,20,30,40,50,60,70},{1,1.5,1,21,20,23,21});
    }else if (ui->rb_trigonometrisch->isChecked()){
        beispieldaten({10,20,30,40,50,60,70},{0,0.84,0.91,0.14,-0.76,-0.99,-0.28,-0,14});
    }
}

void MainWindow::beispieldaten(QVector<double> x, QVector<double> y){
    ui->xp1_3->setValue(x[0]);
    ui->xp2_3->setValue(x[1]);
    ui->xp3_3->setValue(x[2]);
    ui->xp4_3->setValue(x[3]);
    ui->xp5_3->setValue(x[4]);
    ui->xp6_3->setValue(x[5]);
    ui->xp7_3->setValue(x[6]);
    ui->yp1_3->setValue(y[0]);
    ui->yp2_3->setValue(y[1]);
    ui->yp3_3->setValue(y[2]);
    ui->yp4_3->setValue(y[3]);
    ui->yp5_3->setValue(y[4]);
    ui->yp6_3->setValue(y[5]);
    ui->yp7_3->setValue(y[6]);
}

void MainWindow::on_pushButton_6_clicked(){
    QUrl url("https://www.paypal.com/donate?business=tomax63%40web.de");
    if (!QDesktopServices::openUrl(url)) {
        qWarning("Failed to open the URL.");
    }
}

void MainWindow::on_pushButton_7_clicked(){
    on_pushButton_6_clicked();
}

void MainWindow::on_pushButton_10_clicked(){
    auto app = static_cast<QApplication *>(QApplication::instance());
    app->removeTranslator(&translator);
    if (translator.load(QCoreApplication::applicationDirPath() + "/CurveFit_en_GB.qm")) {
        app->installTranslator(&translator);
    } else {
        qWarning("Die Sprachdatei konnte nicht geladen werden.");
    }
    this->retranslateUi();
}

void MainWindow::retranslateUi() {
    ui->retranslateUi(this);
}

void MainWindow::on_pushButton_2_clicked(){
    meldung(tr("In diesem Fenster können Koordinaten grafisch digitalisiert werden.\n"
               "Zunächst wird mit dem Button 'Bild laden' eine Grafik gewählt, die die Pumpenkurven enthält. Diese Grafik sollte im Format PNG oder JPG vorliegen.\n"
               "Alternativ kann euch ein Bild aus der Zwischenablage eingefügt werden. Mit dem Haken in dem Feld 'proportional' wird das Bild eingefügt wie kopiert. "
               "Wird der Haken entfernt, wird das Bild auf maximale Grösse gedehnt. "
               "Danach erfolgt die Eichung der eingelesenen Grafik, indem die min- und max-Werte der Achsen des Diagramms angeklickt werden. Zusätzlich sollten "
               "vorher die entsprechenden Grenzwerte der Grafik in die Felder unter 'Wert' eingegeben werden. Damit weiss das Programm dann, welche Minima und Maxima "
               "berücksichtigt werden sollen.\n"
               "Mit den Kästchen 'logX' und 'logY' kann festgelegt werden, dass es sich bei der einzulesenden Grafik um eine logarhytmische Darstellung handelt.\n"
               "Danach klickt man auf sieben Punkte, worauf deren Koordinaten in der rechten Tabelle erscheinen.\n"
               "Nach Abschluss der Digitalisierung können die Koordinaten mit Klick auf den Button 'senden' "
               "in den Reiter 'Digitalisierung' transferiert werden, wo sie dann zur Berechnung bereit stehen.").toStdString());
}


void MainWindow::on_pushButton_11_clicked(){
    auto app = static_cast<QApplication *>(QApplication::instance());
    app->removeTranslator(&translator);
    this->retranslateUi();
}

