import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import expon, kstest

lambda_param = 2 / 4
sample_size = 10000
intervals = 30

sample = np.random.exponential(size=sample_size)

# Теоретические значения
theoretical_mean = 1 / lambda_param
theoretical_var = 1 / (lambda_param ** 2)

# Выборочные характеристики
sample_mean = np.mean(sample)
sample_var = np.var(sample)
print(f"Теоретическое среднее: {theoretical_mean:.4f}")
print(f"Выборочное среднее: {sample_mean:.4f}")
print(f"Теоретическая дисперсия: {theoretical_var:.4f}")
print(f"Выборочная дисперсия: {sample_var:.4f}")

# Тест Колмогорова-Смирнова
ks_statistic, p_value = kstest(sample, 'expon', args=(0, 1))
print(f"K-S статистика: {ks_statistic:.4f}")
print(f"P-значение: {p_value:.4f}")

# Интерпретация p-значения
alpha = 0.05
if p_value > alpha:
    print("Соответсвует распределению")
else:
    print("Не соответствует распределению")


x = np.linspace(0, 10, sample_size)
y = expon.pdf(x)

plt.hist(sample, bins=intervals, density=True, alpha=0.6, color='g', label='Гистограмма частот')
plt.plot(x, y, 'r-', label='Теоретическая плотность')
plt.title('Гистограмма и теоретическая плотность распределения')
plt.xlabel('Значения')
plt.ylabel('Плотность')
plt.legend()
plt.show()
