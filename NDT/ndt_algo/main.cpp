#include <pcl/io/pcd_io.h>
#include <pcl/registration/ndt.h>
#include <pcl/filters/voxel_grid.h>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: ./NDT map.pcd frame.pcd init_guess.txt" << std::endl;
        return -1;
    }

    std::string map_path = argv[1];
    std::string frame_path = argv[2];
    std::string init_path = argv[3];

    // Load the map and scan point clouds
    pcl::PointCloud<pcl::PointXYZ>::Ptr map(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr scan(new pcl::PointCloud<pcl::PointXYZ>());

    if (pcl::io::loadPCDFile(map_path, *map) == -1 || pcl::io::loadPCDFile(frame_path, *scan) == -1) {
        std::cerr << "Failed to load input PCD files." << std::endl;
        return -1;
    }

    // Apply voxel grid downsampling to reduce point cloud density
    pcl::VoxelGrid<pcl::PointXYZ> voxel;
    voxel.setLeafSize(0.5f, 0.5f, 0.5f);

    voxel.setInputCloud(map);
    voxel.filter(*map);

    voxel.setInputCloud(scan);
    voxel.filter(*scan);

    // Read the initial guess transformation matrix from file
    Eigen::Matrix4f init_guess;
    std::ifstream file(init_path);
    if (!file) {
        std::cerr << "Cannot read init guess file." << std::endl;
        return -1;
    }

    // Load 4x4 matrix values into Eigen matrix
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            file >> init_guess(i, j);

    // Set up the NDT registration object
    pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;
    ndt.setResolution(1.0);
    ndt.setInputTarget(map);
    ndt.setInputSource(scan);
    ndt.setMaximumIterations(50);

    // Perform the alignment using the initial guess
    pcl::PointCloud<pcl::PointXYZ>::Ptr output(new pcl::PointCloud<pcl::PointXYZ>());
    ndt.align(*output, init_guess);

    // Get the resulting transformation from scan to map
    Eigen::Matrix4f final = ndt.getFinalTransformation();

    // Print the final transformation matrix to stdout (this is read by the Python part)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            std::cout << final(i, j) << ' ';
        std::cout << std::endl;
    }

    return 0;
}
