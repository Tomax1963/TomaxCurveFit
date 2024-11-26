#include "QuantileRegression.h"
#include <cmath>
#include <algorithm>
#include <numeric>

QuantileRegression::QuantileRegression(double quantile) : quantile(quantile) {}

double QuantileRegression::pinballLoss(double residual) const {
    if (residual >= 0) {
        return quantile * residual;
    } else {
        return (quantile - 1) * residual;
    }
}

double QuantileRegression::fit(const QVector<double>& x, const QVector<double>& y, QVector<double>& coefficients) {
    int n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("x and y must have the same size.");
    }

    // Initial coefficients (e.g., slope = 0, intercept = median of y)
    double slope = 0.0;
    double intercept = std::accumulate(y.begin(), y.end(), 0.0) / n;

    // Iterative optimization (simple gradient descent for example)
    double learning_rate = 0.01;
    int iterations = 1000;

    for (int iter = 0; iter < iterations; ++iter) {
        double grad_slope = 0.0;
        double grad_intercept = 0.0;

        for (int i = 0; i < n; ++i) {
            double pred = slope * x[i] + intercept;
            double residual = y[i] - pred;
            double grad = (residual >= 0) ? -quantile : (1 - quantile);

            grad_slope += grad * (-x[i]);
            grad_intercept += grad * (-1.0);
        }

        grad_slope /= n;
        grad_intercept /= n;

        slope -= learning_rate * grad_slope;
        intercept -= learning_rate * grad_intercept;
    }

    coefficients = {slope, intercept};
    return 0.0; // Return the loss or other metric if needed
}
