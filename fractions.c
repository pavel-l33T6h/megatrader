#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    unsigned int total;
    scanf("%d", &total);
    double *buf = malloc(total * sizeof(double));
    double sum = 0.0;
    double c = 0.0;
    for(unsigned int i = 0; i < total; i ++) {
        scanf("%lf", &buf[i]);
        double y = buf[i] - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    for(unsigned int i = 0; i < total; i ++) {
        printf("%.3lf\n", buf[i] / sum);
    }
    free(buf);
    return 0;
}
