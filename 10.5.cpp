#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

// 表示三维空间中的点或向量。
struct Vec3 {
    double x, y, z;
};

// 生成单位球面上的均匀随机点：先取三个独立标准正态随机数，再归一化方向。
Vec3 randomPointOnSphere(std::mt19937_64& rng,
                         std::normal_distribution<double>& normal) {
    // 各坐标独立且服从标准正态分布，所得向量的方向在球面上均匀分布。
    double x = normal(rng);
    double y = normal(rng);
    double z = normal(rng);
    // 将随机向量除以其长度，缩放到单位球面。
    double length = std::sqrt(x * x + y * y + z * z);
    return {x / length, y / length, z / length};
}

// 判断由四个顶点构成的四面体是否包含原点：求原点的重心坐标并检查其是否非负。
bool containsOrigin(const Vec3 p[4]) {
    // 解：
    // x*(p0-p3) + y*(p1-p3) + z*(p2-p3) = -p3
    double a[3][4] = {
        {p[0].x - p[3].x, p[1].x - p[3].x, p[2].x - p[3].x, -p[3].x},
        {p[0].y - p[3].y, p[1].y - p[3].y, p[2].y - p[3].y, -p[3].y},
        {p[0].z - p[3].z, p[1].z - p[3].z, p[2].z - p[3].z, -p[3].z}
    };

    // 用带部分主元的高斯消元求解线性方程组，避免较小主元导致数值不稳定。
    for (int col = 0; col < 3; ++col) {
        int pivot = col;
        for (int row = col + 1; row < 3; ++row) {
            if (std::abs(a[row][col]) > std::abs(a[pivot][col])) {
                pivot = row;
            }
        }

        // 主元过小表示矩阵近似奇异，四面体退化，无法可靠求出重心坐标。
        if (std::abs(a[pivot][col]) < 1e-12) {
            return false; // 四面体退化，概率为零的情况
        }

        for (int j = col; j < 4; ++j) {
            std::swap(a[col][j], a[pivot][j]);
        }

        double divisor = a[col][col];
        for (int j = col; j < 4; ++j) {
            a[col][j] /= divisor;
        }

        for (int row = 0; row < 3; ++row) {
            if (row == col) continue;
            double factor = a[row][col];
            for (int j = col; j < 4; ++j) {
                a[row][j] -= factor * a[col][j];
            }
        }
    }

    // 前三个重心坐标由方程组求得，第四个由坐标和为 1 得出。
    double x = a[0][3];
    double y = a[1][3];
    double z = a[2][3];
    double w = 1.0 - x - y - z;

    // 使用容差吸收浮点误差；四个重心坐标均非负时，原点位于四面体内或边界上。
    constexpr double eps = 1e-10;
    return x >= -eps && y >= -eps && z >= -eps && w >= -eps;
}

// 通过蒙特卡洛模拟估计四个球面随机点构成的四面体包含原点的概率，并与理论值比较。
int main() {
    // 模拟次数越多，估计结果通常越接近真实概率。
    constexpr int trials = 1'000'000;

    // 初始化随机数引擎以及生成标准正态随机数的分布。
    std::mt19937_64 rng(std::random_device{}());
    std::normal_distribution<double> normal(0.0, 1.0);

    // 统计包含原点的试验次数。
    int count = 0;
    for (int i = 0; i < trials; ++i) {
        // 每次试验独立生成四个单位球面上的随机顶点。
        Vec3 points[4];
        for (Vec3& point : points) {
            point = randomPointOnSphere(rng, normal);
        }
        if (containsOrigin(points)) {
            ++count;
        }
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

    // 用成功次数除以总试验次数得到概率估计，并输出理论概率供对照。
    std::cout << "模拟概率: "
              << std::fixed << std::setprecision(6)
              << static_cast<double>(count) / trials << '\n';
    std::cout << "理论概率: 1/8 = 0.125000\n";
    return 0;
}//AI还是太强了