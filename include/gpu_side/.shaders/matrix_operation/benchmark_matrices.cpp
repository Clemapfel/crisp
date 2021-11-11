// 
// Copyright 2021 Clemens Cords
// Created on 11.11.21 by clem (mail@clemens-cords.com)
//

#include <system/render_window.hpp>
#include <gpu_side/hardware_accelerated_matrix.hpp>
#include <benchmark.hpp>
#include <Dense>

using namespace crisp;

int main()
{
    auto window = RenderWindow();
    window.create(1, 1);
    window.set_active();

    Eigen::MatrixXf eigen_left, eigen_right;

    for (size_t size : {3, 5, 10, 50, 100, 500, 750, 1000, 2500, 5000})
    {
        eigen_left.resize(size, size);
        eigen_right.resize(size, size);
        eigen_left.setRandom();
        eigen_right.setRandom();

        auto gpu_left = HardwareAcceleratedMatrix(eigen_left);
        auto gpu_right = HardwareAcceleratedMatrix(eigen_right);

        // scalar add
        float scalar = 0;
        auto scalar_add_cpu = Benchmark([&](){
            eigen_left = (eigen_left.array() += scalar++).matrix();
        });

        auto scalar_add_gpu  = Benchmark([&](){
            gpu_left += scalar++;
        });

        // scalar product
        auto scalar_prod_cpu = Benchmark([&](){
            eigen_left = (eigen_left.array() *= scalar++).matrix();
        });

        auto scalar_prod_gpu  = Benchmark([&](){
            gpu_left *= scalar++;
        });

        // cwise add
        auto cwise_add_cpu = Benchmark([&](){
            eigen_left += eigen_right;
        });

        auto cwise_add_gpu  = Benchmark([&](){
            gpu_left += gpu_right;
        });

        // cwise multiply
        auto cwise_prod_cpu = Benchmark([&](){
            eigen_left = eigen_left.cwiseProduct(eigen_right);
        });

        auto cwise_prod_gpu  = Benchmark([&](){
            gpu_left *= gpu_right;
        });

        // matrix multiplication
        auto true_prod_cpu = Benchmark([&](){
            eigen_left *= eigen_right;
        });

        auto true_prod_gpu  = Benchmark([&](){
            gpu_left ^= gpu_right;
        });

        // transposing
        auto transpose_cpu = Benchmark([&](){
            eigen_left.transposeInPlace();
        });

        auto transpose_gpu  = Benchmark([&](){
            gpu_left.transpose_in_place();
        });

        // allocation
        auto allocate_new_cpu = Benchmark([&](){
            volatile auto temp = Eigen::MatrixXf(size, size);
        });

        auto allocate_new_gpu = Benchmark([&](){
            volatile auto temp = HardwareAcceleratedMatrix(size, size);
        });

        // copy
        auto copy_cpu = Benchmark([&](){
            volatile auto temp = eigen_left;
        });

        auto copy_gpu = Benchmark([&](){
            volatile auto temp = gpu_left;
        });
    }

}

