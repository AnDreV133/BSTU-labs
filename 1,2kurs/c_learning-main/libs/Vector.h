//
// Created by dmitr on 09.04.2023.
//

#ifndef MAIN_C_VECTOR_H
#define MAIN_C_VECTOR_H

#endif //MAIN_C_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector {
    int *data; // указатель на элементы вектора
    size_t size; // размер вектора
    size_t capacity; // вместимость вектора
} Vector;

Vector createVector(size_t currCapacity);

// изменяет количество
// памяти, выделенное под хранение элементов вектора
void reserve(Vector *v, size_t newCapacity);

// удаляет элементы из контейнера,
// но не освобождает выделенную память
void clear(Vector *v);

// освобождает память, выделенную
// под неиспользуемые элементы
void shrinkToFit(Vector *v);

// освобождает память, выделенную вектору
void deleteVector(Vector *v);

bool isEmpty(Vector v);

bool isFull(Vector v);

// возвращает i-ый элемент вектора v
int getVectorValue(Vector v, size_t i);

// добавляет элемент x в конец вектора v
void pushBack(Vector *v, int x);

// удаляет последний элемент из вектора
void popBack(Vector *v);

// возвращает указатель на
// index-ый элемент вектора
int *atVector(Vector v, size_t index);

// возвращает указатель
// на последний элемент вектора.
int* back(Vector v);

// возвращает указатель
// на нулевой элемент вектора
int* front(Vector v);

// записывает массив в вектор
void arrayToVector(Vector *v, int *a, int n);