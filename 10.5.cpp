
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

struct Vec3 {
    double x, y, z;
};

Vec3 randomPointOnSphere(std::mt19937_64& rng,
                         std::normal_distribution<double>& normal) {
    double x = normal(rng);
    double y = normal(rng);
    double z = normal(rng);
    double length = std::sqrt(x * x + y * y + z * z);
    return {x / length, y / length, z / length};
}

// 判断原点是否在四面体内部：求原点的重心坐标
bool containsOrigin(const Vec3 p[4]) {
    // 解：
    // x*(p0-p3) + y*(p1-p3) + z*(p2-p3) = -p3
    double a[3][4] = {
        {p[0].x - p[3].x, p[1].x - p[3].x, p[2].x - p[3].x, -p[3].x},
        {p[0].y - p[3].y, p[1].y - p[3].y, p[2].y - p[3].y, -p[3].y},
        {p[0].z - p[3].z, p[1].z - p[3].z, p[2].z - p[3].z, -p[3].z}
    };

    // 高斯消元（带部分主元）
    for (int col = 0; col < 3; ++col) {
        int pivot = col;
        for (int row = col + 1; row < 3; ++row) {
            if (std::abs(a[row][col]) > std::abs(a[pivot][col])) {
                pivot = row;
            }
        }

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

    double x = a[0][3];
    double y = a[1][3];
    double z = a[2][3];
    double w = 1.0 - x - y - z;

    constexpr double eps = 1e-10;
    return x >= -eps && y >= -eps && z >= -eps && w >= -eps;
}

int main() {
    constexpr int trials = 1'000'000;

    std::mt19937_64 rng(std::random_device{}());
    std::normal_distribution<double> normal(0.0, 1.0);

    int count = 0;
    for (int i = 0; i < trials; ++i) {
        Vec3 points[4];
        for (Vec3& point : points) {
            point = randomPointOnSphere(rng, normal);
        }
        if (containsOrigin(points)) {
            ++count;
        }
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

    std::cout << "模拟概率: "
              << std::fixed << std::setprecision(6)
              << static_cast<double>(count) / trials << '\n';
    std::cout << "理论概率: 1/8 = 0.125000\n";
    return 0;
}//AI还是太强了