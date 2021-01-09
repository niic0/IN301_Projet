#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "avl_fct.h"
#include "text_to_tab.h" // Pour les messages d'erreurs

// Initialise un noeud
AVL *cree_noeud (char* mot) {
  AVL* A = NULL;
  A = malloc(sizeof(AVL));
  if (A == NULL) allocation_erreur();

  A->fg = NULL; A->fd = NULL;
  A->occ = 1;
  A->ligne = calloc(1, sizeof(int));

  A->mot = mot;

  return A;
}

// Libère la mémoire
void libere_memoire(AVL *A){
  if(A){
    libere_memoire(A->fg);
    libere_memoire(A->fd);
    free(A);
  }
}

// Trouve la hauteur d'un AVL récusrivement
int AVL_height (AVL *A) {
  int height_g = 0, height_d = 0;   // fils gauche, fils droit

  if (A->fg) height_g = AVL_height(A->fg);
  if (A->fd) height_d = AVL_height(A->fd);

  if (height_d > height_g) return ++height_d; // On incrémente avant le return
  else return ++height_g;                     // pour qu'il prenne la bonne val
}

// Calul le déséquilibre d'un ABR
int calcul_desequilibre (AVL *A) {
  int delta = 0;        // Variable qui stock le déséquilibre

  if(!A) return 0;

  if(A->fd) delta += AVL_height(A->fd);
  if(A->fg) delta -= AVL_height(A->fg);

  return delta;
}

    /////////////////
    /// Rotations ///
    /////////////////

AVL *rotation_droite (AVL *A) {
  AVL* result = NULL;

  AVL* tmp = A;
  result = A->fg;

  tmp->fg = result->fd;
  result->fd = tmp;

  return result;
}

AVL *rotation_gauche (AVL *A) {
  AVL* result = NULL;

  AVL* tmp    = A;
  result = tmp->fd;

  tmp->fd    = result->fg;
  result->fg = tmp;

  return result;
}

// Equillibre l'arbre pour qu'il devienne un AVL
AVL *equilibre_noeud (AVL *A) {

  int diff = calcul_desequilibre(A); // diff = déséquilibre

  if (diff >= 2) {
    if (calcul_desequilibre(A->fd) <= -1) {
      A->fd = rotation_droite(A->fd);
      A = rotation_gauche(A);
    }
    else if (calcul_desequilibre(A->fd) >= 1)
      A = rotation_gauche(A);
  }

  else if (diff <= -2) {
    if (calcul_desequilibre(A->fg) >= 1) {
      A->fg = rotation_gauche(A->fg);
      A = rotation_droite(A);
    }
    else if (calcul_desequilibre(A->fg) <= -1)
      A = rotation_droite(A);
  }

  return A;
}

// Renvoie un mot aléatoire parmis les noeud du tableau
char *mot_alea (AVL *A) {
  int H = AVL_height(A);
  int val_alea = rand()%H+1;

  if (val_alea == 1)
    return A->mot;

  if (A->fg && (val_alea<(H/2)))
    return mot_alea(A->fg);
  if (A->fd && (val_alea > (H/2)))
    return mot_alea(A->fd);

  return A->mot;
}

          /////////////////
          /// Affichage ///
          /////////////////

void affichage_avl (AVL* A, char* text) {
  char* T = char_to_tab (text);
  int* L = first_char (text);

  printf("\nOccurence du mot %s : %d\n\n",A->mot,A->occ);
  for (int i=0 ; i<A->occ ; i++) {
    if (A->ligne[i] != A->ligne[i+1]){
      printf("Ligne %d : ",A->ligne[i]);
      for (int k=L[A->ligne[i]] ; k<L[A->ligne[i]+1] ; k++)
        printf("%c",T[k]);
    }
  }

}

void affichage_chrono (AVL* A) {

}