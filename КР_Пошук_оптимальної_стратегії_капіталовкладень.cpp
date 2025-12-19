// КР_Пошук_оптимальної_стратегії_капіталовкладень.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Підключення бібліотек
#include <stdio.h> 
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

// Глобальні змінні, необхідні для реалізації методу гілок і меж
float RecordProfit = -1.0;      
float* BestAssignment = NULL;   
float* CurrentAssignment = NULL; 

/* ПРОТОТИПИ ЗАГАЛЬНИХ ФУНКЦІЙ */

void print_separator(int cols); // Прототип функції, необхідної для друку роздільної межі для таблиць

void print_table(float** k_arr, float** result_arr, int rows, int cols, int iter); // Прототип функії, необхідної для побудови проміжних таблиць, що відносяться до етапу умовної оптимізації

/* ПРОТОТИПИ ФУНКЦІЙ, ЩО ВІДНОСЯТЬСЯ ДО РЕАЛІЗАЦІЇ МЕТОДУ ДИНАМІЧНОГО ПРОГРАМУВАННЯ */

float** dynamic_table(int rows, int cols); // Прототип функції, необхідної для динамічного виділення пам'яті під двохвимірний масив

void clean_memory(float** arr, int rows); // Прототип функції, необхідної для очищення пам'яті від двовимірних масивів

/* ПРОТОТИПИ ФУНКЦІЙ, ЩО ВІДНОСЯТЬСЯ ДО РЕАЛІЗАЦІЇ МЕТОДУ ГІЛОК І МЕЖ */

float get_upper_bound(float** table, int next_factory_idx, int total_factories, int rows); // Пртотип функції, необхідної для розрахунку теоретичної верхньої межі прибутку

void branch_and_bound(float** table, int rows, int total_factories, int current_factory, float current_cost, float current_profit, float max_resource, int step); // Прототип рекурсивної функції, необхідної для реалізації методу гілок і меж

int main() // Головна функція програми
{
    SetConsoleCP(1251); // Підключення кодування Windows-1251 для відображення українських символів
    SetConsoleOutputCP(1251); // Кодування Windows-1251
    // Оголошення змінних
    int cols, rows, correlation, choice;
    float** start_table, resource;
    puts("\t\t|================================================|");
    puts("\t\t|> ПОШУК ОПТИМАЛЬНОЇ СТРАТЕГІЇ КАПІТАЛОВКЛАДЕНЬ <|");
    puts("\t\t|================================================|");
    while (true)
    {
        puts("\n\t\t\t|=========================|");
        puts("\t\t\t|> ГОЛОВНЕ МЕНЮ ПРОГРАМИ <|");
        puts("\t\t\t|=========================|");
        puts("\n\n|----------------------------------------|");
        puts("| Оберіть пункт головного меню:          |");
        puts("| >1< - Ввести нову таблицю значень;     |");
        puts("| >2< - Вийти з програми.                |");
        puts("|----------------------------------------|\n");
        puts("\n|---------------");
        printf("| Ваш вибір: ");
        scanf_s("%d", &choice);
        while (choice != 1 && choice != 2)
        {
            puts("\n|> [ПОМИЛКА] - Зроблено некоректний вибір!");
            printf("| Ваш вибір: ");
            scanf_s("%d", &choice);
        }
        puts("|---------------\n\n");
        if (choice == 1)
        {
            choice = 0;
            puts("\n\n|--------------------------------------");
            printf("| Введіть кількість підприємств (n): ");
            scanf_s("%d", &cols);
            while (cols <= 1)
            {
                puts("\n|> [ПОМИЛКА] - Кількість підприємств має бути більшою за 1!");
                printf("| Введіть кількість підприємств (n): ");
                scanf_s("%d", &cols);
            }
            puts("|--------------------------------------");
            puts("\n|---------------------------------");
            printf("| Введіть загальний ресурс (X): ");
            scanf_s("%f", &resource);
            while (resource <= 0)
            {
                puts("\n|> [ПОМИЛКА] - Загальний ресурс має бути більшим за 0!");
                printf("| Введіть загальний ресурс (X): ");
                scanf_s("%f", &resource);
            }
            puts("|---------------------------------");
            puts("\n|-----------------------------");
            printf("| Введіть крок розподілу: 1/");
            scanf_s("%d", &correlation);
            while (correlation <= 0)
            {
                puts("\n|> [ПОМИЛКА] - Крок розподілу має бути більшим за 0!");
                printf("| Введіть крок розподілу: 1/");
                scanf_s("%d", &correlation);
            }
            puts("|-----------------------------\n");

            rows = (int)(resource / correlation) + 1; // Розрахунок кількості рядків

            start_table = dynamic_table(rows, cols); // Створення початкової таблиці

            // Заповнення першого стовпця початкової таблиці обсягами капіталовкладень
            for (int i = 0; i < rows; i++)
            {
                start_table[i][0] = (float)(i * correlation);
                for (int j = 1; j <= cols; j++)
                {
                    start_table[i][j] = 0.0;
                }
            }

            // Заповнення початкової таблиці
            for (int i = 0; i < rows; i++)
            {
                puts("\n|--------------------------------------------------------------------------------");
                printf("| Введіть значення приросту випуску продукції при обсягу капіталовкладень %.0f:\n", start_table[i][0]);
                for (int j = 1; j <= cols; j++)
                {
                    printf("|-> Підприємство #%d: ", j);
                    scanf_s("%f", &start_table[i][j]);
                    while (start_table[i][j] < 0)
                    {
                        puts("\n|> [ПОМИЛКА] - Значення приросту випуску продукції має бути не від'ємним!");
                        printf("|-> Підприємство #%d: ", j);
                        scanf_s("%f", &start_table[i][j]);
                    }
                }
                puts("|-------------------------");
            }

            // Побудова початкової таблиці
            puts("\n|-------------------------|");
            puts("| Таблиця дана за умовою: |");
            puts("|-------------------------|\n");
            print_separator(cols);
            printf("\t|    X    |");
            for (int j = 1; j <= cols; j++)
            {
                printf("   g%-2d   |", j);
            }
            printf("\n");
            print_separator(cols);

            for (int i = 0; i < rows; i++)
            {
                printf("\t| %7.3f |", start_table[i][0]);
                for (int j = 1; j <= cols; j++)
                {
                    printf(" %7.3f |", start_table[i][j]);
                }
                printf("\n");
            }
            print_separator(cols);
            puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
            _getch();
            puts("\n\n\t\t\t|======================|");
            puts("\t\t\t|> МЕНЮ ВИБОРУ МЕТОДУ <|");
            puts("\t\t\t|======================|\n");
            puts("\n|--------------------------------------------|");
            puts("| Оберіть метод розв'язання задачі:          |");
            puts("| >1< - Метод динамічного програмування;     |");
            puts("| >2< - Метод гілок і меж.                   |");
            puts("|--------------------------------------------|\n");
            puts("\n|---------------");
            printf("| Ваш вибір: ");
            scanf_s("%d", &choice);
            while (choice != 1 && choice != 2)
            {
                puts("\n|> [ПОМИЛКА] - Некоректний вибір методу!");
                printf("| Ваш вибір: ");
                scanf_s("%d", &choice);
            }
            puts("|---------------\n\n");

            if (choice == 1) // Вибір методу динамічного програмування
            {
                int remainder_idx, idx, x_idx;
                float* final_x;
                float** k_table, ** result, ** prev_result, ** current_k_table, ** current_result, max_val, best_delta, gx, f_future, total, current_C, total_max_profit;
                float*** all_results, profit, delta, check_sum_profit;
                all_results = (float***)malloc((cols + 1) * sizeof(float**)); // Виділення пам'яті під архів результатів
                // Перевірка виділення пам'яті
                if (all_results == NULL)
                {
                    puts("\n|-----------------------------------|");
                    puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
                    puts("|-----------------------------------|");
                    exit(1);
                }
                puts("\t|==================================================================================|");
                puts("\t|> ПОШУК ОПТИМАЛЬНОЇ СТРАТЕГІЇ КАПІТАЛОВКЛАДЕНЬ МЕТОДОМ ДИНАМІЧНОГО ПРОГРАМУВАННЯ <|");
                puts("\t|==================================================================================|");


                puts("\n\n\t\t\t|===============================|");
                puts("\t\t\t|> І ЕТАП - УМОВНА ОПТИМІЗАЦІЯ <|");
                puts("\t\t\t|===============================|\n");

                k_table = dynamic_table(rows, rows); // Виділення пам'яті під масив проміжної таблиці
                result = dynamic_table(rows, 1); // Виділення пам'яті під масив, що зберігатиме маскимальні значення випуску продукції та обсяг капіталовкладень при цьому
                // Заповнення таблиці для останнього підприємства
                for (int i = 0; i < rows; i++)
                {
                    k_table[i][0] = start_table[i][0];
                    for (int j = 1; j <= rows; j++)
                    {
                        if (i + 1 == j)
                        {
                            k_table[i][j] = start_table[i][cols];
                            result[i][0] = start_table[i][cols];
                            result[i][1] = start_table[i][0];
                        }
                        else
                        {
                            k_table[i][j] = -1.0; // Ініціалізація невикористаних клітинок
                        }
                    }
                }
                // Вивід таблиці на екран
                puts("\n|---------------------------------|");
                printf("| Крок %-2d (Останнє підприємство): |\n", cols);
                puts("|---------------------------------|\n");
                print_table(k_table, result, rows, rows, cols);

                puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
                _getch();
                all_results[cols] = result; // Результат зберігається 

                clean_memory(k_table, rows); // Очищення пам'яті, що займав масив для побудови проміжної таблиці

                // Зворотній цикл   
                for (int iter = cols - 1; iter > 0; iter--)
                {
                    prev_result = all_results[iter + 1]; // Ініціалізація вказівника на масив з результатами минулого кроку 
                    // Оголошення нових двовимірних масивів для таблиці та проміжних результатів
                    current_k_table = dynamic_table(rows, rows);
                    current_result = dynamic_table(rows, 1);

                    // Заповнення таблиці
                    for (int i = 0; i < rows; i++)
                    {
                        current_k_table[i][0] = start_table[i][0]; // Копіювання у таблицю першого стовпця з початковї таблиці
                        // Ініціалізація найкращих параметрів
                        max_val = -1.0;
                        best_delta = -1.0;

                        for (int j = 1; j <= rows; j++)
                        {
                            if ((j - 1) <= i) // Підходять лише ті клітинки таблиці, що придатні для інвестування
                            {
                                gx = start_table[j - 1][iter]; // Береться значення капіталовкладень з початкової таблиці
                                remainder_idx = i - (j - 1); // Обчислення залишку інвестицій
                                f_future = prev_result[remainder_idx][0]; // Знаходження капітовкладення за залишком 
                                total = gx + f_future; // Значення, що буде відображатися в комірці таблиці

                                current_k_table[i][j] = total;

                                // Якщо значення в комірці приросту продукції більше за припущене максимальне значення в рядку
                                if (total > max_val)
                                {
                                    max_val = total; // Оновлення максимального значення приросту продукції
                                    best_delta = start_table[j - 1][0]; // Розмір капіталовкладення найбільшого значення приросту продукції в рядку
                                }
                            }
                            else
                            {
                                current_k_table[i][j] = -1.0; // Ініціалізація невикористаних клітинок
                            }
                        }

                        // Занесення найкращих значень у таблицю результатів
                        current_result[i][0] = max_val;
                        current_result[i][1] = best_delta;
                    }

                    all_results[iter] = current_result; // Збереження в архів

                    puts("\n|----------|");
                    printf("| Крок %-2d: |\n", iter);
                    puts("|----------|\n");
                    print_table(current_k_table, current_result, rows, rows, iter);
                    puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
                    _getch();
                    clean_memory(current_k_table, rows); // Очищення пам'яті, виділеної під проміжну таблицю 
                }

                puts("\n\n\t\t|=======================================================|");
                puts("\t\t|> ІІ ЕТАП - БЕЗУМОВНА ОПТИМІЗАЦІЯ (Розподіл ресурсів) <|");
                puts("\t\t|=======================================================|\n");

                current_C = resource; // Збереження значення капіталовкладення
                final_x = (float*)malloc((cols + 1) * sizeof(float)); // Динамічне виділення пам'яті під одновимірний масив, що зберігає результат оптимізації
                // Перевірка виділення пам'яті
                if (final_x == NULL)
                {
                    puts("\n|-----------------------------------|");
                    puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
                    puts("|-----------------------------------|");
                    exit(1);
                }
                total_max_profit = 0; // Ініціалізація максимально можливого прибутку
                // Цикл по кожному пройденому кроку
                for (int k = 1; k <= cols; k++)
                {
                    idx = (int)(current_C / correlation);

                    profit = all_results[k][idx][0];
                    delta = all_results[k][idx][1];

                    if (k == 1)
                    {
                        puts("\n|---------------------------------------------------------------------------------------------------");
                        total_max_profit = profit; // Останній крок включає найбільший прибуток
                        printf("| 1-й крок:\n| За даними таблиці максимальний приріст випуску продукції при розподілі %.0f ум. од. \n", current_C);
                        printf("| між %d підприємствами становить: C1 = %.0f, F1(%.0f) = %.1f\n", cols, current_C, current_C, profit);
                        printf("| При цьому першому підприємству потрібно виділити x1* = %.0f ум. од.\n", delta);
                        puts("|---------------------------------------------------------------------------------------------------");
                        puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
                        _getch();
                    }
                    else
                    {
                        puts("\n|-------------------------------------------------------------------------------");
                        printf("| %d-й крок:\n| Визначення величини залишених ум. од. обсягів капіталовкладень,\n", k);
                        printf("| що припадають на частку %d-го ", k);
                        if (k < cols)
                        {
                            printf("і наступних підприємств:\n");
                        }
                        else
                        {
                            printf("підприємства:\n");
                        }

                        printf("| C%d = C%d - x%d* = %.1f - %.1f = %.1f ум. од.\n",
                            k, k - 1, k - 1, current_C, delta, current_C - delta);

                        printf("| F%d(%.1f) = %.1f при виділенні ", k, current_C - delta, profit);
                        printf("| %d-му підприємству x%d* = %.0f ум. од.\n", k, k, delta);
                        puts("|-------------------------------------------------------------------------------");
                        puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
                        _getch();
                    }

                    final_x[k] = delta;
                    current_C -= delta;
                }
                puts("\n\n\t\t\t|==========================|");
                puts("\t\t\t|> РЕЗУЛЬТАТИ ОПТИМІЗАЦІЇ <|");
                puts("\t\t\t|==========================|\n");
                puts("\n|---------------------------------------------");
                printf("| Отже, оптимальний план інвестування:\n");
                printf("| X* = (");

                check_sum_profit = 0;

                for (int k = 1; k <= cols; k++)
                {
                    printf("%.0f", final_x[k]);
                    if (k < cols) printf(", ");

                    x_idx = (int)(final_x[k] / correlation);
                    check_sum_profit += start_table[x_idx][k];
                }
                printf(")\n");

                printf("| Максимальний приріст випуску продукції: F = %.2f ум. од.\n", total_max_profit);
                printf("| Перевірка суми g_k(x): %.2f ум. од.\n", check_sum_profit);
                puts("|---------------------------------------------\n");

                // Очищення пам'яті
                for (int k = 1; k <= cols; k++)
                {
                    clean_memory(all_results[k], rows);
                }
                free(all_results);
                free(final_x);
            }
            else if (choice == 2) // Вибір методу гілок і меж
            {
                float total_invested;
                puts("\t|====================================================================|");
                puts("\t|> ПОШУК ОПТИМАЛЬНОЇ СТРАТЕГІЇ КАПІТАЛОВКЛАДЕНЬ МЕТОДОМ ГІЛОК І МЕЖ <|");
                puts("\t|====================================================================|\n");
                BestAssignment = (float*)malloc((cols + 1) * sizeof(float)); // Динамічне виділення пам'яті під одновимірний масив, що зберігає найкращий розподіл інвестицій
                // Перевірка виділення пам'яті
                if (BestAssignment == NULL)
                {
                    puts("\n|-----------------------------------|");
                    puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
                    puts("|-----------------------------------|");
                    exit(1);
                }
                CurrentAssignment = (float*)malloc((cols + 1) * sizeof(float)); // Динамічне виділення пам'яті під одновимірний масив, що зберігає поточний розподіл інвестицій
                // Перевірка виділення пам'яті
                if (CurrentAssignment == NULL)
                {
                    puts("\n|-----------------------------------|");
                    puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
                    puts("|-----------------------------------|");
                    exit(1);
                }

                RecordProfit = 0.0; // Ініціалізація рекордного прибутку
                puts("|--------------------------|");
                printf("| Початок роботи методу... |\n");
                puts("|--------------------------|");
                branch_and_bound(start_table, rows, cols, 1, 0, 0, resource, correlation);

                puts("\n\n\t\t\t|==========================|");
                puts("\t\t\t|> РЕЗУЛЬТАТИ ГІЛОК І МЕЖ <|");
                puts("\t\t\t|==========================|\n");

                puts("\n|-----------------------------");
                printf("| Максимальний приріст випуску продукції: %.2f\n", RecordProfit);
                printf("| Розподіл інвестицій:\n");

                total_invested = 0; // Ініціалізація загального обсягу інвестицій
                // Цикл по кожному заводу
                for (int j = 1; j <= cols; j++)
                {
                    printf("|-> Підприємство %d: %.0f\n", j, BestAssignment[j]);
                    total_invested += BestAssignment[j]; // Накопичення загального обсягу інвестицій
                }
                printf("|-----------------------------\n");
                printf("Всього інвестовано: %.0f / %.0f\n\n", total_invested, resource);

                // Очищення пам'яті
                free(BestAssignment);
                free(CurrentAssignment);
            }
            clean_memory(start_table, rows); // Очищення пам'яті, що займала початкова таблиця
        }
        else if (choice == 2)
        {
            printf("\n[УВАГА] - Завершення роботи програми...\n");
            _getch();
            return 0; // Завершення роботи програми
        }
    }
}

/* ВИЗНАЧЕННЯ ЗАГАЛЬНИХ ФУНКЦІЙ */

void print_separator(int cols) // Визначення функції, необхідної для друку роздільної межі для таблиць
{
    printf("\t+");
    printf("---------+");
    for (int j = 0; j < cols; j++)
    {
        printf("---------+");
    }
    puts("");
}

void print_table(float** k_arr, float** result_arr, int rows, int cols, int iter) // Визначення функії, необхідної для побудови проміжних таблиць, що відносяться до етапу умовної оптимізації
{
    print_separator(cols + 2);
    printf("\t| C%-2d     |", iter);

    for (int j = 1; j <= cols; j++)
    {
        printf(" %7.0f |", k_arr[j - 1][0]);
    }
    printf(" F%-2d     | delta%-2d |", iter, iter);
    puts("");
    print_separator(cols + 2);

    for (int i = 0; i < rows; i++)
    {
        printf("\t| %7.0f |", k_arr[i][0]);
        for (int j = 1; j <= cols; j++)
        {
            if (k_arr[i][j] < -0.5)
            {
                printf("    -    |");
            }
            else
            {
                printf(" %7.3f |", k_arr[i][j]);
            }
        }
        printf(" %7.3f |", result_arr[i][0]);
        printf(" %7.0f |", result_arr[i][1]);
        puts("");
    }
    print_separator(cols + 2);
}

/* ВИЗНАЧЕННЯ ФУНКЦІЙ, ЩО ВІДНОСЯТЬСЯ ДО РЕАЛІЗАЦІЇ МЕТОДУ ДИНАМІЧНОГО ПРОГРАМУВАННЯ */

float** dynamic_table(int rows, int cols) // Визначення функції, необхідної для динамічного виділення пам'яті під двохвимірний масив
{
    float** arr;
    arr = (float**)malloc(rows * sizeof(float*));
    // Перевірка виділення пам'яті
    if (arr == NULL)
    {
        puts("\n|-----------------------------------|");
        puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
        puts("|-----------------------------------|");
        exit(1);
    }
    for (int i = 0; i < rows; i++)
    {
        arr[i] = (float*)malloc((cols + 1) * sizeof(float));
        // Перевірка виділення пам'яті
        if (arr[i] == NULL)
        {
            puts("\n|-----------------------------------|");
            puts("|> [ПОМИЛКА] - Пам'ять не виділено! |");
            puts("|-----------------------------------|");
            exit(1);
        }
    }
    return arr; // Повернення вказівника на створену область пам'яті
}

void clean_memory(float** arr, int rows) // Визначення функції, необхідної для очищення пам'яті від двовимірних масивів
{
    for (int i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

/* ВИЗНАЧЕННЯ ФУНКЦІЙ, ЩО ВІДНОСЯТЬСЯ ДО РЕАЛІЗАЦІЇ МЕТОДУ ГІЛОК І МЕЖ */

float get_upper_bound(float** table, int next_factory_idx, int total_factories, int rows) // Визначення функції, необхідної для розрахунку теоретичної верхньої межі прибутку
{
    float potential = 0, max_in_col;
    // Цикл по всіх наступних заводах
    for (int j = next_factory_idx; j <= total_factories; j++)
    {
        max_in_col = 0;
        // Пошук максимального приросту випуску продукції в стовпці
        for (int i = 0; i < rows; i++)
        {
            // Якщо поточне значення більше за знайдене раніше максимальне
            if (table[i][j] > max_in_col)
            {
                max_in_col = table[i][j]; // Оновлення максимального значення
            }
        }
        potential += max_in_col; // Додавання максимального значення до потенціалу
    }
    return potential; // Повернення розрахованого потенціалу
}

void branch_and_bound(float** table, int rows, int total_factories, int current_factory, float current_cost, float current_profit, float max_resource, int step) // Визначення рекурсивної функції, необхідної для реалізації методу гілок і меж
{
    float invest_amount, profit_amount, potential_remaining, upper_bound;
    // Якщо всі заводи розглянуті
    if (current_factory > total_factories)
    {
        // Оновлення рекордного приросту випуску продукції, якщо поточний кращий
        if (current_profit > RecordProfit)
        {
            RecordProfit = current_profit;
            // Збереження найкращого розподілу інвестицій
            for (int i = 1; i <= total_factories; i++)
            {
                BestAssignment[i] = CurrentAssignment[i]; // Копіювання поточного розподілу інвестицій у найкращий
            }
            printf("\n|-> Знайдено кращий варіант: [");
            for (int k = 1; k <= total_factories; k++) 
            {
                printf("%.0f", CurrentAssignment[k]); 
                if (k < total_factories)
                {
                    printf(", ");
                }
            }
            printf("] ->  Приріст випуску продукції: %.0f\n", current_profit);
            puts("\n[УВАГА] - Натисність будь-яку клавішу для продовження...");
            _getch();
        }
        return; // Вихід з рекурсії
    }
    // Цикл по всіх можливих обсягах інвестицій для поточного заводу
    for (int i = 0; i < rows; i++)
    {
        invest_amount = table[i][0]; // Обсяг інвестицій
        profit_amount = table[i][current_factory]; // Відповідний пририст випуску продукції

        // Якщо поточні витрати плюс інвестиції не перевищують максимальний ресурс
        if (current_cost + invest_amount <= max_resource)
        {
            potential_remaining = get_upper_bound(table, current_factory + 1, total_factories, rows); // Знаходження теоретичної верхньої межі прибутку
            upper_bound = current_profit + profit_amount + potential_remaining;

            // Якщо верхня межа менша або дорівнює рекордному прибутку
            if (upper_bound <= RecordProfit)
            {
                continue; // Відрізання цієї гілки
            }

            // Оновлення поточного розподілу інвестицій
            CurrentAssignment[current_factory] = invest_amount;

            branch_and_bound(table, rows, total_factories, current_factory + 1, current_cost + invest_amount, current_profit + profit_amount, max_resource, step); // Рекурсивний виклик для наступного заводу
        }
    }
}