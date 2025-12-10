#ifndef CLASSANALOGDIGITALMETER_H
#define CLASSANALOGDIGITALMETER_H

#include <vector>
#include <cassert>

/**
 * @brief A vector with a fixed size to avoid reallocation
 */
template <typename T, std::size_t MaxSize>
class static_vector {
public:
    using value_type = T;
    using size_type  = std::size_t;
    using iterator = T*;
    using const_iterator = const T*;

    static_vector() : size_(0) {}

    // Initialization
    static_vector(size_t size, const T& initVal) : size_(size) {
        assert(size <= MaxSize);
        for (size_t i = 0; i < size; ++i) data_[i] = initVal;
    }

    // Access
    T& operator[](size_type i) {
        assert(i < size_);
        return data_[i];
    }

    const T& operator[](size_type i) const {
        assert(i < size_);
        return data_[i];
    }

    T* data() { return data_; }
    const T* data() const { return data_; }

    iterator begin() { return data_; }
    iterator end()   { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end()   const { return data_ + size_; }

    size_type size() const { return size_; }

private:
    T data_[MaxSize];
    size_type size_;
};

class ClassAnalogDigitalMeter
{
public:
    static const size_t MaxDigits = 16;
    using DigitVector = static_vector<double, MaxDigits>;

    enum TransitionMode
    {
        Rolling, /// Continously Rolling like a dial
        Digital  /// Transitioning, only when the next dial / digit passes a certain threshold
    };

    ClassAnalogDigitalMeter(size_t nDigits, size_t nDials);

    /**
     *  @brief Set the last digit transition model.
     *
     *  If the meter only consists of digits, the last digit is probably of type Model_Analog.
     *  Otherwise, the last digit is typically of type Model_Digital.
     *  When the last digit rolls continuously though,
     *  the model of the last digit needs to be switched to Model_Analog.
     *
     *  @param mode The mode to set for the last digit
     */
    ClassAnalogDigitalMeter& setLastDigitTransitionMode(TransitionMode mode);

    /**
     * @brief Sets the number of decimal places (after the comma)
     */
    ClassAnalogDigitalMeter& setNDecimalPlaces(size_t);

    ClassAnalogDigitalMeter& setAnalogDigitalTransitionEnd(double val);

    /**
     * @brief Computes the digit and dial position for a
              certain meter value

     * @param value The meter value to simulate
     * @return array of digit and dial values in range [0, 10)
     */
    std::vector<double> forward(double value) const;

    struct Result
    {
        double value{0.};
        double score{0.};
    };

    Result getValue(const std::vector<double>& meterValues, bool extendedResolution=false) const;

    double getDigitError(size_t digitIndex) const;

    static double digitTransitionFunction(double phase, double dt);

private:
    /**
     * @brief Simulate an analog/rolling decimal digit display for a scalar value.
     *
     * This models how each decimal digit of ``value`` would appear on an
     * analog-style rolling display (like an odometer), where digits
     * transition smoothly to the next value over a finite phase window.
     *
     * Each digit has its own phase offset (``m_phaseDetuneVec``) to account for
     * misalignment or detuning between digit wheels. The transition width
     * is controlled by ``dt``: smaller values give a sharper, more
     * instantaneous change; larger values give a longer rolling transition.
     *
     * @param value   The true numeric value to be represented. Typically a
     *                non-negative value; only its decimal digits are used.
     *
     * @param iDigit  The index of the digit to be computed starting from
     *                the most-significant digit first
     *
     * @param dt      Width of the transition region in phase space, in [0, 1).
     *                A value of 0.1 means each digit takes 10% of a full cycle
     *                to roll from its current value to the next.
     *
     * @return        The modeled digit value. This is generally a non-integer in the
     *                transition region and are taken modulo 10, so they are
     *                suitable for rendering as analog/rolling digits.
     */
    double digitModel(double value, size_t iDigit, double dt) const;


    /**
     * @brief Model a multi-decade analog display with rotating hands on 0–9 dials.
     *
     * For each decimal digit of ``value``, this function imagines a circular dial
     * labeled 0–9 with a single hand. The hand moves *continuously* as the
     * underlying value changes:
     *
     * - The integer decimal digit sets the base position on the dial.
     * - The phase within the current decade (0..1) moves the hand between
     *   the current digit and the next.
     *
     * @param value   The true numeric value to be represented. Typically a
     *                non-negative value; only its decimal digits their phases are used.
     *
     * @param iDigit  The index of the digit to be computed starting from
     *                the most-significant digit first
     *
     * @return        The hand positions on each 0–9 dial, wrapped modulo 10.
     */
    double dialModel(double value, size_t iDigit) const;

    void forwardImpl(double value, DigitVector& result) const;

    double correctionValue() const;


    size_t baseIndex(size_t digitIdx) const;

    /// Number of analog digits
    size_t m_nDigits;
    /// Number of analog dials (typically using hands)
    size_t m_nDials;

    /// Number of decimal places (after the comma), ofter the same as nDials
    size_t m_nDecimalPlaces;

    TransitionMode m_lastDigitTransitionMode{Digital};

    /// The phase width of a digital transition
    /// A dial often start transition at phase = 0.9 and ends at phase = 1.0
    double m_digitalTransitionWidth{0.1};

    /// The phase detune for each analog unit
    /// each value in range [0, 1)
    /// +0.3 means, that the dial is rotated 30% clockwise from its place where it should actually be
    DigitVector m_phaseDetuneVec;

    /// the statistical read-out error of digits
    double m_ocrDigitError{0.2};
    /// the statistical read-out error of dials
    double m_ocrDialError{0.3};
};

namespace helper
{
double baseDigit(double value, size_t iDigit);
double decimalPhase(double value, size_t iDigit);

/**
 * @brief Compute the circular distance between values on a 0–10 circle.
 *
 * This returns the shortest distance between ``a`` and ``b`` on a
 * circular scale that wraps at 10. The distance is always in [0, 5].
 *
 * @return The circular distance(s) between ``a`` and ``b``
 */
double circDistance10(double a, double b);

/**
 * @brief Log of a gaussian shaped compatibility score for a deviation
 *
 * @param deltaX Difference between model prediction and ocr measurement
 * @param sigma  Expected error / standard deviation for this measurement
 * @return The log score
 */
double gaussianMatchLogScore(double deltaX, double sigma);
}

#endif // CLASSANALOGDIGITALMETER_H
