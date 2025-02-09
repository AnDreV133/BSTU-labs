//
// Created by dmitr on 09.04.2023.
//

#include "Vector.h"

#include <stdio.h>
#include <memory.h>

// возвращает структуру-дескриптор вектор из currCapacity значений.
Vector createVector(size_t currCapacity) {
    if (currCapacity == 0)
        currCapacity = 1;

    int *data = (int *) malloc(currCapacity * sizeof(int));

    if (data == NULL) {
        fprintf(stderr, "bad alloc");
        exit(1);
    }

    return (Vector) {data, 0, currCapacity};
}

void reserve(Vector *v, size_t newCapacity) {
    if (newCapacity < v->size)
        v->size = newCapacity;
    else if (newCapacity == 0) {
        *v = (Vector) {NULL, 0, 0};
        return;
    }

    v->capacity = newCapacity;
    v->data = (int *) realloc(v->data, newCapacity * sizeof(int));
}

void clear(Vector *v) {
    v->size = 0;
}

void shrinkToFit(Vector *v) {
    v->capacity = v->size;
}

void deleteVector(Vector *v) {
    free(v->data);
}

bool isEmpty(Vector v) {
    return v.size == 0;
}

bool isFull(Vector v) {
    return v.size == v.capacity;
}

int getVectorValue(Vector v, size_t i) {
    return v.data[i];
}

void pushBack(Vector *v, int x) {
    if (isFull(*v))
        reserve(v, v->capacity * 2);

    v->data[v->size] = x;
    v->size++;
}

void popBack(Vector *v) {
    if (isEmpty(*v)) {
        fprintf(stderr, "vector is empty");
        exit(1);
    }

    v->size--;
}

int *atVector(Vector v, size_t index) {
    if (v.size == 0)
        return v.data;

    if (index >= v.size) {
        fprintf(stderr, "IndexError: a[index] is not exists");
        exit(1);
    }

    return &v.data[index];
}

int *back(Vector v) {
    return &v.data[v.size != 0 ? v.size - 1 : 0];
}

int *front(Vector v) {
    return v.data;
}

void arrayToVector(Vector *v, int *a, int n) {
    reserve(v, n);

    memcpy(v->data, a, n * sizeof(int));
}