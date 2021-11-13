// 
// Copyright 2021 Clemens Cords
// Created on 11.11.21 by clem (mail@clemens-cords.com)
//

#include <system/render_window.hpp>
#include <gpu_side/hardware_accelerated_matrix.hpp>
#include <benchmark.hpp>
#include <Dense>
#include <fstream>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/core/cuda.hpp>

#include <frequency_domain_filter.hpp>

int main()
{
    using namespace crisp;

    auto window = RenderWindow();
    window.create(1, 1);
    window.set_active();

    std::string path = "/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/matrix_multiplication.csv";

    std::fstream file(path, file.binary | file.trunc | file.in | file.out);
    assert(file.is_open());

    Eigen::MatrixXf eigen_left, eigen_right;
    size_t n_cycles = 15;

    cv::Mat_<float> cv_left, cv_right;
    cv::cuda::GpuMat cvgpu_left, cvgpu_right;

    file << "# n cycles per benchmark: " << n_cycles << std::endl;
    //file << "size,scalar_add_eigen,scalar_add_crisp,scalar_prod_eigen,scalar_prod_crisp,cwise_add_eigen,cwise_add_crisp,cwise_prod_eigen,cwise_prod_crisp,true_prod_eigen,true_prod_crisp,transpose_eigen,transpose_crisp,allocate_new_eigen,allocate_new_crisp,copy_eigen,copy_crisp" << std::endl;
    file << "size,eigen,crisp,cv,cv_gpu" << std::endl;

    std::vector<size_t> sizes;

    for (size_t size = 1; size < 150; size += 1)
        sizes.push_back(size);


    for (size_t size = 150; size < 500; size += 50)
        sizes.push_back(size);

    for (auto size : sizes)
    {
        std::cout << "Benchmarking n = " << size << " ..." << std::endl;

        eigen_left.resize(size, size);
        eigen_right.resize(size, size);
        eigen_left.setRandom();
        eigen_right.setRandom();

        cv_left.create(size, size);
        cv_right.create(size, size);

        cvgpu_left.upload(cv_left);
        cvgpu_right.upload(cv_right);

        for (size_t i = 0; i < eigen_left.rows() * eigen_left.cols(); ++i)
        {
            cv_left.data[i] = eigen_left.data()[i];
            cv_right.data[i] = eigen_right.data()[i];
        }

        auto gpu_left = HardwareAcceleratedMatrix(eigen_left);
        auto gpu_right = HardwareAcceleratedMatrix(eigen_right);

        file << size << "," << std::flush;

        auto true_prod_eigen = Benchmark([&](){
            eigen_left *= eigen_right;
        });

        file << true_prod_eigen.execute(n_cycles/2) << "," << std::flush;

        auto true_prod_crisp  = Benchmark([&](){
            gpu_left ^= gpu_right;
        });

        file << true_prod_crisp.execute(n_cycles) << "," << std::flush;

        auto true_prod_cv = Benchmark([&](){
            cv_left = cv_left * cv_right;
        });

        file << true_prod_cv.execute(n_cycles) << "," << std::flush;

        auto true_prod_cv_gpu = Benchmark([&](){
            //cv::cuda::multiply(cvgpu_left, cv_gpu_right, cvgpu_left);
        });

        file << true_prod_cv_gpu.execute(n_cycles) << std::endl;

        /*
        // scalar add
        float scalar = 0;
        auto scalar_add_eigen = Benchmark([&](){
            eigen_left = (eigen_left.array() += scalar++).matrix();
        });

        auto scalar_add_crisp  = Benchmark([&](){
            gpu_left += scalar++;
        });

        file << scalar_add_crisp.execute(n_cycles) << "," << std::flush;

        // scalar product
        auto scalar_prod_eigen = Benchmark([&](){
            eigen_left = (eigen_left.array() *= scalar++).matrix();
        });

        file << scalar_prod_eigen.execute(n_cycles/2) << "," << std::flush;

        auto scalar_prod_crisp  = Benchmark([&](){
            gpu_left *= scalar++;
        });

        file << scalar_prod_crisp.execute(n_cycles) << "," << std::flush;

        // cwise add
        auto cwise_add_eigen = Benchmark([&](){
            eigen_left += eigen_right;
        });

        file << cwise_add_eigen.execute(n_cycles/2) << "," << std::flush;

        auto cwise_add_crisp  = Benchmark([&](){
            gpu_left += gpu_right;
        });

        file << cwise_add_crisp.execute(n_cycles) << "," << std::flush;

        // cwise multiply
        auto cwise_prod_eigen = Benchmark([&](){
            eigen_left = eigen_left.cwiseProduct(eigen_right);
        });

        file << cwise_prod_eigen.execute(n_cycles/2) << "," << std::flush;

        auto cwise_prod_crisp  = Benchmark([&](){
            gpu_left *= gpu_right;
        });

        file << cwise_prod_crisp.execute(n_cycles) << "," << std::flush;

        // matrix multiplication
        auto true_prod_eigen = Benchmark([&](){
            eigen_left *= eigen_right;
        });

        file << true_prod_eigen.execute(n_cycles/2) << "," << std::flush;

        auto true_prod_crisp  = Benchmark([&](){
            gpu_left ^= gpu_right;
        });

        file << true_prod_crisp.execute(n_cycles) << "," << std::flush;

        // transposing
        auto transpose_eigen = Benchmark([&](){
            eigen_left.transposeInPlace();
        });

        file << transpose_eigen.execute(n_cycles/2) << "," << std::flush;

        auto transpose_crisp  = Benchmark([&](){
            gpu_left.transpose_in_place();
        });

        file << transpose_crisp.execute(n_cycles) << "," << std::flush;

        // allocation
        auto allocate_new_eigen = Benchmark([&](){
            volatile auto temp = Eigen::MatrixXf(size, size);
        });

        file << allocate_new_eigen.execute(n_cycles/2) << "," << std::flush;

        auto allocate_new_crisp = Benchmark([&](){
            volatile auto temp = HardwareAcceleratedMatrix(size, size);
        });

        file << allocate_new_crisp.execute(n_cycles) << "," << std::flush;

        // copy
        auto copy_eigen = Benchmark([&](){
            volatile auto temp = eigen_left;
        });

        file << copy_eigen.execute(n_cycles/2) << "," << std::flush;

        auto copy_crisp = Benchmark([&](){
            volatile auto temp = gpu_left;
        });

        file << copy_crisp.execute(n_cycles) << std::endl;
         */
    }

    std::cout << "completed." << std::endl;
    file.close();
}

