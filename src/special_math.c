#include "special_math.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Замена нестандартной функции strdup для строгого C11
static char* my_strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* new_s = (char*)malloc(len);
    if (new_s) {
        strcpy(new_s, s);
    }
    return new_s;
}

char* big_add(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int max_len = (len_a > len_b) ? len_a : len_b;
    
    char* result = (char*)malloc(max_len + 2);
    if (!result) return NULL;
    
    int carry = 0; 
    int i = len_a - 1;
    int j = len_b - 1;
    int k = 0;
    
    while (i >= 0 || j >= 0 || carry) {
        int digit_a = (i >= 0) ? (a[i] - '0') : 0;
        int digit_b = (j >= 0) ? (b[j] - '0') : 0;
        
        int sum = digit_a + digit_b + carry;
        result[k++] = (sum % 10) + '0'; 
        carry = sum / 10;               
        
        i--; j--;
    }
    result[k] = '\0';
    
    for (int l = 0; l < k / 2; l++) {
        char temp = result[l];
        result[l] = result[k - 1 - l];
        result[k - 1 - l] = temp;
    }
    return result;
}

char* big_sub(const char* a, const char* b) {
    int len_a = strlen(a), len_b = strlen(b);
    char* res = (char*)malloc(len_a + 1);
    int i = len_a - 1, j = len_b - 1, k = 0, borrow = 0;
    
    while (i >= 0) {
        int sub = (a[i] - '0') - borrow - (j >= 0 ? (b[j] - '0') : 0);
        if (sub < 0) { sub += 10; borrow = 1; } else { borrow = 0; }
        res[k++] = sub + '0';
        i--; if (j >= 0) j--;
    }
    while (k > 1 && res[k - 1] == '0') k--; 
    res[k] = '\0';
    
    for (int l = 0; l < k / 2; l++) {
        char temp = res[l]; res[l] = res[k - 1 - l]; res[k - 1 - l] = temp;
    }
    return res;
}

char* big_mul(const char* a, const char* b) {
    int len_a = strlen(a), len_b = strlen(b);
    int* res = (int*)calloc(len_a + len_b, sizeof(int));
    
    for (int i = len_a - 1; i >= 0; i--) {
        for (int j = len_b - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = mul + res[p2];
            res[p1] += sum / 10;
            res[p2] = sum % 10;
        }
    }
    
    char* result = (char*)malloc(len_a + len_b + 1);
    int k = 0, i = 0;
    while (i < len_a + len_b && res[i] == 0) i++; 
    if (i == len_a + len_b) { 
        result[0] = '0'; result[1] = '\0'; 
    } else {
        while (i < len_a + len_b) result[k++] = res[i++] + '0';
        result[k] = '\0';
    }
    free(res);
    return result;
}

char* big_factorial(int n) {
    if (n < 0) return NULL;
    char* result = my_strdup("1");
    for (int i = 2; i <= n; i++) {
        char num_str[12];
        sprintf(num_str, "%d", i);
        char* temp = big_mul(result, num_str);
        free(result);
        result = temp;
    }
    return result;
}

char* big_power(const char* base, int exp) {
    if (exp < 0) return NULL;
    char* result = my_strdup("1");
    for (int i = 0; i < exp; i++) {
        char* temp = big_mul(result, base);
        free(result);
        result = temp;
    }
    return result;
}

char* convert_base(const char* num, int from_base, int to_base) {
    if (from_base < 2 || from_base > 36 || to_base < 2 || to_base > 36) return NULL;
    
    char* dec = my_strdup("0");
    for (int i = 0; i < (int)strlen(num); i++) {
        int val = (num[i] >= '0' && num[i] <= '9') ? num[i] - '0' : (num[i] >= 'A' && num[i] <= 'Z') ? num[i] - 'A' + 10 : num[i] - 'a' + 10;
        char base_str[12]; sprintf(base_str, "%d", from_base);
        char val_str[12]; sprintf(val_str, "%d", val);
        
        char* temp = big_mul(dec, base_str);
        char* temp2 = big_add(temp, val_str);
        free(temp); free(dec); dec = temp2;
    }
    
    if (strcmp(dec, "0") == 0) { free(dec); return my_strdup("0"); }
    
    char* res = (char*)malloc(strlen(dec) * 4 + 1); 
    int k = 0;
    
    while (strcmp(dec, "0") != 0) {
        int rem = 0;
        for (int i = 0; i < (int)strlen(dec); i++) {
            int current = rem * 10 + (dec[i] - '0');
            dec[i] = (current / to_base) + '0';
            rem = current % to_base;
        }
        res[k++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'A');
        
        int start = 0; 
        while (dec[start] == '0' && start < (int)strlen(dec) - 1) start++;
        if (start > 0) memmove(dec, dec + start, strlen(dec) - start + 1);
    }
    res[k] = '\0';
    
    for (int l = 0; l < k / 2; l++) { 
        char t = res[l]; res[l] = res[k - 1 - l]; res[k - 1 - l] = t; 
    }
    free(dec);
    return res;
}
