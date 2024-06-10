#include <system_error>
#include <string>
#include <memory.h>
#include <stdexcept>
#include <alt_bn128.hpp>
#include "poly_utils.hpp"
#include <cmath>  // cmath 헤더를 포함하여 수학 함수를 사용

namespace PolyUtils {

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::tensorProduct(FrElementMatrix& array1, FrElementMatrix& array2) {
    size_t numRows = array1.size();
    size_t numCols = array2[0].size();
    FrElementMatrix zeroMatrix(1, FrElementVector(1, AltBn128::Fr.zero()));

    if (numRows == 1) {
        return zeroMatrix;
    }

    if (numCols == 1) {
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

    // Add poly1 to the result
    for (size_t i = 0; i < poly1.size(); ++i) {
        for (size_t j = 0; j < poly1[i].size(); ++j) {
            result[i][j] = poly1[i][j];  // Direct assignment first
        }
    }

    // Add poly2 to the result
    for (size_t i = 0; i < poly2.size(); ++i) {
        for (size_t j = 0; j < poly2[i].size(); ++j) {
            if (i < result.size() && j < result[i].size()) {
                AltBn128::Fr.add(result[i][j], poly1[i][j], poly2[i][j]);
            } else {
                // Should resize internally if poly2 is larger, which is rare
                // Assuming rows exist but need more columns
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

    // 결과 다항식의 차수 계산
    size_t resultRows = numRows1 + numRows2 - 1;
    size_t resultCols = numCols1 + numCols2 - 1;

    // 결과 다항식의 계수를 0으로 초기화
    FrElementMatrix result(resultRows, FrElementVector(resultCols, AltBn128::Fr.zero()));

    typename Engine::FrElement temp;
    for (size_t i = 0; i < numRows1; ++i) {
        for (size_t j = 0; j < numCols1; ++j) {
            for (size_t k = 0; k < numRows2; ++k) {
                for (size_t l = 0; l < numCols2; ++l) {
                    // i+k, j+l 위치에 결과 저장
                    // 결과 배열이 충분히 큰지 확인하고 필요하면 확장
                    if (i + k >= result.size() || j + l >= result[i + k].size()) {
                        if (i + k >= result.size()) {
                            result.resize(i + k + 1);
                        }
                        if (j + l >= result[i + k].size()) {
                            result[i + k].resize(j + l + 1, AltBn128::Fr.zero());
                        }
                    }
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

    // 최대 열 길이 계산
    for (const auto& row : poly1) {
        if (!row.empty()) numCols = std::max(numCols, row.size());
    }
    for (const auto& row : poly2) {
        if (!row.empty()) numCols = std::max(numCols, row.size());
    }

    // 결과 다항식의 계수를 초기화
    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    // poly1의 계수를 결과에 할당
    for (size_t i = 0; i < poly1.size(); ++i) {
        for (size_t j = 0; j < poly1[i].size(); ++j) {
            result[i][j] = poly1[i][j];
        }
    }
    
    // poly2의 계수를 결과에서 빼기
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

    // 결과 다항식을 초기화합니다. 
    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    // 모든 계수에 스칼라를 곱합니다.
    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            AltBn128::Fr.mul(result[i][j], poly[i][j], scaler);
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::fftMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    // 차원 줄이기를 적용하여 다항식의 불필요한 차수를 제거
    FrElementMatrix reducedPoly1 = reduceDimPoly(poly1);
    FrElementMatrix reducedPoly2 = reduceDimPoly(poly2);

    size_t numRows = reducedPoly1.size();
    size_t numCols = reducedPoly1[0].size();

    size_t numRows2 = reducedPoly2.size();
    size_t numCols2 = reducedPoly2[0].size();

    // 결과 다항식의 차수 계산
    size_t resultRows = numRows + numRows2 - 1;
    size_t resultCols = numCols + numCols2 - 1;

    // FFT를 위한 배열 크기 결정 (2의 거듭제곱으로)
    size_t xPad = minPowerOfTwo(resultRows);
    size_t yPad = minPowerOfTwo(resultCols);

    // Padding 적용
    paddingMatrix(reducedPoly1, xPad, yPad);
    paddingMatrix(reducedPoly2, xPad, yPad);

    // 각 행에 대해 FFT 수행
    for (size_t i = 0; i < xPad; ++i) {
        fft->fft(reducedPoly1[i], yPad);
        fft->fft(reducedPoly2[i], yPad);
    }

    // 행렬의 각 요소를 곱함
    FrElementMatrix result(xPad, FrElementVector(yPad));
    for (size_t i = 0; i < xPad; ++i) {
        for (size_t j = 0; j < yPad; ++j) {
            AltBn128::Fr.mul(result[i][j], reducedPoly1[i][j], reducedPoly2[i][j]);
        }
    }

    // 역 FFT 수행
    for (size_t i = 0; i < xPad; ++i) {
        fft->ifft(result[i], yPad);
    }

    // 결과 배열 크기 조정
    result.resize(resultRows);
    for (size_t i = 0; i < resultRows; ++i) {
        result[i].resize(resultCols);
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::QapDiv(FrElementMatrix& pXY, size_t n, size_t sMax, FFT<typename Engine::Fr>* fft) {
    // 차원 줄이기를 적용하여 다항식의 불필요한 차수를 제거
    FrElementMatrix reducedPXY = reduceDimPoly(pXY);

    // Transpose P(X, Y) to work along the other dimension
    FrElementMatrix transposedPXY = _transpose(reducedPXY);

    // Setup for division
    size_t targetDegreeX = 2 * n - 2;
    size_t targetDegreeY = 2 * sMax - 2;

    // Pad the matrices to the nearest power of two that fits the operation
    size_t padX = minPowerOfTwo(targetDegreeX + 1);
    size_t padY = minPowerOfTwo(targetDegreeY + 1);

    paddingMatrix(transposedPXY, padX, padY);

    // Prepare the divisor polynomials t(X) = X^(n-1) - 1 and t(Y) = Y^(sMax-1) - 1
    FrElementMatrix tX = createTX(n, padX, fft);
    FrElementMatrix tY = createTY(sMax, padY, fft);

    // Compute Q(X, Y) = P(X, Y) / (t(X) * t(Y))
    // First multiply tX and tY using FFT
    FrElementMatrix tXTY = _fft2dMulPoly(tX, tY, fft);

    // Now divide P(X, Y) by t(X)*t(Y)
    FrElementMatrix qXY = divideByTXandTY(transposedPXY, tXTY, fft);

    // Transpose back to the original polynomial orientation
    FrElementMatrix finalQXY = _transpose(qXY);

    // Resize the final polynomial to remove any excess padding
    finalQXY.resize(n);
    for (auto& row : finalQXY) {
        row.resize(sMax);
    }

    return finalQXY;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::createTX(size_t n, size_t padX, FFT<typename Engine::Fr>* fft) {
    FrElementMatrix tX(1, FrElementVector(padX, AltBn128::Fr.zero()));
    tX[0][n-1] = AltBn128::Fr.one();  // X^(n-1)
    tX[0][0] = AltBn128::Fr.negOne();  // -1
    fft->fft(tX[0], padX);
    return tX;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::createTY(size_t sMax, size_t padY, FFT<typename Engine::Fr>* fft) {
    FrElementMatrix tY(1, FrElementVector(padY, AltBn128::Fr.zero()));
    tY[0][sMax-1] = AltBn128::Fr.one();  // Y^(sMax-1)
    tY[0][0] = AltBn128::Fr.negOne();  // -1
    fft->fft(tY[0], padY);
    return tY;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::divideByTXandTY(FrElementMatrix& pXY, FrElementMatrix& tXTY, FFT<typename Engine::Fr>* fft) {
    // Multiply P(X, Y) by the inverse of t(X)*t(Y)
    for (size_t i = 0; i < pXY.size(); ++i) {
        fft->fft(pXY[i], pXY[i].size());
        for (size_t j = 0; j < pXY[i].size(); ++j) {
            AltBn128::Fr.mul(pXY[i][j], pXY[i][j], tXTY[0][j]); // Multiply element-wise
        }
        fft->ifft(pXY[i], pXY[i].size());
    }
    return pXY;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_fft2dMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::Fr>* fft) {
    // 차수 감소 후 사용할 최소 크기 계산
    poly1 = reduceDimPoly(poly1);
    poly2 = reduceDimPoly(poly2);

    // 결과 다항식의 차수 계산
    size_t xDegree = poly1.size() + poly2.size() - 1;
    size_t yDegree = poly1[0].size() + poly2[0].size() - 1;

    // FFT를 위한 배열 크기 결정 (2의 거듭제곱으로)
    size_t xPad = minPowerOfTwo(xDegree);
    size_t yPad = minPowerOfTwo(yDegree);

    // Padding 적용
    paddingMatrix(poly1, xPad, yPad);
    paddingMatrix(poly2, xPad, yPad);

    // FFT 수행
    for (auto& row : poly1) {
        fft->fft(row, row.size());
    }
    for (auto& row : poly2) {
        fft->fft(row, row.size());
    }

    // 곱셈 수행
    FrElementMatrix result(xPad, FrElementVector(yPad));
    for (size_t i = 0; i < xPad; ++i) {
        for (size_t j = 0; j < yPad; ++j) {
            AltBn128::Fr.mul(result[i][j], poly1[i][j], poly2[i][j]);
        }
    }

    // 역 FFT 수행
    for (auto& row : result) {
        fft->ifft(row, row.size());
    }

    // 결과 배열 크기 조정
    result.resize(xDegree);
    for (auto& row : result) {
        row.resize(yDegree);
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::FrElementVector Poly<Engine>::_fft1dMulPoly(FrElementVector& poly1, FrElementVector& poly2, FFT<typename Engine::Fr>* fft) {
    // 입력 다항식의 차수 감소
    size_t xDegree = poly1.size() + poly2.size() - 1;

    // FFT를 위한 배열 크기 결정 (2의 거듭제곱으로)
    size_t xPad = minPowerOfTwo(xDegree);

    // Padding 적용
    poly1.resize(xPad, AltBn128::Fr.zero());
    poly2.resize(xPad, AltBn128::Fr.zero());

    // FFT 수행
    fft->fft(poly1, poly1.size());
    fft->fft(poly2, poly2.size());

    // 곱셈 수행
    FrElementVector result(xPad);
    for (size_t i = 0; i < xPad; ++i) {
        AltBn128::Fr.mul(result[i], poly1[i], poly2[i]);
    }

    // 역 FFT 수행
    fft->ifft(result, result.size());

    // 결과 배열 크기 조정
    result.resize(xDegree);

    return result;
}

template <typename Engine>
typename Poly<Engine>::OrderPoly Poly<Engine>::_orderPoly(FrElementMatrix& coefs) {
    // xOrder를 찾기 위해 각 행의 마지막으로 0이 아닌 요소를 찾습니다.
    int xOrder = -1;
    for (size_t i = 0; i < coefs.size(); ++i) {
        for (size_t j = coefs[i].size(); j > 0; --j) {
            if (!AltBn128::Fr.isZero(coefs[i][j-1])) {
                xOrder = std::max(xOrder, static_cast<int>(i));
                break;
            }
        }
    }

    // yOrder를 찾기 위해 각 열의 마지막으로 0이 아닌 요소를 찾습니다.
    int yOrder = -1;
    if (!coefs.empty()) {
        size_t numCols = coefs[0].size();
        for (size_t j = 0; j < numCols; ++j) {
            for (size_t i = coefs.size(); i > 0; --i) {
                if (!AltBn128::Fr.isZero(coefs[i-1][j])) {
                    yOrder = std::max(yOrder, static_cast<int>(j));
                    break;
                }
            }
        }
    }

    return {xOrder, yOrder};
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::reduceDimPoly(FrElementMatrix& coefs) {
    auto [xOrder, yOrder] = _orderPoly(coefs); // 최고 유효 차수를 얻습니다.

    if (xOrder == -1 || yOrder == -1) return FrElementMatrix(); // 모든 계수가 0인 경우

    // 필요한 차원만큼만 포함하는 새로운 다항식 배열을 생성합니다.
    FrElementMatrix reducedPoly(xOrder + 1, FrElementVector(yOrder + 1, AltBn128::Fr.zero()));
    for (int i = 0; i <= xOrder; ++i) {
        for (int j = 0; j <= yOrder; ++j) {
            reducedPoly[i][j] = coefs[i][j];
        }
    }

    return reducedPoly;
}

template <typename Engine>
void Poly<Engine>::paddingMatrix(FrElementMatrix& matrix, size_t targetRowLength, size_t targetColLength) {
    // 현재 행렬의 크기
    size_t currentRowLength = matrix.size();
    size_t currentColLength = currentRowLength > 0 ? matrix[0].size() : 0;

    // 열 길이 조정
    if (currentColLength < targetColLength) {
        for (auto& row : matrix) {
            row.resize(targetColLength, AltBn128::Fr.zero());
        }
    }

    // 행 길이 조정
    if (currentRowLength < targetRowLength) {
        FrElementVector newRow(targetColLength, AltBn128::Fr.zero());
        matrix.resize(targetRowLength, newRow);
    }
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
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_autoTransFromObject(StringMatrix& input) {
    size_t numRows = input.size();
    size_t numCols = numRows > 0 ? input[0].size() : 0;

    FrElementMatrix result(numRows, FrElementVector(numCols, AltBn128::Fr.zero()));

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            AltBn128::Fr.fromString(result[i][j], input[i][j]);  // 문자열에서 유한 필드 요소로 변환
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
            result[i][j] = AltBn128::Fr.toString(input[i][j]);  // 유한 필드 요소를 문자열로 변환
        }
    }

    return result;
}

template <typename Engine>
typename Poly<Engine>::PolyOrder Poly<Engine>::_findOrder(FrElementMatrix& coefs, int dir) {
    PolyOrder order;
    order.xOrder = -1; // 초기 차수 설정
    order.yOrder = -1; // 초기 차수 설정
    order.coefficient = AltBn128::Fr.zero(); // 초기 계수는 0

    int numRows = coefs.size();
    int numCols = numRows > 0 ? coefs[0].size() : 0;

    // 방향에 따라 찾는 로직 변경
    if (dir == 0) { // X 방향으로 가장 높은 차수를 찾음
        for (int i = numRows - 1; i >= 0; --i) {
            for (int j = numCols - 1; j >= 0; --j) {
                if (!AltBn128::Fr.isZero(coefs[i][j])) {
                    order.xOrder = i;
                    order.yOrder = j;
                    order.coefficient = coefs[i][j];
                    return order; // 첫 번째 비-0 계수를 찾으면 반환
                }
            }
        }
    } else if (dir == 1) { // Y 방향으로 가장 높은 차수를 찾음
        for (int j = numCols - 1; j >= 0; --j) {
            for (int i = numRows - 1; i >= 0; --i) {
                if (!AltBn128::Fr.isZero(coefs[i][j])) {
                    order.xOrder = i;
                    order.yOrder = j;
                    order.coefficient = coefs[i][j];
                    return order; // 첫 번째 비-0 계수를 찾으면 반환
                }
            }
        }
    }

    // 모든 계수가 0인 경우 -1 위치와 0 계수 반환
    return order;
}

template <typename Engine>
typename Poly<Engine>::FrElementMatrix Poly<Engine>::_transpose(const FrElementMatrix& matrix) {
    size_t numRows = matrix.size();
    size_t numCols = matrix.empty() ? 0 : matrix[0].size();

    // Create a new matrix with dimensions flipped
    FrElementMatrix transposedMatrix(numCols, FrElementVector(numRows, AltBn128::Fr.zero()));

    // Fill the new matrix with transposed values
    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }

    return transposedMatrix;
}

// Explicit template instantiation
template class PolyUtils::Poly<AltBn128::Engine>;

} // namespace PolyUtils
