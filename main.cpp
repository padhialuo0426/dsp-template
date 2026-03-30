#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include "arm_math.h"

// 定义矩阵的维度
const uint16_t MATRIX_DIM = 1024;

// 嵌套 for 循环矩阵乘法实现
void matrixMultiplyNaive(const float32_t* A, const float32_t* B, float32_t* C, uint16_t N) {
    for (uint16_t i = 0; i < N; ++i) {
        for (uint16_t j = 0; j < N; ++j) {
            float32_t sum = 0.0f;
            for (uint16_t k = 0; k < N; ++k) {
                // A 的第 i 行乘以 B 的第 j 列
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main() {
    std::cout << "初始化矩阵数据 (" << MATRIX_DIM << "x" << MATRIX_DIM << ")...\n";

    // 使用 std::vector 在堆上分配内存，防止大矩阵导致栈溢出 (Stack Overflow)
    size_t numElements = MATRIX_DIM * MATRIX_DIM;
    std::vector<float32_t> srcA(numElements);
    std::vector<float32_t> srcB(numElements);
    std::vector<float32_t> dstNaive(numElements, 0.0f);
    std::vector<float32_t> dstDSP(numElements, 0.0f);

    // 填充随机浮点数，防止编译器因为数据全为 0 而进行过度优化
    std::mt19937 rng(42); // 固定种子，保证每次运行数据一致
    std::uniform_real_distribution<float32_t> dist(-1.0f, 1.0f);
    for (size_t i = 0; i < numElements; ++i) {
        srcA[i] = dist(rng);
        srcB[i] = dist(rng);
    }

    // ==========================================
    // for 循环计算
    // ==========================================
    std::cout << "\n开始运行传统 for 循环计算..." << std::endl;
    auto startNaive = std::chrono::high_resolution_clock::now();

    matrixMultiplyNaive(srcA.data(), srcB.data(), dstNaive.data(), MATRIX_DIM);

    auto endNaive = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timeNaive = endNaive - startNaive;
    std::cout << "for 循环耗时: " << std::fixed << std::setprecision(3) << timeNaive.count() << " ms\n";

    // ==========================================
    // CMSIS-DSP 库函数计算
    // ==========================================
    // 初始化 CMSIS-DSP 的矩阵结构体
    arm_matrix_instance_f32 matA, matB, matDSP;
    arm_mat_init_f32(&matA, MATRIX_DIM, MATRIX_DIM, srcA.data());
    arm_mat_init_f32(&matB, MATRIX_DIM, MATRIX_DIM, srcB.data());
    arm_mat_init_f32(&matDSP, MATRIX_DIM, MATRIX_DIM, dstDSP.data());

    std::cout << "开始运行 CMSIS-DSP 库计算..." << std::endl;
    auto startDSP = std::chrono::high_resolution_clock::now();

    // 调用 DSP 库矩阵乘法函数
    arm_status status = arm_mat_mult_f32(&matA, &matB, &matDSP);

    auto endDSP = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timeDSP = endDSP - startDSP;

    if (status != ARM_MATH_SUCCESS) {
        std::cerr << "DSP 矩阵乘法执行失败! 错误码: " << status << std::endl;
        return -1;
    }
    std::cout << "CMSIS-DSP 耗时: " << std::fixed << std::setprecision(3) << timeDSP.count() << " ms\n";

    // ==========================================
    // 结果分析与验证
    // ==========================================
    double speedup = timeNaive.count() / timeDSP.count();
    std::cout << "\n>>> 性能提升倍数: " << std::setprecision(2) << speedup << "x <<<\n";

    // 简单验证结果是否一致 (检查第一个元素，注意浮点误差)
    float32_t diff = std::abs(dstNaive[0] - dstDSP[0]);
    if (diff < 1e-3) {
        std::cout << "(验证通过：两种方法计算结果一致)\n";
    } else {
        std::cout << "(警告：结果存在较大偏差，可能是优化级别过激)\n";
    }

    return 0;
}