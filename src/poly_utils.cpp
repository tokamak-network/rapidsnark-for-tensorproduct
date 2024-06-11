#include <system_error>
#include <string>
#include <memory.h>
#include <stdexcept>
#include <alt_bn128.hpp>
#include "poly_utils.hpp"
#include <cmath>
#include <iostream>

namespace PolyUtils {

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::tensorProduct(FrElementMatrix& array1, FrElementMatrix& array2) {
    size_t numRows = array1.size();
    size_t numCols = array2[0].size();
    FrElementMatrix zeroMatrix(1, FrElementVector(1, AltBn128::Fr.zero()));

    if (numRows == 1 || numCols == 1) {
        return zeroMatrix;
    }

    FrElementMatrix result(numRows, FrElementVector(numCols));

    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            AltBn128::Fr.mul(result[i][j], array1[i][0], array2[0][j]);
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::addPoly(FrElementMatrix& poly1, FrElementMatrix& poly2) {
    size_t numRows = std::max(poly1.size(), poly2.size());
    size_t numCols = 0;

    if (!poly1.empty()) numCols = std::max(numCols, poly1[0].size());
    if (!poly2.empty()) numCols = std::max(numCols, poly2[0].size());

    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    for (size_t i = 0; i < poly1.size(); ++i) {
        for (size_t j = 0; j < poly1[i].size(); ++j) {
            result[i][j] = poly1[i][j];
        }
    }

    for (size_t i = 0; i < poly2.size(); ++i) {
        for (size_t j = 0; j < poly2[i].size(); ++j) {
            if (i < result.size() && j < result[i].size()) {
                AltBn128::Fr.add(result[i][j], result[i][j], poly2[i][j]);
            } else {
                if (j >= result[i].size()) {
                    result[i].resize(j + 1, AltBn128::Fr.zero());
                }
                result[i][j] = poly2[i][j];
            }
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::mulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2) {
    if (poly1.empty() || poly2.empty()) {
        return FrElementMatrix();
    }

    size_t numRows1 = poly1.size();
    size_t numRows2 = poly2.size();
    size_t numCols1 = poly1[0].size();
    size_t numCols2 = poly2[0].size();

    size_t resultRows = numRows1 + numRows2 - 1;
    size_t resultCols = numCols1 + numCols2 - 1;

    FrElementMatrix result(resultRows, FrElementVector(resultCols, AltBn128::Fr.zero()));

    typename Engine::FrElement temp;
    for (size_t i = 0; i < numRows1; ++i) {
        for (size_t j = 0; j < numCols1; ++j) {
            for (size_t k = 0; k < numRows2; ++k) {
                for (size_t l = 0; l < numCols2; ++l) {
                    AltBn128::Fr.mul(temp, poly1[i][j], poly2[k][l]);
                    AltBn128::Fr.add(result[i + k][j + l], result[i + k][j + l], temp);
                }
            }
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::subPoly(FrElementMatrix& poly1, FrElementMatrix& poly2) {
    size_t numRows = std::max(poly1.size(), poly2.size());
    size_t numCols = 0;

    for (const auto& row : poly1) {
        if (!row.empty()) numCols = std::max(numCols, row.size());
    }
    for (const auto& row : poly2) {
        if (!row.empty()) numCols = std::max(numCols, row.size());
    }

    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    for (size_t i = 0; i < poly1.size(); ++i) {
        for (size_t j = 0; j < poly1[i].size(); ++j) {
            result[i][j] = poly1[i][j];
        }
    }
    
    for (size_t i = 0; i < poly2.size(); ++i) {
        for (size_t j = 0; j < poly2[i].size(); ++j) {
            if (i < result.size() && j < result[i].size()) {
                AltBn128::Fr.sub(result[i][j], result[i][j], poly2[i][j]);
            }
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::scalePoly(FrElementMatrix& poly, typename Engine::FrElement& scaler) {
    size_t numRows = poly.size();
    size_t numCols = poly[0].size();

    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            AltBn128::Fr.mul(result[i][j], poly[i][j], scaler);
        }
    }

    return result;
}

template <typename Engine>
size_t Poly<Engine>::checkDim(FrElementMatrix& array) {
    size_t row = array.size();
    size_t col = array[0].size();

    if (row == 1) return 1;
    if (col == 1) return -1;

    return 0;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::fftMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    size_t shape1 = checkDim(poly1);
    size_t shape2 = checkDim(poly2);

    if (shape1 == 0 && shape2 == 0) {
        return _fft2dMulPoly(poly1, poly2, fft);
    }

    FrElementMatrix vector1 = poly1;
    FrElementMatrix vector2 = poly2;

    if (shape1 != shape2) {
        bool isColumnVector = shape2 == -1;
        if (isColumnVector) {
            vector1 = _transpose(vector1);
            vector2 = _transpose(vector2);
        }

        FrElementMatrix reducedPoly1 = reduceDimPoly(vector1);
        FrElementMatrix result;

        for (size_t i = 0; i < reducedPoly1.size(); ++i) {
            result.push_back(_fft1dMulPoly(reducedPoly1[i], vector2[0], fft));
        }

        if (isColumnVector) {
            return _transpose(result);
        }
        return result;
    }
    return _fft2dMulPoly(vector1, vector2, fft);
}

template <typename Engine>
typename Poly<Engine>::FrElementVector Poly<Engine>::_fft1dMulPoly(FrElementVector& poly1, FrElementVector& poly2, FFT<typename Engine::Fr>* fft) {
    FrElementVector vector1 = poly1;
    FrElementVector vector2 = poly2;

    size_t degree = vector1.size() + vector2.size() - 1;
    size_t padSize = minPowerOfTwo(degree);

    vector1.resize(padSize, AltBn128::Fr.zero());
    vector2.resize(padSize, AltBn128::Fr.zero());

    fft->fft(vector1.data(), padSize);
    fft->fft(vector2.data(), padSize);

    FrElementVector result(padSize);
    for (size_t i = 0; i < padSize; ++i) {
        AltBn128::Fr.mul(result[i], vector1[i], vector2[i]);
    }

    fft->ifft(result.data(), padSize);

    result.resize(degree);
    return result;
}

template <typename Engine>
size_t Poly<Engine>::minPowerOfTwo(size_t x) {
    if (x < 2) {
        return 2;
    }
    unsigned int power = static_cast<unsigned int>(std::ceil(std::log2(x)));
    return static_cast<size_t>(std::pow(2, power));
}

template <typename Engine>
void Poly<Engine>::paddingMatrix(FrElementMatrix& matrix, size_t targetRowLength, size_t targetColLength) {
    size_t currentRowLength = matrix.size();
    size_t currentColLength = currentRowLength > 0 ? matrix[0].size() : 0;

    if (currentColLength < targetColLength) {
        for (auto& row : matrix) {
            row.resize(targetColLength, AltBn128::Fr.zero());
        }
    }

    if (currentRowLength < targetRowLength) {
        FrElementVector newRow(targetColLength, AltBn128::Fr.zero());
        matrix.resize(targetRowLength, newRow);
    }
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::reduceDimPoly(FrElementMatrix& coefs) {
    auto [xOrder, yOrder] = _orderPoly(coefs);

    if (xOrder == -1 || yOrder == -1) return FrElementMatrix();

    FrElementMatrix reducedPoly(xOrder + 1, FrElementVector(yOrder + 1, AltBn128::Fr.zero()));
    for (int i = 0; i <= xOrder; ++i) {
        for (int j = 0; j <= yOrder; ++j) {
            reducedPoly[i][j] = coefs[i][j];
        }
    }

    return reducedPoly;
}

template <typename Engine>
typename Poly<Engine>::OrderPoly Poly<Engine>::_orderPoly(FrElementMatrix& coefs) {
    OrderPoly order;
    order.xOrder = -1;
    order.yOrder = -1;

    int numRows = coefs.size();
    int numCols = numRows > 0 ? coefs[0].size() : 0;

    for (int i = numRows - 1; i >= 0; --i) {
        for (int j = numCols - 1; j >= 0; --j) {
            if (!AltBn128::Fr.isZero(coefs[i][j])) {
                order.xOrder = i;
                order.yOrder = j;
                return order;
            }
        }
    }

    return order;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_transpose(const FrElementMatrix& matrix) {
    size_t numRows = matrix.size();
    size_t numCols = matrix.empty() ? 0 : matrix[0].size();

    FrElementMatrix transposedMatrix(numCols, FrElementVector(numRows, AltBn128::Fr.zero()));

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }

    return transposedMatrix;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_fft2dMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    size_t xDegree = poly1.size() + poly2.size() - 1;
    size_t yDegree = poly1[0]..size() + poly2[0].size() - 1;

    size_t xPad = minPowerOfTwo(xDegree);
    size_t yPad = minPowerOfTwo(yDegree);

    paddingMatrix(poly1, xPad, yPad);
    paddingMatrix(poly2, xPad, yPad);

    FrElementMatrix result(xPad, FrElementVector(yPad));

    for (size_t i = 0; i < xPad; ++i) {
        fft->fft(poly1[i].data(), yPad);
        fft->fft(poly2[i].data(), yPad);
    }

    for (size_t i = 0; i < xPad; ++i) {
        for (size_t j = 0; j < yPad; ++j) {
            AltBn128::Fr.mul(result[i][j], poly1[i][j], poly2[i][j]);
        }
    }

    for (size_t i = 0; i < xPad; ++i) {
        fft->ifft(result[i].data(), yPad);
    }

    result.resize(xDegree);
    for (size_t i = 0; i < xDegree; ++i) {
        result[i].resize(yDegree);
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::QapDiv(FrElementMatrix& pXY, size_t n, size_t sMax, FFT<typename Engine::Fr>* fft) {
    FrElementMatrix reducedPXY = reduceDimPoly(pXY);
    FrElementMatrix transposedPXY = _transpose(reducedPXY);

    size_t targetDegreeX = 2 * n - 2;
    size_t targetDegreeY = 2 * sMax - 2;

    size_t padX = minPowerOfTwo(targetDegreeX + 1);
    size_t padY = minPowerOfTwo(targetDegreeY + 1);

    paddingMatrix(transposedPXY, padX, padY);

    FrElementMatrix tX = createTX(n, padX, fft);
    FrElementMatrix tY = createTY(sMax, padY, fft);

    FrElementMatrix tXTY = _fft2dMulPoly(tX, tY, fft);
    FrElementMatrix qXY = divideByTXandTY(transposedPXY, tXTY, fft);

    FrElementMatrix finalQXY = _transpose(qXY);

    finalQXY.resize(n);
    for (auto& row : finalQXY) {
        row.resize(sMax);
    }

    return finalQXY;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::createTX(size_t n, size_t padX, FFT<typename Engine::Fr>* fft) {
    FrElementMatrix tX(1, FrElementVector(padX, AltBn128::Fr.zero()));
    tX[0][n-1] = AltBn128::Fr.one();
    tX[0][0] = AltBn128::Fr.negOne();
    fft->fft(tX[0].data(), padX);
    return tX;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::createTY(size_t sMax, size_t padY, FFT<typename Engine::Fr>* fft) {
    FrElementMatrix tY(1, FrElementVector(padY, AltBn128::Fr.zero()));
    tY[0][sMax-1] = AltBn128::Fr.one();
    tY[0][0] = AltBn128::Fr.negOne();
    fft->fft(tY[0].data(), padY);
    return tY;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::divideByTXandTY(FrElementMatrix& pXY, FrElementMatrix& tXTY, FFT<typename Engine::Fr>* fft) {
    for (size_t i = 0; i < pXY.size(); ++i) {
        fft->fft(pXY[i].data(), pXY[i].size());
        for (size_t j = 0; j < pXY[i].size(); ++j) {
            AltBn128::Fr.mul(pXY[i][j], pXY[i][j], tXTY[0][j]);
        }
        fft->ifft(pXY[i].data(), pXY[i].size());
    }
    return pXY;
}

template class PolyUtils::Poly<AltBn128::Engine>;

} // namespace PolyUtils
