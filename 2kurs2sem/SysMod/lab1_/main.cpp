// #include <iostream>
// #include <cmath>

// const double PI = 3.14159;
// const double STEP = PI / 100;

// int k1 = 20000;
// int k2 = 10000;
// int k3 = 40000;
// double n1 = 1;
// double n2 = 0.5;
// double n3 = 0.7;
// int m = 10;

// double alpha = 0;
// double beta = 0;
// double gama = 0;

// double x() {
//     return (n3 * tan(alpha) - n1) / (tan(PI / 2 - alpha) - tan(gama));
// }

// double y() {
//     return (-tan(gama) * (n3 * tan(alpha) - n1)) / (tan(PI / 2 - alpha) - tan(gama));
// }

// double delta1() {
//     return sqrt(pow(x() - n1, 2) + pow(y(), 2)) - n1;
// }

// double delta2() {
//     return sqrt(pow(x(), 2) + pow(y() + n2, 2)) - n2;
// }

// double delta3() {
//     return sqrt(pow(x(), 2) + pow(y() - n3, 2)) - n3;
// }

// double func1() {
//     return k2 * delta2() * sin(beta) + k3 * delta3() * sin(alpha) - k1 * delta1() * cos(gama);
// }

// double func2() {
//     return m * 9.8 + k2 * delta2() * cos(beta) + k1 * delta1() * sin(gama) - k3 * delta3() * sin(alpha);
// }

// int main() {
//     while (alpha < PI * 2) {
//         while (beta < PI * 2) {
//             while (gama < PI * 2) {
//                 try {
//                     if (std::abs(func1()) < 0.1 && std::abs(func2()) < 0.1) {
//                         std::cout << alpha << ' ' << beta << ' ' << gama << " res -> " << func1() << ' ' << func2() << '\n';
//                     }
//                 } catch (...) {
//                     // Handle the exception if needed
//                 }

//                 gama += STEP;
//             }
//             beta += STEP;
//             gama = 0;
//         }
//         alpha += STEP;
//         beta = 0;
//         gama = 0;
//     }

//     return 0;
// }

// #include <iostream>
// #include <cmath>
// #include <fstream>

// const double PI = 3.14159;
// const double STEP = 0.003;

// int k1 = 20000;
// int k2 = 10000;
// int k3 = 40000;
// double n1 = 1;
// double n2 = 0.5;
// double n3 = 0.7;
// int m = 10;

// // double alpha = 0;

// double x = 0;
// double y = 0;

// double beta()
// {
//     return atan(x / n2);
// }
// double gama()
// {
//     return atan(y / n1);
// }

// double x()
// {
//     return (n2 - tan(beta()) - n1) / (tan(gama()) * tan(beta()) + 1);
// }

// double y()
// {
//     return (n1 * tan(beta()) * tan(gama()) - n2 * tan(gama())) / (tan(beta()) * tan(gama()) + 1) + n1;
// }

// double delta1()
// {
//     return sqrt(pow(x() - n1, 2) + pow(y(), 2)) - n1;
// }

// double delta2()
// {
//     return sqrt(pow(x(), 2) + pow(y() - n2, 2)) - n2;
// }

// double delta3()
// {
//     return sqrt(pow(x(), 2) + pow(y() + n3, 2)) - n3;
// }

// double alpha()
// {
//     return acos((pow(n2 + n3, 2) + pow(n3 + delta3(), 2) - pow(n2 + delta2(), 2)) / (2 * (n2 + n3) * (n3 + delta3())));
// }

// double func1()
// {
//     return k2 * delta2() * sin(beta()) + k3 * delta3() * sin(alpha()) + k1 * delta1() * cos(gama());
// }

// double func2()
// {
//     return m * 9.8 + k3 * delta3() * cos(alpha()) + k1 * delta1() * sin(gama()) + k2 * delta2() * cos(beta());
// }

// int main()
// {
//     std::ofstream fout;
//     fout.open("res.txt");

//     double temp_func1;
//     double temp_func2;
//     // while (alpha() < PI * 2) {
//     while (x < 1)
//     {
//         while (y < 1)
//         {
//             try
//             {
//                 // if (std::abs(func1()) < 0.1 && std::abs(func2()) < 0.1) {
//                 if (func1() < temp_func1)
//                 {
//                     fout << x << ' ' << y << " res -> " << func1() << ' ' << func2() << '\n';
//                 }
//                 temp_func1 = func1();
//                 temp_func2 = func2();
//             }
//             catch (...)
//             {
//                 std::cerr << x << ' ' << y << " - gen err";
//             }

//             y += STEP;
//         }
//         x += STEP;
//         y = 0;

//         temp_func1 = 0;
//         temp_func2 = 0;
//     }
//     // alpha() += STEP;
//     // beta = 0;
//     // gama = 0;
//     // }

//     fout.close();

//     return 0;
// }

#include <iostream>
#include <cmath>
#include <fstream>

const double PI = 3.14159;
const double STEP = 0.001;

int k1 = 20000;
int k2 = 10000;
int k3 = 40000;
double n1 = 1;
double n2 = 0.5;
double n3 = 0.7;
int m = 10;

// double alpha = 0;

double x = -1; // 2.931
double y = -1;

double beta()
{
    return atan(x / n2);
}
double gama()
{
    return atan(y / n1);
}

double delta1()
{
    return sqrt(pow(x - n1, 2) + pow(y, 2)) - n1;
}

double delta2()
{
    return sqrt(pow(x, 2) + pow(y - n2, 2)) - n2;
}

double delta3()
{
    return sqrt(pow(x, 2) + pow(y + n3, 2)) - n3;
}

double alpha()
{
    auto temp = (pow(n2 + n3, 2) + pow(n3 + delta3(), 2) - pow(n2 + delta2(), 2)) / (2 * (n2 + n3) * (n3 + delta3()));
    return acos(abs(temp) > 1.0 && abs(temp) < 1.00001 ? 1 : temp);
}

double func1()
{
    return k2 * delta2() * sin(beta()) + k3 * delta3() * sin(alpha()) + k1 * delta1() * cos(gama());
}

double func2()
{
    return m * 9.8 + k3 * delta3() * cos(alpha()) + k1 * delta1() * sin(gama()) + k2 * delta2() * cos(beta());
}

int main()
{
    std::ofstream fout;
    fout.open("res.txt");

    double temp_func1 = 10000000000000000;
    double temp_func2 = 10000000000000000;
    while (x < 0)
    {
        while (y < 0)
        {
            try
            {
                // if (func1() < temp_func1)
                {
                fout << x << ' ' << y << " res -> " << func1() << ' ' << func2() << '\n';
                }
                temp_func1 = func1();
                temp_func2 = func2();
            }
            catch (...)
            {
                std::cerr << x << ' ' << y << " - gen err";
            }

            y += STEP;
        }
        x += STEP;
        y = -1;

        temp_func1 = 10000000000000000;
        temp_func2 = 10000000000000000;
    }

    fout.close();

    return 0;
}
