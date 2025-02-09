use std::f64;

const MAX_ITER: i32 = 10000;

fn f(x: [f64; 2]) -> [f64; 2] {
    [x[0].exp() - x[1] - 3., (x[0] + 6.).ln() + x[1] - 2.]
}

fn error_function(x: [f64; 2]) -> f64 {
    f(x)[0].powf(2.) + f(x)[1].powf(2.)
}

fn gradient_error_function(x: [f64; 2], step: f64) -> [f64; 2] {
    let res_f = f(x);

    let dedx1 = 2. * res_f[0] * (x[0].exp()) + 2. * res_f[1] * (1. / (x[0] + 6.));
    let dedx2 = 2. * res_f[0] * (-1.) + 2. * res_f[1] * (1.);

    [x[0] - step * dedx1, x[1] - step * dedx2]
}

fn gradient_descent_method(
    start_point: [f64; 2],
    step: f64,
    exponent: f64,
) -> Result<[f64; 2], &'static str> {
    let mut prev_vector_x = [start_point[0], start_point[1]];
    let mut vector_x = [start_point[0], start_point[1]];
    let mut i = 0;
    while i < MAX_ITER {
        vector_x = gradient_error_function(vector_x, step);
        // println!("{} {}", prev_vector_x[0], prev_vector_x[1]);
        // println!("{} {}\n", vector_x[0], vector_x[1]);
        if f64::max(
            f64::abs(vector_x[0] - prev_vector_x[0]),
            f64::abs(vector_x[1] - prev_vector_x[1]),
        ) < exponent
        {
            return Ok(vector_x);
        }

        prev_vector_x = vector_x;
        i += 1;
    }

    Err("Метод не сходится!")
}

fn main() {
    match gradient_descent_method([0., 0.], 0.001, 0.00001) {
        Ok(solution) => println!("Решение: {:?}", solution),
        Err(error) => println!("{}", error),
    };
}
