#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    int id;
    char nom[50];
    int quantite;
    float prix;
} Article;

// Prototypes
void ajouterArticle(Article** stock, int* taille, int* capacite);
void afficherStock(const Article* stock, int taille);
int rechercherArticle(const Article* stock, int taille, int id);
void libererMemoire(Article* stock);
int lireChoix(void); // lecture robuste d’un entier

// ===== Programme principal =====
int main(void) {
    Article* stock = malloc(2 * sizeof(Article));
    if (stock == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return EXIT_FAILURE;
    }

    int capacite = 2;
    int taille = 0;
    int choix, id;

    printf("======= INTERFACE =======\n");

    do {
        printf("\n1. Ajouter un article\n");
        printf("2. Afficher le stock\n");
        printf("3. Rechercher un article\n");
        printf("4. Quitter\n");
        printf("Choix : ");

        choix = lireChoix();
        if (choix == -1) continue;

        switch (choix) {
            case 1:
                ajouterArticle(&stock, &taille, &capacite);
                break;
            case 2:
                afficherStock(stock, taille);
                break;
            case 3: {
                printf("Entrez l'ID : ");
                id = lireChoix(); // utilisation de la fonction robuste
                if (id == -1) break;
                int index = rechercherArticle(stock, taille, id);
                if (index != -1)
                    printf("Article trouvé : %s\n", stock[index].nom);
                else
                    fprintf(stderr, "Article introuvable.\n");
                break;
            }
            case 4:
                libererMemoire(stock);
                printf("Au revoir !\n");
                break;
            default:
                fprintf(stderr, "Choix invalide.\n");
        }
    } while (choix != 4);

    return EXIT_SUCCESS;
}

// ===== Ajouter un article =====
void ajouterArticle(Article** stock, int* taille, int* capacite) {
    if (*taille >= *capacite) {
        *capacite *= 2;
        *stock = realloc(*stock, (*capacite) * sizeof(Article));
        if (*stock == NULL) {
            fprintf(stderr, "Erreur de réallocation mémoire.\n");
            exit(EXIT_FAILURE);
        }
    }

    Article a;
    printf("ID : ");
    a.id = lireChoix();
    if (a.id == -1) return;

    printf("Nom : ");
    if (scanf("%49s", a.nom) != 1) {
        fprintf(stderr, "Nom invalide.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Quantité : ");
    a.quantite = lireChoix();
    if (a.quantite == -1) return;

    printf("Prix : ");
    if (scanf("%f", &a.prix) != 1) {
        fprintf(stderr, "Prix invalide.\n");
        while (getchar() != '\n');
        return;
    }

    (*stock)[*taille] = a;
    (*taille)++;
}

// ===== Afficher le stock =====
void afficherStock(const Article* stock, int taille) {
    printf("\n--- STOCK ---\n");
    if (taille == 0) {
        printf("Aucun article en stock.\n");
        return;
    }
    int i;
    for (i = 0; i < taille; i++) {
        printf("ID: %d | Nom: %s | Quantité: %d | Prix: %.2f\n",
               stock[i].id, stock[i].nom, stock[i].quantite, stock[i].prix);
    }
}

// ===== Rechercher un article =====
int rechercherArticle(const Article* stock, int taille, int id) {
	int i;
    for (i = 0; i < taille; i++) {
        if (stock[i].id == id)
            return i;
    }
    return -1;
}

// ===== Libérer la mémoire =====
void libererMemoire(Article* stock) {
    free(stock);
}

// ====lire un entier =====

int lireChoix(void) {
    char buffer[100];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Erreur de lecture.\n");
        return -1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    int uniquementEspaces = 1;
    int i;
    for (i = 0; buffer[i] != '\0'; i++) {
        if (!isspace((unsigned char)buffer[i])) {
            uniquementEspaces = 0;
            break;
        }
    }
    if (uniquementEspaces) {
        fprintf(stderr, "Entrer une Valeur Numerique !! Espaces ou Tabulation Interdit !!.\n");
        return -1;
    }

    int valeur = (int)strtol(buffer, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Entrer invalide !! Non Numeriques.\n");
        return -1;
    }

    return valeur;
}

