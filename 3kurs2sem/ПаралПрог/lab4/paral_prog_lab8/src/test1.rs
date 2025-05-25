use ocl::flags::MEM_READ_WRITE;
use ocl::{Buffer, Device, Kernel, Platform, Program, Queue};
use rand::Rng;

const INPUT_SIZE: usize = 5;
const LEARNING_RATE: f32 = 0.1;
const EPOCHS: usize = 3001;

struct NeuralNetwork {
    w_input_hidden: Vec<f32>,
    w_hidden_output: f32,
    bias_hidden: f32,
    bias_output: f32,
    queue: Queue,
    forward_kernel: Kernel,
    backward_kernel: Kernel,
}

impl NeuralNetwork {
    fn new() -> Self {
        let mut rng = rand::thread_rng();

        // Initialize weights
        let w_input_hidden: Vec<f32> = (0..INPUT_SIZE).map(|_| rng.gen_range(-1.0..1.0)).collect();
        let w_hidden_output = rng.gen_range(-1.0..1.0);
        let bias_hidden = rng.gen_range(-1.0..1.0);
        let bias_output = rng.gen_range(-1.0..1.0);

        // Initialize OpenCL
        let platform = Platform::first().expect("No OpenCL platform found");
        let device = Device::first(platform).expect("No OpenCL device found");
        let context = ocl::Context::builder()
            .platform(platform)
            .devices(device)
            .build()
            .expect("Failed to create OpenCL context");
        let queue = Queue::new(&context, device, None).expect("Failed to create OpenCL queue");

        // Compile OpenCL program
        let program = Program::builder()
            .src(include_str!("kernels.cl"))
            .devices(device)
            .build(&context)
            .expect("Failed to build OpenCL program");

        // Create kernels
        let forward_kernel = Kernel::builder()
            .program(&program)
            .name("forward_pass")
            .queue(queue.clone())
            .global_work_size(1) // One work item per sample
            .arg(None::<&Buffer<f32>>) // input
            .arg(None::<&Buffer<f32>>) // weights
            .arg(None::<&Buffer<f32>>) // hidden_input
            .arg(0.0f32) // bias_hidden
            .build()
            .expect("Failed to create forward kernel");

        let backward_kernel = Kernel::builder()
            .program(&program)
            .name("backward_pass")
            .queue(queue.clone())
            .global_work_size(1) // One work item per sample
            .arg(None::<&Buffer<f32>>) // input
            .arg(None::<&Buffer<f32>>) // weights
            .arg(0.0f32) // d_hidden_output
            .arg(0.0f32) // learning_rate
            .build()
            .expect("Failed to create backward kernel");

        NeuralNetwork {
            w_input_hidden,
            w_hidden_output,
            bias_hidden,
            bias_output,
            queue,
            forward_kernel,
            backward_kernel,
        }
    }

    fn sigmoid(&self, x: f32) -> f32 {
        1.0 / (1.0 + (-x).exp())
    }

    fn sigmoid_derivative(&self, x: f32) -> f32 {
        let s = self.sigmoid(x);
        s * (1.0 - s)
    }

    fn forward_pass(&self, x: &[f32]) -> (f32, f32, f32, f32) {
        // Create buffers
        let x_buffer = Buffer::<f32>::builder()
            .queue(self.queue.clone())
            .flags(MEM_READ_WRITE)
            .len(INPUT_SIZE)
            .copy_host_slice(x)
            .build()
            .expect("Failed to create input buffer");

        let weights_buffer = Buffer::<f32>::builder()
            .queue(self.queue.clone())
            .flags(MEM_READ_WRITE)
            .len(INPUT_SIZE)
            .copy_host_slice(&self.w_input_hidden)
            .build()
            .expect("Failed to create weights buffer");

        let hidden_input_buffer = Buffer::<f32>::builder()
            .queue(self.queue.clone())
            .flags(MEM_READ_WRITE)
            .len(1) // Single hidden input value
            .build()
            .expect("Failed to create hidden input buffer");

        // Set kernel arguments
        self.forward_kernel
            .set_arg(0, &x_buffer)
            .expect("Failed to set input buffer");
        self.forward_kernel
            .set_arg(1, &weights_buffer)
            .expect("Failed to set weights buffer");
        self.forward_kernel
            .set_arg(2, &hidden_input_buffer)
            .expect("Failed to set hidden input buffer");
        self.forward_kernel
            .set_arg(3, self.bias_hidden)
            .expect("Failed to set bias_hidden");

        // Execute kernel
        unsafe {
            self.forward_kernel.enq().expect("Failed to enqueue forward kernel");
        }

        // Read hidden input
        let mut hidden_input = vec![0.0; 1];
        hidden_input_buffer
            .read(&mut hidden_input)
            .enq()
            .expect("Failed to read hidden input");

        let hidden_input = hidden_input[0];
        let hidden_output = self.sigmoid(hidden_input);
        let final_input = hidden_output * self.w_hidden_output + self.bias_output;
        let y_pred = self.sigmoid(final_input);

        (hidden_input, hidden_output, final_input, y_pred)
    }

    fn train(&mut self, X: &[Vec<f32>], Y: &[f32]) {
        for epoch in 0..EPOCHS {
            let mut total_error = 0.0;

            for (x, &y_true) in X.iter().zip(Y) {
                let (hidden_input, hidden_output, final_input, y_pred) = self.forward_pass(x);

                let error = y_true - y_pred;
                total_error += error.powi(2);

                let d_y_pred = error * self.sigmoid_derivative(final_input);
                let d_hidden_output =
                    d_y_pred * self.w_hidden_output * self.sigmoid_derivative(hidden_input);

                // Update weights with OpenCL
                let x_buffer = Buffer::<f32>::builder()
                    .queue(self.queue.clone())
                    .flags(MEM_READ_WRITE)
                    .len(INPUT_SIZE)
                    .copy_host_slice(x)
                    .build()
                    .expect("Failed to create input buffer");

                let weights_buffer = Buffer::<f32>::builder()
                    .queue(self.queue.clone())
                    .flags(MEM_READ_WRITE)
                    .len(INPUT_SIZE)
                    .copy_host_slice(&self.w_input_hidden)
                    .build()
                    .expect("Failed to create weights buffer");

                self.backward_kernel
                    .set_arg(0, &x_buffer)
                    .expect("Failed to set input buffer");
                self.backward_kernel
                    .set_arg(1, &weights_buffer)
                    .expect("Failed to set weights buffer");
                self.backward_kernel
                    .set_arg(2, d_hidden_output)
                    .expect("Failed to set d_hidden_output");
                self.backward_kernel
                    .set_arg(3, LEARNING_RATE)
                    .expect("Failed to set learning_rate");

                unsafe {
                    self.backward_kernel
                        .enq()
                        .expect("Failed to enqueue backward kernel");
                }

                // Read updated weights
                weights_buffer
                    .read(&mut self.w_input_hidden)
                    .enq()
                    .expect("Failed to read updated weights");

                // Update remaining weights and biases
                self.w_hidden_output += LEARNING_RATE * hidden_output * d_y_pred;
                self.bias_hidden += LEARNING_RATE * d_hidden_output;
                self.bias_output += LEARNING_RATE * d_y_pred;
            }

            if epoch % 100 == 0 {
                println!("Epoch {}: error = {:.4}", epoch, total_error);
            }
        }
    }

    fn predict(&self, x: &[f32]) -> f32 {
        let (_, _, _, y_pred) = self.forward_pass(x);
        y_pred
    }
}

pub fn test1() {
    let num_samples = 10_000;
    let mut rng = rand::thread_rng();

    // Generate synthetic data
    let X: Vec<Vec<f32>> = (0..num_samples)
        .map(|_| {
            (0..INPUT_SIZE)
                .map(|_| rng.gen_range(0.0..1.0))
                .collect()
        })
        .collect();
    let Y: Vec<f32> = (0..num_samples)
        .map(|i| if i % 2 == 0 { 0.0 } else { 1.0 })
        .collect();

    let mut nn = NeuralNetwork::new();
    nn.train(&X, &Y);

    println!("\n=== Predictions on first 4 samples ===");
    for x in X.iter().take(4) {
        let y_pred = nn.predict(x);
        println!("x = {:?} → y_pred = {:.4}", x, y_pred);
    }

    let x_new = vec![0.5, 0.5, 0.5, 0.5, 0.5];
    let y_pred_new = nn.predict(&x_new);
    println!("\n=== Prediction on new input ===");
    println!("x_new = {:?} → y_pred = {:.4}", x_new, y_pred_new);
}