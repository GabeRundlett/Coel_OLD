#include "Shaders.hpp"
#include <Coel.hpp>
#include <iostream>
#include <thread>

Coel::Window window(900, 900, "Window");
Coel::Renderer::Batch2d renderer;
Coel::Shader shader(vertSrc, fragSrc);

const double RES = (double)window.size.x / 1, SCALE = 5;

constexpr long double values[]{
    1.0e0,
    1.0e0,
    5.0e-1,
    1.6666666666666666e-1,
    4.1666666666666666e-1,
    8.3333333333333333e-2,
    1.3888888888888888e-2,
    1.9841269841269841e-3,
    2.4801587301587302e-5,
    2.7557319223985891e-6,
    2.7557319223985891e-7,
    2.5052108385441719e-08,
    2.0876756987868099e-09,
    1.6059043836821615e-10,
    1.1470745597729725e-11,
    7.6471637318198165e-13,
    4.7794773323873853e-14,
    2.8114572543455208e-15,
    1.5619206968586226e-16,
    8.2206352466243297e-18,
    4.1103176233121649e-19,
    1.9572941063391261e-20,
    8.8967913924505733e-22,
    3.8681701706306840e-23,
};

inline long double trigSeries(long double x) {
    // x = Math::modPi<long double>(x);
    auto x2 = Math::sq(x);

    auto result = 0.0L, m = 1.0L;
    for (unsigned int i = 0; i < 23 / 2 - 2; ++i, m *= -x2) {
        auto index = i * 2;
        auto fac = values[index];
        result += m * fac;

        // /*
        std::cout << "Index = ";
        std::cout.width(5);
        std::cout << index;
        std::cout << ", M = ";
        std::cout.width(5);
        std::cout << m;
        std::cout << ", Fac = ";
        std::cout.width(15);
        std::cout << fac;
        std::cout << ", result = ";
        std::cout.width(15);
        std::cout << result;
        std::cout << '\n';
        // */
    }
    return result;
}

static inline auto cosine(long double x) {
    return trigSeries(x);
    if (x < 0) x *= -1;
    if (Math::modTau<long double>(x) > Math::PI) {
        if (Math::modPi<long double>(x) > Math::HALF_PI) return trigSeries(x);
        return trigSeries(-x);
    } else if (Math::modPi<long double>(x) > Math::HALF_PI)
        return -trigSeries(x);
    if (x == 0) return static_cast<long double>(1);
    return -trigSeries(-x);
}

int main() {
    window.onResize = [](Coel::Window &w) {
        Coel::Renderer::Command::resizeViewport(0, 0, w.size.x, w.size.y); //
    };
    window.onResize(window);
    shader.bind();

    // unsigned int i = 3;
    // std::cout.precision(17);
    // long double val = 1;
    // for (auto i = 0; i < 23; ++i) {
    //     std::cout << values[i] <<": " << val << '\n';
    //     val /= i + 1;
    // }

    trigSeries(1.f);

    // for (auto &a : results) {
    //     std::cout << i << ": " << a << "\n";
    //     i += 2;
    // }

    // window.close();
    // window.update();
    //
    // return 0;

    // auto a = trigSeries(1);
    // std::cout << "result: " << a << '\n';
    // return 0;

    auto drawFunc = [](long double (*f)(long double), long double off = 0) {
        for (unsigned int i = 0; i < RES; ++i) {
            long double x = (long double)i / RES * 2 - 1;
            renderer.submitRect((float)x + (float)off, 0, 1.f / (float)RES,
                                (float)f(long double(x + off) * SCALE) / (float)SCALE);
        }
    };

    window.close();

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();
        renderer.fill(0xff0000ff);
        drawFunc(std::sin);
        renderer.fill(0x0000ffff);
        drawFunc(cosine, 1.f / RES);
        renderer.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        window.update();
    }
}
