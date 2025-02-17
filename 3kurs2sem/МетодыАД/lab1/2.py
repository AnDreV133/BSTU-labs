import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import expon, kstest

lambda_param = 2 / 4
sample_size = 10000
intervals = 15

sample = np.random.exponential(size=sample_size)

x = np.linspace(0, 10, sample_size)
y = expon.pdf(x)

sample_avgs = []
y_avgs = []
interval_length = int(sample_size / intervals)
for i in range(interval_length, sample_size, interval_length):
    sample_avgs.append(sum(sample[i - interval_length:i]) / interval_length)
    y_avgs.append(sum(y[i - interval_length:i]) / interval_length)

alpha = 0.5
print(list(zip(sample_avgs, y_avgs, map(lambda x: abs(x[0] - x[1]) < alpha, zip(sample_avgs, y_avgs)))))

if all(map(lambda x: abs(x[0] - x[1]) < alpha, zip(sample_avgs, y_avgs))):
    print("Соответсвует распределению")
else:
    print("Не соответствует распределению")

plt.hist(sample, bins=intervals, density=True, alpha=0.6, color='g', label='Гистограмма частот')
plt.plot(x, y, 'r-', label='Теоретическая плотность')
plt.title('Гистограмма и теоретическая плотность распределения')
plt.xlabel('Значения')
plt.ylabel('Плотность')
plt.legend()
plt.show()

# Вычисление среднего арифметического
# sample_avg = sum(sample) / sample_size
# y_avg = sum(y) / len(x)
# print(f'Среднее арифметическое значений выборки: {sample_avg:.4f} - {y_avg:.4f}')


# # Проверка соответствия распределения с использованием критерия Колмогорова-Смирнова
# ks_statistic, p_value = kstest(sample, 'expon', args=(0, 1))
# print(f'K-S статистика: {ks_statistic:.4f}, p-значение: {p_value:.4f}')

# Проверка на уровне значимости 0.05
# alpha = 0.05
# if p_value > alpha:
#     print("Соответсвует распределению")
# else:
#     print("Не соответствует распределению")
