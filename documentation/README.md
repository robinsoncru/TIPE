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

Conseils de code:
- Executer toujours le compile.sh dans le dossier ou est le main sinon les inclusions des autres fichiers font planter les fonctions du module SDL_ttf (TTF_OpenFont)
- Virer VOID_INDEX ou NEUTRAL_INDICE c'est redondant


Todo list :
- Finir l'implémentation des rafles
- Ajouters des nouvelles règles :
    - [à compléter]
- [à compléter]