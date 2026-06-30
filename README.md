# dsp-template

一个基于 [CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) 的工程模板，演示如何在 ARM 平台上使用 CMSIS-DSP 库进行高性能数值计算。

示例程序以 **1024 × 1024 单精度浮点矩阵乘法** 为基准，对比「传统三重 `for` 循环」与「CMSIS-DSP 库函数（`arm_mat_mult_f32`）」两种实现的耗时，并计算性能提升倍数，直观展示 NEON SIMD 加速的效果。

## 功能特性

- 🚀 通过 CMake `FetchContent` 自动拉取并编译 CMSIS-DSP（v1.17.0），无需手动管理依赖
- 🧮 内置朴素矩阵乘法与 CMSIS-DSP 加速矩阵乘法两种实现，作为性能对照
- ⏱️ 使用 `std::chrono::high_resolution_clock` 高精度计时，输出毫秒级耗时与加速比
- ✅ 自动验证两种方法的计算结果一致性（容忍浮点误差）
- ⚙️ 默认启用 NEON SIMD 与 AArch64 优化，并开启 `-O3 -ffast-math -mcpu=native` 编译优化

## 项目结构

```
dsp-template/
├── CMakeLists.txt   # 构建配置，自动拉取 CMSIS-DSP
├── main.cpp         # 主程序：矩阵乘法性能对比基准
├── LICENSE          # MIT 开源协议
├── .gitignore
└── README.md
```

## 环境要求

- **硬件**：ARM 处理器（支持 NEON）。模板默认面向 64 位 AArch64 架构。
- **CMake** ≥ 4.2
- **C++ 编译器**：支持 C++14（GCC / Clang）
- **Git**（CMake `FetchContent` 拉取 CMSIS-DSP 源码时需要）

> 提示：`-mcpu=native` 会按当前机器的 CPU 进行优化，因此请在目标 ARM 设备上直接编译；若要交叉编译，请将该选项替换为目标 CPU 型号。

## 构建与运行

```bash
# 配置（首次会自动下载 CMSIS-DSP 源码，需要联网）
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build build

# 运行
./build/dsp_template
```

### 输出示例

```
初始化矩阵数据 (1024x1024)...

开始运行传统 for 循环计算...
for 循环耗时: 3250.123 ms
开始运行 CMSIS-DSP 库计算...
CMSIS-DSP 耗时: 180.456 ms

>>> 性能提升倍数: 18.01x <<<
(验证通过：两种方法计算结果一致)
```

> 实际数值取决于具体的 CPU 型号、主频与编译选项，仅供参考。

## 配置说明

可在 `CMakeLists.txt` 中按需调整以下选项：

| 配置项 | 说明 |
| --- | --- |
| `ARM_MATH_NEON` | 启用 NEON SIMD 加速 |
| `ARM_MATH_AARCH64` | 面向 64 位 AArch64 架构；**若使用 32 位处理器，请注释掉此行** |
| `OPTIMIZATION_FLAGS` | 编译优化选项（`-O3 -ffast-math -mcpu=native`） |
| `GIT_TAG` | 所使用的 CMSIS-DSP 版本，默认 `v1.17.0` |

矩阵维度可在 `main.cpp` 中通过 `MATRIX_DIM` 常量修改。

## 许可证

本项目采用 [MIT 许可证](./LICENSE) 开源，详见 `LICENSE` 文件。

CMSIS-DSP 为 ARM 公司所有，遵循其自身的 [Apache-2.0 许可证](https://github.com/ARM-software/CMSIS-DSP/blob/main/LICENSE)。
