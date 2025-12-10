#include "ClassAnalogDigitalMeter.h"

#include <cassert>
#include <cmath>

static constexpr const double pow10_table[] = {
    1.,                  // 1
    10.,                 // 2
    100.,                // 3
    1000.,               // 4
    10000.,              // 5
    100000.,             // 6
    1000000.,            // 7
    10000000.,           // 8
    100000000.,          // 9
    1000000000.,         // 10
    10000000000.,        // 11
    100000000000.,       // 12
    1000000000000.,      // 13
    10000000000000.,     // 14
    100000000000000.,    // 15
    1000000000000000.    // 16
};

constexpr const size_t pow10_table_size = std::size(pow10_table);

constexpr double pow10(size_t n)
{
    if (n < pow10_table_size)
        return pow10_table[n];

    double result = pow10_table[pow10_table_size-1];
    for (size_t i = pow10_table_size; i < n; ++i)
        result *= 10.;

    return result;
}

/**
 * @brief A python-like modulo that results in the range [0, y), i.e. avoid negative numbers
 *
 * @param x
 * @param y
 */
template <typename T>
inline T modulo(T x, T y)
{
    T r = std::fmod(x, y);
    return (r < 0.) ? (r + y) : r;
}


ClassAnalogDigitalMeter::ClassAnalogDigitalMeter(size_t nDigits, size_t nDials)
    : m_nDigits(nDigits)
    , m_nDials(nDials)
    , m_phaseDetuneVec(nDials + nDigits, 0.)
{
    assert(nDigits + nDials <= MaxDigits);

    // this is a good assumption unless specified otherwise
    m_nDecimalPlaces = nDials;

    // If the Meter consist only of digits, it is safe to assume that the last digit
    // must be rolling
    if (nDials == 0) {
        m_lastDigitTransitionMode = Rolling;
    }
    else {
        m_lastDigitTransitionMode = Digital;
    }
}

ClassAnalogDigitalMeter &ClassAnalogDigitalMeter::setLastDigitTransitionMode(TransitionMode mode)
{
    m_lastDigitTransitionMode = mode;
    return *this;
}

ClassAnalogDigitalMeter &ClassAnalogDigitalMeter::setNDecimalPlaces(size_t n)
{
    m_nDecimalPlaces = n;
    return *this;
}

ClassAnalogDigitalMeter &ClassAnalogDigitalMeter::setAnalogDigitalTransitionEnd(double val)
{
    val = modulo(val, 10.);

    if (val > 5) val = val - 10.;

    m_phaseDetuneVec[m_nDigits] = val;
    return *this;
}

std::vector<double> ClassAnalogDigitalMeter::forward(double value) const
{
    DigitVector result(m_nDigits + m_nDials, 0.);
    forwardImpl(value, result);
    return {result.begin(), result.end()};
}

void ClassAnalogDigitalMeter::forwardImpl(double value, DigitVector& result) const
{

    assert(result.size() == m_nDigits + m_nDials);

    /// The first entries are all digits, following by rolling scales
    /// Find out, at which index the behaviour changes

    size_t idxTransitionModeStop = m_nDigits;
    if (m_lastDigitTransitionMode == Rolling) {
        idxTransitionModeStop -= 1;
    }

    // start with the dials first
    for (int idx = m_nDigits + m_nDials - 1; idx >= idxTransitionModeStop; --idx) {
        result[idx] = dialModel(value, idx);
    }

    // continue with the digits
    double currentDt = m_digitalTransitionWidth;
    for (int idx = idxTransitionModeStop-1; idx >= 0; --idx) {
        result[idx] = digitModel(value, idx, currentDt);
        currentDt /= 10.;
    }
}

double ClassAnalogDigitalMeter::correctionValue() const
{
    double val = 0.;
    for (int idx = m_nDials + m_nDigits - 1; idx >= 0; --idx)
    {
        val += pow10(baseIndex(idx)) * m_phaseDetuneVec[idx];
    }

    return val;
}

ClassAnalogDigitalMeter::Result ClassAnalogDigitalMeter::getValue(
    const std::vector<double> &meterValues,
    bool extendedResolution) const
{
    const size_t nvals = m_nDials + m_nDigits;
    if (meterValues.size() != nvals) {
        return {-1., 0};
    }

    const double lastDigit = modulo(meterValues[nvals-1] - m_phaseDetuneVec[nvals-1], 10.);

    // buffer for the predicted values to avoid reallocations
    DigitVector predictedValues(nvals, 0.);

    double predictedValue = lastDigit;

    // Greedy search from least-significant to most-significant position, skipping last digit
    for (int iDigit = int(nvals - 2); iDigit >= 0; --iDigit) {
        const double base = pow10(baseIndex(iDigit));

        // the expected statistical readout error for this digit
        const double digitSigma = getDigitError(iDigit);

        double bestDigitScore = std::numeric_limits<double>::lowest();
        size_t bestDigitValue = 0;

        // find the best digit value for the current iteration
        for (size_t digitValue = 0; digitValue < 10; ++digitValue) {
            double candidateValue = predictedValue + base*digitValue;
            forwardImpl(candidateValue, predictedValues);

            // compare predicted values with meter values
            double dist = helper::circDistance10(predictedValues[iDigit], meterValues[iDigit]);
            const double digitScore = helper::gaussianMatchLogScore(dist, digitSigma);

            if (digitScore > bestDigitScore) {
                bestDigitScore = digitScore;
                bestDigitValue = digitValue;
            }
        }

        predictedValue += bestDigitValue * base;
    }

    // do the final simulation again with the best value
    forwardImpl(predictedValue, predictedValues);

    // compute total score
    double totalLogScore = 0.;
    for (int iDigit = 0; iDigit < nvals; ++iDigit) {
        double dist = helper::circDistance10(predictedValues[iDigit], meterValues[iDigit]);
        totalLogScore += helper::gaussianMatchLogScore(dist, getDigitError(iDigit));
    }

    if (extendedResolution)
        predictedValue = std::floor(predictedValue * 10.) / 10.;
    else
        predictedValue = std::floor(predictedValue);

    return {
        (predictedValue + correctionValue()) / pow10(m_nDecimalPlaces),
        std::exp(totalLogScore)
    };
}

double ClassAnalogDigitalMeter::getDigitError(size_t digitIndex) const
{
    if (digitIndex < m_nDigits) {
        return m_ocrDigitError;
    }
    else {
        return m_ocrDialError;
    }
}

double ClassAnalogDigitalMeter::digitTransitionFunction(double phase, double dt)
{
    const double transitionStart = 1. - dt;

    const double offset = std::floor(phase);

    // make phase into range [0, 1)
    phase = modulo(phase, 1.0);

    return offset + std::max(0., (phase - transitionStart) / dt);
}

double ClassAnalogDigitalMeter::digitModel(double value, size_t iDigit, double dt) const
{

    size_t bIdx = baseIndex(iDigit);

    // extract i-th digit from value
    const double base = helper::baseDigit(value, bIdx);
    const double phase = helper::decimalPhase(value, bIdx);

    const double localShift = ClassAnalogDigitalMeter::digitTransitionFunction(phase + m_phaseDetuneVec[iDigit], dt);

    return modulo(base + localShift, 10.);
}

double ClassAnalogDigitalMeter::dialModel(double value, size_t iDigit) const
{
    size_t bIdx = baseIndex(iDigit);

    double base = helper::baseDigit(value, bIdx);
    double phase = helper::decimalPhase(value, bIdx);

    return modulo(base + phase + m_phaseDetuneVec[iDigit], 10.);
}

size_t ClassAnalogDigitalMeter::baseIndex(size_t digitIdx) const
{
    assert(digitIdx < m_nDigits + m_nDials);

    return m_nDigits + m_nDials - digitIdx - 1;
}

double helper::baseDigit(double value, size_t iDigit)
{
    const double d = value / pow10(iDigit);
    return modulo(std::floor(d), 10.);
}

double helper::decimalPhase(double value, size_t iDigit)
{
    const double b = ::pow10(iDigit);
    return modulo(value, b) / b;
}


double helper::circDistance10(double a, double b)
{
    const double diff = std::fabs(a - b);
    return std::min(diff, 10. - diff);
}

double helper::gaussianMatchLogScore(double deltaX, double sigma)
{
    const double z = deltaX / sigma;
    return -0.5 * z * z;
}
