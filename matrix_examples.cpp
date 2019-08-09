#include <iostream>
#include "Eigen/Dense"


int main(){
    Eigen::MatrixXd m(2,2);
    m(0,0) = 1;
    m(1,0) = 0;
    m(0,1) = 0;
    m(1,1) = 1;
    
    m = 3*m;

    std::cout << m << std::endl;
    std::cout << std::endl;
    

    Eigen::MatrixXd n(2,2);
    n(0,0) = 3;
    n(1,0) = 2.5;
    n(0,1) = -1;
    n(1,1) = m(1,0) + m(0,1);
    std::cout << n << std::endl;
    std::cout << std::endl;


    Eigen::MatrixXd out(2,2);
    out = m*n;
    std::cout << out << std::endl;
    std::cout << std::endl;

    Eigen::MatrixXd t(3,1);
    t << 1,2,3;
    std::cout << t.transpose() << std::endl;
    std::cout << std::endl;
}

