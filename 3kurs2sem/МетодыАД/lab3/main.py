import pandas as pd
import numpy as np
import datetime
import scipy
import matplotlib.pyplot as plt
import random
from sklearn.cluster import KMeans, AgglomerativeClustering
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import silhouette_score
from sklearn.manifold import TSNE
import seaborn as sns

from sklearn.decomposition import PCA
from sklearn.datasets import load_iris
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split



data=pd.read_csv('/content/adult.csv')

df = data.copy()
df = df.drop(['education', 'fnlwgt'], axis=1)


dataset = pd.read_csv('/content/adult.csv', usecols=['age','education-num','hours-per-week','salary'])[0:2000]
#str_fe = ['workclass', 'marital-status', 'occupation', 'relationship', 'race', 'sex', 'native-country', 'salary']
str_fe = ['salary']
#dataset = dataset.drop(['education', 'fnlwgt'], axis=1)
for cat in str_fe:
    dataset[cat] = pd.factorize(dataset[cat], sort=True)[0]

# Подготавливаем числовые данные
scaler = StandardScaler()
dataset = pd.DataFrame(data = scaler.fit_transform(dataset),columns = dataset.columns)

pca = PCA(n_components=2)
pca.fit(dataset)

#Применение метода кластеризации
clasterizer = KMeans(n_clusters=2)
cluster = clasterizer.fit_predict(dataset)

x_pca=pca.fit_transform(dataset)
clasterizer = KMeans(n_clusters=5)
cluster = clasterizer.fit_predict(x_pca)
x_pca = pd.DataFrame(x_pca)
x_pca['claster'] = clasterizer.labels_

# giving a larger plot
plt.scatter(x_pca.iloc[:,0],x_pca.iloc[:,1],c=x_pca.iloc[:,2])

plt.show()

dataset['claster'] = clasterizer.labels_

for clast in range(5):
    print(dataset.loc[dataset['claster'] == clast].mean())


x_pca.iloc[2:10, 0]


# distributing the dataset into two components X and Y
dataset = pd.read_csv('/content/adult.csv', usecols=['age','education-num','hours-per-week','salary'])[0:100]
#str_fe = ['workclass', 'marital-status', 'occupation', 'relationship', 'race', 'sex', 'native-country', 'salary']
str_fe = ['salary']
#dataset = dataset.drop(['education', 'fnlwgt'], axis=1)
for cat in str_fe:
    dataset[cat] = pd.factorize(dataset[cat], sort=True)[0]


X = dataset.iloc[:, 0:3].values
y = dataset.iloc[:, 3].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=0)
# performing preprocessing part
sc = StandardScaler()

X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)
# Applying PCA function on training
# and testing set of X component

pca = PCA(n_components=2)

X_train = pca.fit_transform(X_train)
X_test = pca.transform(X_test)

explained_variance = pca.explained_variance_ratio_

classifier = LogisticRegression(random_state=0)
classifier.fit(X_train, y_train)
y_pred = classifier.predict(X_test)
cm = confusion_matrix(y_test, y_pred)

# Predicting the training set
# result through scatter plot
from matplotlib.colors import ListedColormap

X_set, y_set = X_train, y_train
X1, X2 = np.meshgrid(np.arange(start=X_set[:, 0].min() - 1,
                               stop=X_set[:, 0].max() + 1, step=0.01),
                     np.arange(start=X_set[:, 1].min() - 1,
                               stop=X_set[:, 1].max() + 1, step=0.01))

plt.contourf(X1, X2, classifier.predict(np.array([X1.ravel(),
                                                  X2.ravel()]).T).reshape(X1.shape), alpha=0.75,
             cmap=ListedColormap(('yellow', 'white', 'aquamarine')))

plt.xlim(X1.min(), X1.max())
plt.ylim(X2.min(), X2.max())

for i, j in enumerate(np.unique(y_set)):
    plt.scatter(X_set[y_set == j, 0], X_set[y_set == j, 1],
                color=ListedColormap(('red', 'green', 'blue'))(i), label=j)

plt.title('Logistic Regression (Training set)')
plt.xlabel('PC1')  # for Xlabel
plt.ylabel('PC2')  # for Ylabel
plt.legend()  # to show legend

# show scatter plot
plt.show()