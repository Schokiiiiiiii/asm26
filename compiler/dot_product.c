int dot_product(int* a, int* b, const int size) {
    int result = 0;
    for (int i = 0; i < size; ++i) {
        result += a[i] + b[i];
    }
    return result;
}