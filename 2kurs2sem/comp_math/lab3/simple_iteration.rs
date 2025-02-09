use std::f64;

const MAX_ITER: i32 = 1000;

fn f1(x: [f64; 2]) -> [f64; 2] {
    [(2. - x[1]).exp() - 6., x[0].exp() - 3.]
}

fn f2(x: [f64; 2]) -> [f64; 2] {
    [(3. - x[1]).ln(), 2. - (x[0] + 6.).ln()]
}

fn simple_iteration_method(
    f: fn([f64; 2]) -> [f64; 2],
    start_point: [f64; 2],
    exponent: f64,
) -> Result<[f64; 2], &'static str> {
    let mut prev_vector_x = [start_point[0], start_point[1]];
    let mut vector_x = [start_point[0], start_point[1]];
    let mut i = 0;
    while i < MAX_ITER {
        vector_x = f(vector_x);
        println!("{} {}", prev_vector_x[0], prev_vector_x[1]);
        println!("{} {}\n", vector_x[0], vector_x[1]);
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
    match simple_iteration_method(f2, [0., 0.], 0.001) {
        Ok(solution) => println!("Решение: {:?}", solution),
        Err(error) => println!("{}", error),
    };
}
