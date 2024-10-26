#include <stdio.h>
#include <stdlib.h>

#define MAX_BOND_NAME 64
#define MAX_PRICE_REPR 16
#define BOND_PRICE 1000
#define DELTA_DAYS 30
#define BATCH_SIZE 1000

typedef struct {
    unsigned int day;
    char *bond_name;
    char *price_repr;
    double price;
    unsigned int quantity;
    double cost;
    unsigned int coupon_income;
    double profit;
} bond_t;

int cmp_profit(const void *p1, const void *p2) {
   return ((bond_t*)p2)->profit - ((bond_t*)p1)->profit;
}

unsigned int read_bonds(bond_t *bonds, unsigned int batch_size, int n_days) {
    int i;
    int status;
    for(i = 0; i < batch_size; i ++) {
        bonds[i].bond_name = calloc(MAX_BOND_NAME, sizeof(char));
        bonds[i].price_repr = calloc(MAX_PRICE_REPR, sizeof(char));
        if((status = scanf("%d %s %s %d", &bonds[i].day, bonds[i].bond_name, bonds[i].price_repr, &bonds[i].quantity)) < 0) {
            if(status == EOF) {
                break;
            }
            perror("scanf");
            exit(1);
        }
        bonds[i].price = strtod(bonds[i].price_repr, NULL);
        bonds[i].cost = bonds[i].price * bonds[i].quantity * BOND_PRICE / 100.0;
        bonds[i].coupon_income = (n_days - bonds[i].day + DELTA_DAYS) * bonds[i].quantity;
        bonds[i].profit = (BOND_PRICE * bonds[i].quantity) - bonds[i].cost + bonds[i].coupon_income;
    }
    return i;
}

int main(int argc, const char * argv[]) {
    unsigned int m_bonds, s_cash, n_days;
    int status;
    if((status = scanf("%d %d %d", &n_days, &m_bonds, &s_cash)) < 0) {
        perror("scanf");
        exit(1);
    }
    bond_t *bonds = malloc(BATCH_SIZE * 2 * sizeof(bond_t));
    
    unsigned int total_bonds = read_bonds(bonds, BATCH_SIZE, n_days);
    unsigned int more_bonds = 0;
    qsort(bonds, total_bonds, sizeof(bond_t), cmp_profit);
    
    for(unsigned int i = BATCH_SIZE; i < n_days * m_bonds; i += BATCH_SIZE) {
        more_bonds = read_bonds(bonds + sizeof(bond_t) * total_bonds, BATCH_SIZE, n_days);
        qsort(bonds, total_bonds + more_bonds, sizeof(bond_t), cmp_profit);
        if(BATCH_SIZE > total_bonds + more_bonds) {
            total_bonds += more_bonds;
            more_bonds = 0;
        }else {
            more_bonds = total_bonds + more_bonds - BATCH_SIZE;
            total_bonds = BATCH_SIZE;
        }
    }
    
    double cash_left = (double)s_cash;
    double profit = 0.0;
    int top_n_bonds = 0;
    for(top_n_bonds = 0; cash_left > 0 && top_n_bonds < total_bonds; top_n_bonds ++) {
        if(bonds[top_n_bonds].cost > cash_left) {
            break;
        }
        cash_left -= bonds[top_n_bonds].cost;
        profit += bonds[top_n_bonds].profit;
    }
    printf("%d\n", (unsigned int)profit);
    for(int i = 0; i < top_n_bonds; i ++) {
        printf("%d %s %s %d\n", bonds[i].day, bonds[i].bond_name, bonds[i].price_repr, bonds[i].quantity);
    }
    printf("\n");
    
    for(int i = 0; i < total_bonds + more_bonds; i ++) {
        free(bonds[i].bond_name);
        free(bonds[i].price_repr);
    }
    free(bonds);
    return 0;
}
