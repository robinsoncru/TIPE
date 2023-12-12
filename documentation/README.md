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

Regles du jeu:

On ne peut lier d'amitie que deux amis au max, sinon le coup n'est pas joué
De même pour ennemie

Conseils de code:
- Executer toujours le compile.sh dans le dossier ou est le main sinon les inclusions des autres fichiers font planter les fonctions du module SDL_ttf (TTF_OpenFont)
- Virer VOID_INDEX ou NEUTRAL_INDICE c'est redondant
La selection avec la souris ne permet pas de revenir en arriere, donc si on selectionne un pion coincé, le jeu est bloqué

INFOS IMPORTANTES pour Victor D:
Victor D fais gaffe aux fonctions suivantes:
-pawn_default_value et pawn_default_value_new la derniere est plus facile a utiliser (parametres plus simples, plus generaux) alors si on peut se passer de la premiere c'est mieux sinon on la garde.
-Killpawn a aussi ses parametres modifiés (plus simples)donc il faudra que tu l'update dans tes structures si tu l'utilise, si c'est trop dur on réimplemente aussi l'ancienne et on met l'autre en new'
-Change_pawn_place et change_pawn_place_new j'ai gardé l'ancienne car tu l'utilise dans nonLoggingChangeForEat mais l'autre a des paramètres plus simples a utiliser dans 
le code sera plus libre

Les fonctions suivantes permettent de modifier le pion dans le game juste en le reconnaissant avec son indice et sa couleur:
-put_pawn_value
-get_pawn_value
Victor D jettes y un oeil

Pour move back: on suppose que le joueur joue bien, donc quand on doit move arriere, il joue bien les fleches gauche/droite

Todo list :
- Finir l'implémentation des rafles
- Ajouters des nouvelles règles :
    - [à compléter]
- [à compléter]

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