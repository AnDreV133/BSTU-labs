float sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

float sigmoid_derivative(float x) {
    float sx = sigmoid(x);
    return sx * (1.0f - sx);
}

__kernel void forward_pass(
    __global const float *input,
    __global const float *weights,
    __global float *hidden_input,
    float bias_hidden
) {
    int gid = get_global_id(0);
    if (gid != 0) return; // Process only one work item

    float sum = 0.0f;
    for (int i = 0; i < 5; i++) { // INPUT_SIZE = 5
        sum += input[i] * weights[i];
    }
    hidden_input[0] = sum + bias_hidden;
}

__kernel void backward_pass(
    __global const float *input,
    __global float *weights,
    float d_hidden_output,
    float learning_rate
) {
    int gid = get_global_id(0);
    if (gid != 0) return; // Process only one work item

    for (int i = 0; i < 5; i++) { // INPUT_SIZE = 5
        weights[i] += learning_rate * input[i] * d_hidden_output;
    }
}