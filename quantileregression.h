#ifndef QUANTILEREGRESSION_H
#define QUANTILEREGRESSION_H

#include <QVector>

class QuantileRegression {
public:
    QuantileRegression(double quantile);
    double fit(const QVector<double>& x, const QVector<double>& y, QVector<double>& coefficients);

private:
    double quantile;
    double pinballLoss(double residual) const;
};

#endif // QUANTILEREGRESSION_H
