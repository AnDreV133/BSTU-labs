fn f(x: f64)->f64 {
    f64::ln(x.powf(2.)+1.)+x*f64::exp(-0.05*x)
}

fn dfdx(x: f64)->f64 {
    2.*x/(x.powf(2.)+1.)-0.05*x*f64::exp(-0.05*x)+f64::exp(-0.05*x)
}

fn dfdx_1(x: f64, h: f64)->f64 {
    (f(x+h)-f(x))/h
}

fn dfdx_2(x: f64, h: f64)->f64 {
    (f(x+h)-f(x-h))/(2.*h)
}

fn dfdx_4(x: f64, h: f64)->f64 {
    (f(x-2.*h)-8.*f(x-h)+8.*f(x+h)-f(x+2.*h))/(12.*h)
}

fn main() {
    let x:f64 = 5.;
    let h:f64 = 0.001;
    println!("res orig: {}", dfdx(x));
    println!("res 1ord: {}", dfdx_1(x, h));
    println!("res 2ord: {}", dfdx_2(x, h));
    println!("res 4ord: {}", dfdx_4(x, h));
    while true {}
}
