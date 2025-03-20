import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import pearsonr, norm, chisquare
import numpy as np

data = pd.read_csv('data.csv', sep=';')

# plt.title('COVID-19')
# plt.plot(data['infected'], color='blue')
# plt.plot(data['recovered'], color='green')
# plt.plot(data['deaths'], color='black')
# plt.plot(data['critical'], color='red')
# plt.plot(data['sicked'], color='yellow')
# plt.show()

data_for_test = [
    (data['infected'], "зараженные"),
    (data['recovered'], "выздоровевшие"),
    (data['deaths'], "умершие"),
    (data['critical'], "критические ситуации"),
    (data['sicked'], "болеющие")
]

for named_data in data_for_test:
    x = np.linspace(1, len(named_data[0]), len(named_data[0]))
    y = named_data[0]

    # y = a * x ^ b
    log_y = np.log(y)

    mean_x = np.mean(x)
    mean_log_y = np.mean(log_y)

    lin_k = np.mean(
        ((x - mean_x) * (log_y - mean_log_y))
        / ((x - mean_x) ** 2)
    )

    lin_b = mean_log_y - lin_k * mean_x

    # y = a * e^(bx)
    a = np.exp(lin_b)
    b = lin_k

    y_repr = a * np.exp(b * x)

    K, p_value = pearsonr(y, y_repr)
    print(f"Для '{named_data[1]}':", "K=", K, "p-value=", p_value)
    print(f"Функция регрессии: y=({a:.2})e^(({b:.2})x)")

    ei = y - y_repr
    mean_ei = np.mean(ei)

    sigma = np.mean(ei ** 2 - mean_ei ** 2) ** 0.5

    pi = norm.cdf(x+1 / sigma) - norm.cdf(x / sigma)
    print(pi)
    # K, p_value = chisquare(pi, ddof=1)
    # print("Адекватность регрессии:", "K=", K, "p-value=", p_value)


    plt.close()
    plt.title(f"COVID-19 '{named_data[1]}'")
    plt.plot(x, y, color='blue')
    plt.plot(x, y_repr, color='red')
    plt.show()
