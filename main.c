#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#define RED "\x1b[31m"
#define BLUE "\x1b[36m"
#define RESET "\x1b[0m"
#define GREEN "\033[0;32m"
#define ESC 27

#define MAX_LIM 100.0
#define MIN_LIM 1e-2
#define MAX_Y 50.0
#define MIN_Y 1e-2
#define EPS 1e-10
#define MAX_ITER 100
#define OFFSET 0.5

void print_greeting() {
    printf(BLUE"Hello, this is a calculator for a certain type of equations\n"RESET);
    printf(BLUE"You'll have to choose an equation, a method, enter limits and an additional value\n"RESET);
    printf(BLUE"The upper limit must be greater than the lower limit\n"RESET);
}

void print_instruction(int ch) {
    if (ch == 1) {
        printf(BLUE"For this equation the domain is {x є R: x != 0}\n"RESET);
        printf(BLUE"The module of the limits must be between %.2lf and %.2lf\n"RESET, MIN_LIM, MAX_LIM);
        printf(BLUE"The module of y must be between %.2lf and %.2lf\n\n"RESET, MIN_Y, MAX_Y);
    }
    else if (ch == 2) {
        printf(BLUE"For this equation the domain is {x є R: x > 0}\n"RESET);
        printf(BLUE"The value of the limits must be between %.2lf and %.2lf\n"RESET, MIN_LIM, MAX_LIM);
        printf(BLUE"The module of y must be between %.2lf and %.2lf\n\n"RESET, MIN_Y, MAX_Y);
    }
}

bool is_esc() {
    printf(BLUE"Press ESC key to exit, or any other key to try again: "RESET);
    char escChoice = _getch();
    printf("\n");
    system("cls");
    return escChoice == ESC;
}

bool is_input_valid(void* inp_ptr, const char* format) {
    int start_space = 0;
    int newline = 0;
    int result = scanf(format, &start_space, inp_ptr, &newline);
    rewind(stdin);
    if (start_space == 0 && result == 2 && newline == '\n') {
        return true;
    }
    printf(RED"Error! Invalid input format!\n\n"RESET);
    return false;
}

bool is_inp_valid(int num, int low_lim, int up_lim) {
    if (!(num >= low_lim && num <= up_lim)) {
        printf(RED"Error! Invalid choice!\n\n"RESET);
        return false;
    }
    return true;
}

void take_choice(int* inp, const char* message) {
    do {
        printf(BLUE"%s"RESET, message);
    } while (!is_input_valid(inp, " %n%d%c") || !is_inp_valid(*inp, 1, 2));
}

bool is_data_valid(double num, int ch) {
    bool isValid = false;
    switch (ch) {
    case 1:
        if (fabs(num) >= MIN_Y && fabs(num) <= MAX_Y) {
            isValid = true;
        }
        else
        {
            printf(RED"Error! Invalid number\n"RESET);
        }
        break;
    case 2:
        if (fabs(num) >= MIN_Y && fabs(num) <= MAX_Y) {
            isValid = true;
        }
        else
        {
            printf(RED"Error! Invalid number\n"RESET);
        }
        break;
    default:
        printf(RED"An error happened\n"RESET);
    }
    return isValid;
}

void read_num_data(double* y, int ch) {
    do {
        printf(BLUE"Enter y: "RESET);
    } while (!is_input_valid(y, " %n%lf%c") || !is_data_valid(*y, ch));
}

bool is_lim_valid(double lim, int ch) {
    bool isLimValid = false;
    switch (ch) {
    case 1:
        if (fabs(lim) >= MIN_LIM && fabs(lim) <= MAX_LIM) {
            isLimValid = true;
        }
        else {
            printf(RED"Invalid limit!\n"RESET);
        }
        break;
    case 2:
        if (lim >= MIN_LIM && lim <= MAX_LIM) {
            isLimValid = true;
        }
        else {
            printf(RED"Invalid limit!\n"RESET);
        }
        break;
    default:
        printf(RED"An error happened!\n");
    }
    return isLimValid;
}

void take_lim(double* lim1, double* lim2, int ch) {
    bool isValid = false;
    do {
        do {
            printf(BLUE"Enter lower limit: "RESET);
        } while (!is_input_valid(lim1, " %n%lf%c") || !is_lim_valid(*lim1, ch));
        do {
            printf(BLUE"Enter upper limit: "RESET);
        } while (!is_input_valid(lim2, " %n%lf%c") || !is_lim_valid(*lim2, ch));
        if (*lim1 < *lim2) {
            isValid = true;
        }
        else {
            printf(RED"Error! The uper limit must be greater than the lower limit!\n"RESET);
        }
    } while (!isValid);
}

double calc_equation(double x, double y, int ch) {
    double res = 0.0;

    switch (ch) {
    case 1:
        res = cos(y / x) - 2.0 * sin(1.0 / x) + (1.0 / x);
        break;
    case 2:
        res = sin(log(x)) - cos(log(x)) + y * log(x);
        break;
    default:
        printf(RED"Error!"RESET);
    }
    return res;
}

double derivative(double x, double y, int ch) {
    double res = 0.0;

    switch (ch) {
    case 1:
        res = -(y / (x * x)) + (2.0 * x) * sin(1 / x) - (1.0 / (x * x * x));
        break;
    case 2:
        res = (log(x) * cos(log(x))) - (sin(log(x))) / x + y * (1 / x + log(x));
        break;
    default:
        printf(RED"Error!"RESET);
    }
    return res;
}

bool has_roots(double (*f)(double, double, int), double a, double b, double y, int ch) {
    double step = 0.1, f1 = 0, f2 = 0, currentX = a;
    while (currentX < b) {
        f1 = f(currentX, y, ch);
        f2 = f((currentX + step), y, ch);
        if ((f1 < 0 && f2 > 0) || (f1 > 0 && f2 < 0)) {
            return true;
        }
        currentX += step;
    }
    return false;
}

void solve_bisection(double (*f)(double, double, int), double a, double b, double y, int ch) {
    double x = (a + b) / 2;
    int iter = 0;
    bool is_solvable = true;

    do {
        if (a < 0.5 && a > 0) {
            a += 0.5;
        }
        if (f(a, y, ch) * f(x, y, ch) > 0) {
            a = x;
        }
        else {
            b = x;
        }
        x = (a + b) / 2;
        iter++;
        if (iter > MAX_ITER) {
            is_solvable = false;
            printf(RED"Unfortunatelly, this equation is unsolvable\n"RESET);
        }
    } while (fabs(b - a) >= EPS && iter <= MAX_ITER);
    if (is_solvable) {
        printf("%sx = %s%s%lf%s\n", BLUE, RESET, GREEN, x, RESET);
    }

}

void solve_tangents(double (*f)(double, double, int), double (*f_der)(double, double, int), double b, double y, int ch) {
    double x = b;
    double delta = 0;
    int iter = 0;
    bool is_solvable = true;

    if (ch == 1) {
        do {
            delta = f(x, y, ch) / f_der(x, y, ch);
            x = x - delta;
            iter++;
            if (iter > MAX_ITER) {
                is_solvable = false;
                printf(RED"Unfortunatelly, this equation is unsolvable\n"RESET);
            }
        } while (fabs(delta) >= EPS && iter <= MAX_ITER);
    }
    else if (ch == 2) {
        do {
            delta = f(fabs(x), y, ch) / f_der(fabs(x), y, ch);
            x = x - delta;
            iter++;
            if (iter > MAX_ITER) {
                is_solvable = false;
                printf(RED"Unfortunatelly, this equation is unsolvable\n"RESET);
            }
        } while (fabs(delta) >= EPS && iter <= MAX_ITER);
    }
    if (is_solvable) {
        printf("%sx = %s%s%lf%s\n", BLUE, RESET, GREEN, x, RESET);
    }
}

int main() {
    int choice = 0, method = 0;
    double l_lim = 0, u_lim = 0, y = 0;
    do {
        print_greeting();
        take_choice(&choice, "Choose equation:\n1. cos(y/x) - 2 * sin(1/x) + 1/x = 0\n2. sin(ln(x)) - cos(ln(x)) + y * ln(x) = 0\n");
        take_choice(&method, "Choose method:\n1. Bisection Method\n2. Tangents Method\n");
        print_instruction(choice);
        take_lim(&l_lim, &u_lim, choice);
        read_num_data(&y, choice);

        if (has_roots(calc_equation, l_lim, u_lim, y, choice)) {
            if (method == 1) {
                solve_bisection(calc_equation, l_lim, u_lim, y, choice);
            }
            else {
                solve_tangents(calc_equation, derivative, u_lim, y, choice);
            }
        }
        else {
            printf(RED"The equation does not have roots at this interval!\n"RESET);
        }
    } while (!is_esc());
    return 0;
}
