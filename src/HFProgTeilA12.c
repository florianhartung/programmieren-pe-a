/**
 * Programmentwurf A
 * Aufgabe 1, 2
 *
 * @author Florian Hartung (Matrikel Nr. 6622800)
 * @file HFProgTeilA12.c
 * @date 21.11.2022
 */

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "include/hashmap.h"
#include "include/list.h"

// Ermöglicht das einfache Testen von höheren Wertebereichen, für die eine 32bit Integer nicht mehr ausreichen würde
#define ulam_t unsigned long long

/**
 * Es wird zu einer Startzahl einer Ulamfolge berechnet und ausgegeben:
 *  - Die Länge der Ulamfolge
 *  - Die größte und drittgrößte Zahl jeweils mit ihrem Index (beginnend bei 0)
 *
 * @param start_number Die Startzahl der Ulamfolge
 */
void aufgabe1(int start_number);

/**
 * Tests für Aufgabe 1
 */
void test_aufgabe1(void);


void aufgabe2(int start_from, int start_to);

/**
 * Tests für Aufgabe 2
 */
void test_aufgabe2(void);

/**
 * Berechnung eines Ulamschrittes
 *
 * @return Falls i gerade: i/2<br> Andernfalls: 3*i+1
 */
ulam_t ulam(ulam_t i);

int main(void) {
    test_aufgabe1();
    test_aufgabe2();

    return 0;
}

void a1_insert_into_top3(ulam_t top3[3], int top3_indices[3], ulam_t new_value, int value_index) {
    ulam_t last_value;
    int last_index;
    int i;
    for (i = 0; i < 3; i++) {
        if (new_value > top3[i]) {
            last_value = top3[i];
            last_index = top3_indices[i];

            top3[i] = new_value;
            top3_indices[i] = value_index;

            i += 1;
            break;
        }
    }

    for (; i < 3; i++) {
        ulam_t tmp_value = top3[i];
        int tmp_index = top3_indices[i];

        top3[i] = last_value;
        top3_indices[i] = last_index;

        last_value = tmp_value;
        last_index = tmp_index;
    }
}

void aufgabe1(int start_number) {
    if (start_number < 100 || start_number >= 99999) {
        fprintf(stderr,
                "[Aufg1] Ungueltige Startzahl der Ulamfolge: %i\n"
                "\tErwartet: Eine positive 3- bis 5-stellige Ganzzahl\n",
                start_number);
        return;
    }

    int length = 1;
    ulam_t top3[3] = {0, 0, 0};
    int top3_indices[3];

    ulam_t current = start_number;
    do {
        a1_insert_into_top3(top3, top3_indices, current, length - 1);
        current = ulam(current);

        length += 1;
    } while (current != 1);
    printf("[Aufg1] Startzahl %i\n", start_number);
    printf("  Laenge: %i\n", length);
    printf("  Groesster Wert in Ulamfolge: %i mit Index %i\n", ((int) top3[0]), top3_indices[0]);
    printf("  Drittgroesster Wert in Ulamfolge: %i mit Index %i\n", ((int) top3[2]), top3_indices[2]);
}

void test_aufgabe1(void) {
    aufgabe1(3022);
    aufgabe1(30221);
}

bool a2_is_valid_range(int from, int to) {
    /*if (to < 1000 || to > 99999) {
        fprintf(stderr,
                "[Aufg2] Ungueltige Obergrenze des Bereichs: %i\n"
                "\tErwartet: Eine 4- bis 5-stellige Ganzzahl\n",
                to);
        return false;
    }*/

    if (from > to) {
        fprintf(stderr,
                "[Aufg2] Ungueltiger Bereich: [%i,%i]\n"
                "\tDie Untergrenze ist größer als die Obergrenze\n",
                from,
                to);
        return false;
    }

    if (to - from + 1 < 2022) {
        fprintf(stderr,
                "[Aufg2] Ungueltiger Bereich: [%i,%i]\n"
                "\tDer Bereich muss mindestens 2022 Elemente lang sein\n",
                from,
                to);
        return false;
    }

    if (from <= 0) {
        fprintf(stderr,
                "[Aufg2] Ungueltige Untergrenze des Bereichs: %i\n"
                "\tErwartet: eine positive Ganzzahl\n",
                from);
        return false;
    }

    return true;
}

int a2_compare_length_frequencies(const void* x1, const void* x2) {
    hashmap_entry_t* e1 = *(hashmap_entry_t**) x1;
    hashmap_entry_t* e2 = *(hashmap_entry_t**) x2;

    int e1_freq = (int) ((int*) e1->value);
    int e2_freq = (int) ((int*) e2->value);

    return e1_freq - e2_freq;
}

void aufgabe2(int start_from, int start_to) {
    if (!a2_is_valid_range(start_from, start_to)) {
        return;
    }

    hashmap_t* frequencies_map = init_hashmap();

    for (int start = start_from; start <= start_to; start++) {

        int length = 1;
        ulam_t u = start;
        do {
            u = ulam(u);
            length += 1;
        } while (u != 1);

        hashmap_put(frequencies_map, length, hashmap_get(frequencies_map, length) + 1);
    }

    list_t* length_frequencies = hashmap_into_entries(frequencies_map);
    free_hashmap(frequencies_map);


    qsort(length_frequencies->data,
          length_frequencies->length,
          sizeof(hashmap_entry_t*),
          a2_compare_length_frequencies);

    int top_n = 3;
    printf("Top %i Laengen:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        hashmap_entry_t* entry = list_get(length_frequencies, length_frequencies->length - 1 - i);
        printf("%i. Laenge %i -> %i mal\n", i + 1, entry->key, entry->value);
    }


    //free_list(length_frequencies);
}

void test_aufgabe2(void) {
    clock_t start = clock();
    aufgabe2(1, 9999999);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5fs\n", elapsed);
}

ulam_t ulam(ulam_t i) {
    return i & 1 ? 3 * i + 1 : i / 2;
}