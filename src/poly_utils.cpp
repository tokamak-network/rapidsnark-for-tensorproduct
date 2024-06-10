#include <system_error>
#include <string>
#include <memory.h>
#include <stdexcept>
#include <alt_bn128.hpp>
#include "poly_utils.hpp"
#include <cmath>

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
typename Poly<Engine>::FrElementMatrix Poly<Engine>::fftMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    // 다차원 다항식 차원 줄이기
    FrElementMatrix reducedPoly1 = reduceDimPoly(poly1);
    FrElementMatrix reducedPoly2 = reduceDimPoly(poly2);

    // 1D 다항식인지 확인
    bool is1DPoly1 = reducedPoly1.size() == 1 || reducedPoly1[0].size() == 1;
    bool is1DPoly2 = reducedPoly2.size() == 1 || reducedPoly2[0].size() == 1;

    if (is1DPoly1 && is1DPoly2) {
        return _fft1dMulPoly(reducedPoly1, reducedPoly2, fft);
    } else {
        return _fft2dMulPoly(reducedPoly1, reducedPoly2, fft);
    }
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_fft2dMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    size_t xDegree = poly1.size() + poly2.size() - 1;
    size_t yDegree = poly1[0].size() + poly2[0].size() - 1;

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
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_fft1dMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    FrElementVector vector1 = poly1[0];
    FrElementVector vector2 = poly2[0];

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
    return {result};
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
    PolyOrder order;
    order.xOrder = -1;
    order.yOrder = -1;
    order.coefficient = AltBn128::Fr.zero();

    int numRows = coefs.size();
    int numCols = numRows > 0 ? coefs[0].size() : 0;

    for (int i = numRows - 1; i >= 0; --i) {
        for (int j = numCols - 1; j >= 0; --j) {
            if (!AltBn128::Fr.isZero(coefs[i][j])) {
                order.xOrder = i;
                order.yOrder = j;
                order.coefficient = coefs[i][j];
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

// template <typename Engine>
// typename Poly<Engine>::FrElementMatrix Poly<Engine>::fftMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
//     FrElementMatrix reducedPoly1 = reduceDimPoly(poly1);
//     FrElementMatrix reducedPoly2 = reduceDimPoly(poly2);

//     size_t numRows = reducedPoly1.size();
//     size_t numCols = reducedPoly1[0].size();

//     size_t numRows2 = reducedPoly2.size();
//     size_t numCols2 = reducedPoly2[0].size();

//     size_t resultRows = numRows + numRows2 - 1;
//     size_t resultCols = numCols + numCols2 - 1;

//     size_t xPad = minPowerOfTwo(resultRows);
//     size_t yPad = minPowerOfTwo(resultCols);

//     paddingMatrix(reducedPoly1, xPad, yPad);
//     paddingMatrix(reducedPoly2, xPad, yPad);

//     for (size_t i = 0; i < xPad; ++i) {
//         fft->fft(reducedPoly1[i].data(), yPad);
//         fft->fft(reducedPoly2[i].data(), yPad);
//     }

//     FrElementMatrix result(xPad, FrElementVector(yPad));
//     for (size_t i = 0; i < xPad; ++i) {
//         for (size_t j = 0; j < yPad; ++j) {
//           printf("%s \n", AltBn128::Fr.toString(reducedPoly1[i][j]).c_str());
//           printf("%s \n", AltBn128::Fr.toString(reducedPoly2[i][j]).c_str());
//             AltBn128::Fr.mul(result[i][j], reducedPoly1[i][j], reducedPoly2[i][j]);
//         }
//     }

//     for (size_t i = 0; i < xPad; ++i) {
//         fft->ifft(result[i].data(), yPad);
//     }

//     result.resize(resultRows);
//     for (size_t i = 0; i < resultRows; ++i) {
//         result[i].resize(resultCols);
//     }

//     return result;
// }

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_autoTransFromObject(StringMatrix& input) {
    size_t numRows = input.size();
    size_t numCols = numRows > 0 ? input[0].size() : 0;

    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            AltBn128::Fr.fromString(result[i][j], input[i][j]);
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::StringMatrix Poly<Engine>::_transToObject(FrElementMatrix& input) {
    size_t numRows = input.size();
    size_t numCols = numRows > 0 ? input[0].size() : 0;

    StringMatrix result(numRows, StringVector(numCols));

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            result[i][j] = AltBn128::Fr.toString(input[i][j]);
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::PolyOrder Poly<Engine>::_findOrder(FrElementMatrix& coefs, int dir) {
    PolyOrder order;
    order.xOrder = -1;
    order.yOrder = -1;
    order.coefficient = AltBn128::Fr.zero();

    int numRows = coefs.size();
    int numCols = numRows > 0 ? coefs[0].size() : 0;

    if (dir == 0) {
        for (int i = numRows - 1; i >= 0; --i) {
            for (int j = numCols - 1; j >= 0; --j) {
                if (!AltBn128::Fr.isZero(coefs[i][j])) {
                    order.xOrder = i;
                    order.yOrder = j;
                    order.coefficient = coefs[i][j];
                    return order;
                }
            }
        }
    } else if (dir == 1) {
        for (int j = numCols - 1; j >= 0; --j) {
            for (int i = numRows - 1; i >= 0; --i) {
                if (!AltBn128::Fr.isZero(coefs[i][j])) {
                    order.xOrder = i;
                    order.yOrder = j;
                    order.coefficient = coefs[i][j];
                    return order;
                }
            }
        }
    }

    return order;
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
