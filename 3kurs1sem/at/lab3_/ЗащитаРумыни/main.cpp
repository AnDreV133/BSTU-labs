#include <iostream>

void program3();

int main() {
    program3();
    return 0;
}

typedef struct structProgram1 {
    char **data;
    size_t size;
    size_t capacity;
} arrayChains;

typedef struct LSF_TO {
    char *data;
    size_t size;
    size_t capacity;
    size_t left;
} LSF_TO;
typedef struct structSteps {
    size_t *data;
    size_t size;
    size_t capacity;
} structSteps;


void program3() {
    size_t **steps = (size_t **) malloc(sizeof(size_t *) * 4);
    size_t steps1[] = {1, 2, 3, 5, 8, 6, 2, 3, 5, 8, 4, 3, 6};
    size_t steps2[] = {2, 1, 3, 5, 8, 4, 3, 6, 2, 3, 5, 8, 6};
    size_t steps3[] = {1, 2, 2, 3, 3, 5, 5, 8, 8, 4, 6, 3, 6};
    size_t steps4[] = {2, 1, 2, 3, 5, 8, 6, 3, 5, 8, 4, 3, 6};
    steps[0] = steps1;
    steps[1] = steps2;
    steps[2] = steps3;
    steps[3] = steps4;
    bool isCan;
    for (size_t step = 0; step < 4; step++) {
        LSF_TO tree;
        tree.data = (char *) malloc(sizeof(char) * 200);
        tree.data[0] = 'S';
        tree.data[1] = '(';
        tree.data[2] = ')';
        tree.data[3] = '\0';
        tree.left = 1;
        tree.capacity = 200;
        tree.size = 4;
        arrayChains rules;
        rules.data = (char **) malloc(sizeof(char *) * 8);
        rules.capacity = 8;
        rules.size = 8;
        rules.data[0] = "S-SbSa";
        rules.data[1] = "S-Sa";
        rules.data[2] = "S-A";
        rules.data[3] = "A-aS";
        rules.data[4] = "A-aB";
        rules.data[5] = "A-b";
        rules.data[6] = "B-b";
        rules.data[7] = "B-Aa";
        arrayChains res;
        res.data = (char **) malloc(sizeof(char *) * 50);
        res.data[0] = (char *) malloc(sizeof(char) * 2);
        res.data[0][0] = 'S';
        res.data[0][1] = '\0';
        res.capacity = 50;
        res.size = 1;
        printf("CF-grammar:\n");
        for (size_t j = 0; j < rules.size; j++)
            printf("%zu. %s\n", j + 1, rules.data[j]);
        printf("Rules: ");
        for (size_t j = 0; j < 13 - 1; j++)
            printf("%zu, ", steps[step][j]);
        printf("%zu, ", steps[step][12]);
        for (size_t i = 0; i < 13; i++) {
            //printf("%s\n", res.data[res.size - 1]);
            bool isOver = false;
            isCan = isOver;
            size_t left;
            for (size_t j = 0; j < res.size; j++) {
                if (res.data[res.size - 1][j] == rules.data[steps[step][i] -
                                                            1][0]) {
                    isOver = true;
                    isCan = isOver;
                    left = j;
                    break;
                }
            }
            if (!isOver) {
                //printf("Sequence %zu - not\n", step + 1);
                break;
            }
            size_t minSize = 0;
            while (rules.data[steps[step][i] - 1][minSize + 2] != '\0')
                minSize++;
            char buff[200];
            size_t index = 0;
            size_t index_ = 0;
            for (; index <= tree.left; index++) {
                buff[index] = tree.data[index_];
                index_++;
            }
            for (size_t j = 2; rules.data[steps[step][i] - 1][j] != '\0';
                 j++) {
                if (isupper(rules.data[steps[step][i] - 1][j])) {
                    buff[index] = rules.data[steps[step][i] - 1][j];
                    index++;
                    buff[index] = '(';
                    index++;
                    buff[index] = ')';
                    index++;
                } else {
                    buff[index] = rules.data[steps[step][i] - 1][j];
                    index++;
                }
            }
            for (; index_ < tree.size; index_++) {
                buff[index] = tree.data[index_];
                index++;
            }
            buff[index] = '\0';
            for (size_t k = 0; k <= index; k++) {
                tree.data[k] = buff[k];
            }
            size_t newLeft = tree.left + 1;
            for (; islower(tree.data[newLeft]); newLeft++) {}
            tree.left = newLeft + 1;
            tree.size = index;
            size_t sizeResFragment = 0;
            while (res.data[res.size - 1][sizeResFragment] != '\0')
                sizeResFragment++;
            res.data[res.size] = (char *) malloc(sizeof(char) * 50);
            size_t ind = 0;
            if (left > 0)
                for (size_t j = 0; j < left; j++)
                    res.data[res.size][ind++] = res.data[res.size - 1][j];
            size_t sizeRulesMin = 2;
            while (rules.data[steps[step][i] - 1][sizeRulesMin] != '\0')
                sizeRulesMin++;
            for (size_t j = 2; j < sizeRulesMin; j++)
                res.data[res.size][ind++] = rules.data[steps[step][i] -
                                                       1][j];
            if (left < sizeResFragment - 1)
                for (size_t j = left + 1; j < sizeResFragment; j++)
                    res.data[res.size][ind++] = res.data[res.size - 1][j];
            res.data[res.size][ind] = '\0';
            res.size++;
        }
        if (isCan)
            printf("Result: \"yes\"\n");//printf("Terminal: %s\nTree: %s\n",res.data[res.size - 1], tree.data);
        else
            printf("Result: \"no\"\n");

        free(tree.data);
        free(rules.data);
        for (size_t i = 0; i < res.size; i++)
            free(res.data[i]);
        free(res.data);
    }
    free(steps);
}
