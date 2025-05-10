import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans, AgglomerativeClustering
from sklearn.preprocessing import StandardScaler
from sklearn.manifold import TSNE
import seaborn as sns

data = pd.read_csv('/content/adult.csv')
data_analys = data.describe()

headers = data.columns.tolist()
print(headers)

data = pd.read_csv('data.csv', sep=';')


def clastirize(clasters_amount, ClastirizeMethod):
    data = pd.read_csv('/content/adult.csv')
    df = data.copy()[0:2000]
    str_fe = ['workclass', 'marital-status', 'occupation', 'relationship', 'race', 'sex', 'native-country', 'salary']
    num_fe = df[["age", "education-num", "hours-per-week"]]

    df = df.drop(['education', 'fnlwgt'], axis=1)

    # Преобразуем строковые данные к числовым
    for cat in str_fe:
        df[cat] = pd.factorize(df[cat], sort=True)[0]
    df_not_fit = df.copy()
    # Подготавливаем числовые данные
    scaler = StandardScaler()
    df = pd.DataFrame(data=scaler.fit_transform(df), columns=df.columns)

    # Применение метода кластеризации
    clasterizer = ClastirizeMethod(n_clusters=clasters_amount)
    cluster = clasterizer.fit_predict(df)

    df_not_fit['claster'] = clasterizer.labels_

    tsne2D = TSNE(n_components=2)
    tsne_data2D = tsne2D.fit_transform(df)
    tsne2D_df = pd.DataFrame(data=tsne_data2D, columns=['x', 'y'])

    tsne2D_df['cluster'] = cluster

    sns.scatterplot(x='x', y='y', hue='cluster', data=tsne2D_df)
    plt.title("T-SNE")
    plt.show()

    for clast in range(clasters_amount):
        print(df_not_fit.loc[df_not_fit['claster'] == clast].mean())


clastirize(2, KMeans)
clastirize(5, KMeans)
clastirize(2, AgglomerativeClustering)
clastirize(5, AgglomerativeClustering)

