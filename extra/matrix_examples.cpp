#include <iostream>
#include "../src/Eigen/Dense"


int main(){
    // Eigen::MatrixXd m(2,2);
    // m(0,0) = 1;
    // m(1,0) = 0;
    // m(0,1) = 0;
    // m(1,1) = 1;
    
    // m = 3*m;

    // std::cout << m << std::endl;
    // std::cout << std::endl;
    

    // Eigen::MatrixXd n(2,2);
    // n(0,0) = 3;
    // n(1,0) = 2.5;
    // n(0,1) = -1;
    // n(1,1) = m(1,0) + m(0,1);
    // std::cout << n << std::endl;
    // std::cout << std::endl;


    // Eigen::MatrixXd out(2,2);
    // out = m*n;
    // std::cout << out << std::endl;
    // std::cout << std::endl;

    // std::cout << out(0,0) << std::endl;
    // std::cout << out.size() << std::endl;

    // Eigen::MatrixXd t(3,1);
    // t << 1,2,3;
    // std::cout << t.transpose() << std::endl;
    // std::cout << std::endl;

    // Eigen::MatrixXf A{      // State transition matrix
    // {1.0, 0.0, 1.0, 0.0},     
    // {1.0, 0.0, 1.0, 0.0},  
    // {1.0, 0.0, 1.0, 0.0},  
    // {1.0, 0.0, 1.0, 0.0}}; //Not working ...
    
    // Eigen::MatrixXd t(3,3);
    // t << 1,2,3,
    //      4,5,6,
    //      7,8,9 ;
    // std::cout << t.transpose() << std::endl;
    // std::cout << std::endl;
Eigen::Matrix4f A = [] {
    Eigen::Matrix4f tmp;
    tmp <<  1,0,1,0,
            0,1,0,1,
            0,0,1,0,
            0,0,0,1;
    return tmp;
}();

Eigen::Vector4f X = [] {
    Eigen::Vector4f tmp;
    tmp <<  0,0,1,2;
    return tmp;
}();

std::cout << A << std::endl;
std::cout << std::endl;

std::cout << X << std::endl;
std::cout << std::endl;

//(4x1) =   (4x4)*(4x1)   
     X  =     A  *  X;

std::cout << X << std::endl;
std::cout << std::endl;    

//(4x1) =   (4x4)*(4x1)   
     X  =     A  *  X;

std::cout << X << std::endl;
std::cout << std::endl;    
}

