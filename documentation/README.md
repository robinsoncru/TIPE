# Implémentation des dames quantiques

Il y a un script bash pour compiler le code : `compile.sh`

__*Ne pas oublier de le mettre à jour à chaque nouvelle modif*__

Avant de l'exécuter, il faut donner les authorisation nécessaires
pour cela, tapper la commande suivante :
``` 
chmod u+x compile.sh
```
Lire `compile.sh` pour plus de détails

Sources :
- http://sylvain.archenault.free.fr/projets/Jeu_De_Dames/c/Jeu_de_Dames-C-Rapport.pdf (benofb)
___

Il y a un Makefile qui permet de ne compiler que les fichiers qui ont été modifiés:
Commande à taper dans le terminal, dans le répertoire TIPE/code:

make dames.out

Regles du jeu:

On ne peut lier d'amitie que deux amis au max, sinon le coup n'est pas joué
De même pour ennemie

Note:
J'ai (Victor G) reporté un bug très bizarre, je passe le pointeur Game *g à une fonction dont l'attribut currentTree est à null à une fonction et dans le corps de la
fonction, currentTree devient invalid adress, bug résolu en copiant-collant la fonction

Conseils de code:
- Executer toujours le compile.sh dans le dossier ou est le main sinon les inclusions des autres fichiers font planter les fonctions du module SDL_ttf (TTF_OpenFont)
La selection avec la souris ne permet pas de revenir en arriere, donc si on selectionne un pion coincé, le jeu est bloqué

Pour move back: on suppose que le joueur joue bien, donc quand on doit move arriere, il joue bien les fleches gauche/droite

GUIDE D'UTILISATION:
- clic gauche: sélectionne le pion
- clic droit (après un clic gauche): lie d'amitié les deux pions cliqués
- clic gauche (après un clic gauche): les deux pions cliqués sont ennemis si le pion sélectionné n'est pas une dame, sinon c'est un déplacement de la dame
- flèche gauche: le pion avance à gauche (une case avant, une case gauche)
- flèche droite: le pion avance à droite
- flèche avant: affiche l'arbre des rafles pour le pion sélectionné
- flèche avant (après flèche avant): le pion mange avec rafle
- escape: quitte le jeu
- p: promeut un pion, au hasard -> soit dame, soit reste le même pion soit devient un pion de la couleur opposé
- j: affiche l'état des pions instantanément dans le terminal
- h: affiche l'état du damier instantanément dans le terminal
- b: bidepl -> le pion se deplace sur les deux cases à la fois en avant à gauche et à droite, en divisant sa probabilité de présence par deux
- l: affiche l'état de la liste chainée qui contient les pions dans la probabilité de présence n'est pas certaine (!!= 1)
- a: lance le jeu automatique
- z: affiche l'arbre des rafles pour le pion sélectionné
- k: affiche l'état du damier instantanément dans le terminal (redondant avec h)
- r: Action la plus difficile, liste tout les coups que le joueur qui a le trait peut faire, puis les applique tous (dans un premier temps leur action déterministe, puis la structure mem permet d'appliquer toutes les issues possibles l'une après l'autre en les executant puis les annulant) puis il annule le coup et passe au suivant, tout cela sans voir les effets sur le plateau: ce sont les fonctions de minmax


BUG REPORT:
g->ind_move_back est modifié involontairement par passage aux fonctions


Todo list :
- Implementer l'IA:
-- Victor G: implémenter une heuristique

-- Victor D: aspect theorique de la gestion aléatoire

-- Victor G à l'adresse de Victor D: le tableau qui contient les différentes issues possibles du jeu est dans la structure memory_move_t dans l'instance issue, il 
contient un entier entre IND_GLORY_QUEEN, IND_BAD_MOVE, IND_NOTHING_HAPPENED pour promotion et sinon un entier représentant le numero d'une des positions possibles du
survivant dans le nuage de pions fantomes, toute associé avec leur pba.
Pour un coup déterministe, tu fais : coupAI, ..., cancelCoupAI
Pour un coup non déterministe, faire: issueCoup, (tu selectionne l'issue que tu veux avec index), coupAI(..., index), ..., cancelSelectedIssue, cancelCoupAI
Pour promotion: issuePromotion, (selectionne l'indice), promotionAI(..., index), ..., cancelIssuePromotion, cancelPromotion

- Etape 1 : (du 18/01/2024 9h au 01/02/2024 14h)
    - Titre
    - motivations
    - ancrage/lien au theme de l'annee
    - MCOT (Mise en Coherence des Objectifs du TIPE)
        - thematique/mots-cles (dames, quantique, jeu, strategie)
            - en francais + anglais
        - bibliogrphie commentee (intro du sujet du TIPE)
        - Problematique
        - Objectifs du TIPE
        - references bibliographiques
    - Declarer les participants du TIPE (Victor G + D)
- Presentation : (27/02/2024 9h au 11/06/2024 14h)
    - Upload du support de presentation
    - Saisie en lignr du DOT (Deroule Operationel du TIPE)
    - Eventuellement ajuster les positionnements/thematiques/mots-cles/biblios
