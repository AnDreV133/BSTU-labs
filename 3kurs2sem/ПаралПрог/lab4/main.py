import math
import random

INPUT_SIZE = 5

w_input_hidden = [random.uniform(-1, 1) for _ in range(INPUT_SIZE)]
w_hidden_output = random.uniform(-1, 1)
bias_hidden = random.uniform(-1, 1)
bias_output = random.uniform(-1, 1)


def sigmoid(x):
    return 1. / (1. + math.exp(-x))


def sigmoid_derivative(x):
    sx = sigmoid(x)
    return sx * (1. - sx)


def forward_pass(x):
    hidden_input = sum(x[i] * w_input_hidden[i] for i in range(INPUT_SIZE)) + bias_hidden
    hidden_output = sigmoid(hidden_input)

    final_input = hidden_output * w_hidden_output + bias_output
    y_pred = sigmoid(final_input)
    return hidden_input, hidden_output, final_input, y_pred


def train(X, Y, epochs, learning_rate):
    global w_input_hidden, w_hidden_output, bias_hidden, bias_output
    for epoch in range(epochs):
        total_error = 0
        for x, y_true in zip(X, Y):
            hidden_input, hidden_output, final_input, y_pred = forward_pass(x)

            error = y_true - y_pred
            total_error += error ** 2

            d_y_pred = error * sigmoid_derivative(final_input)
            d_hidden_output = d_y_pred * w_hidden_output * sigmoid_derivative(hidden_input)

            # Обновляем веса
            for wi in range(INPUT_SIZE):
                w_input_hidden[wi] += learning_rate * x[wi] * d_hidden_output
            w_hidden_output += learning_rate * hidden_output * d_y_pred

            bias_hidden += learning_rate * d_hidden_output
            bias_output += learning_rate * d_y_pred

        if epoch % 100 == 0:
            print(f"Epoch {epoch}: total_error={total_error:.4f}")


def predict(x):
    _, _, _, y_pred = forward_pass(x)
    return y_pred


X = [
    [0.1, 0.2, 0.3, 0.4, 0.5],
    [0.2, 0.1, 0.4, 0.3, 0.5],
    [0.6, 0.7, 0.8, 0.9, 1.0],
    [0.9, 0.8, 1.0, 0.7, 0.6]
]

Y = [0, 0, 1, 1]

train(X, Y, epochs=100_001, learning_rate=0.1)

print("\n=== PREDICTIONS ON TRAINING DATA ===")
for x in X:
    y_pred = predict(x)
    print(f"x={x} → y_pred={y_pred:.4f}")

x_new = [0.5, 0.5, 0.5, 0.5, 0.5]
y_pred_new = predict(x_new)

print("\n=== PREDICTION ON NEW INPUT ===")
print(f"x_new={x_new} → y_pred={y_pred_new:.4f}")
