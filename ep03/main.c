#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Função para estimar PI usando o método de Monte Carlo
double monte_carlo_pi(int samples) {
    int inside_circle = 0;
    for (int i = 0; i < samples; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            inside_circle++;
        }
    }
    return 4.0 * inside_circle / samples;
}

int main() {
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    int samples = 1000000; // Número de amostras para a simulação de Monte Carlo
    pid_t pid = fork();

    if (pid == 0) {
        double pi_estimate = monte_carlo_pi(samples);
        printf("Processo filho: PI estimado = %f\n", pi_estimate);
        exit(0);
    } else if (pid > 0) {
        wait(NULL); // Espera o processo filho terminar
        printf("Processo pai: Processo filho terminou.\n");
    } else {
        fprintf(stderr, "Falha ao criar o processo filho.\n");
        return 1;
    }

    return 0;
}
