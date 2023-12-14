# REV-Party - République Expérimentale Virtuelle

## Description du Projet

Le projet universitaire REV-Party (République Expérimentale Virtuelle) a pour objectif de déterminer un vainqueur en utilisant différentes méthodes de scrutin. Le développement est réalisé en équipe, et le langage de programmation utilisé est le C.

## Méthodes de Vote Implémentées

Le projet prend en charge plusieurs méthodes de scrutin, offrant ainsi une variété de façons de déterminer le vainqueur. Les méthodes de vote actuellement implémentées sont :

1. **Uninominal 1**
2. **Uninominal 2**
3. **Condorcet-Minimax**
4. **Condorcet-Rangement des Paires (Ranked Pairs)**
5. **Schulze**
6. **Jugement Majoritaire**

## Utilisation du Programme

Le programme prend en entrée un fichier CSV qui contient les données de vote. Ce fichier est passé en paramètre lors de l'exécution du programme. Les résultats sont ensuite calculés selon la méthode de vote choisie.

## Instructions d'Exécution

Pour exécuter le programme avec un fichier CSV en tant que source, utilisez la commande suivante :

```bash
./rev <méthode> <source> [-o <log_file>]
```

La méthode doit être renseigné avec la balise -m,
La source doit être renseigné avec la balise -d, -i ou -j selon le contenu du .csv (respectivement si il s'agit d'une matrice de duel, d'un ballot contenant des notes ou un classement pour chaques candidats).
Le programme prend en charge la conversion automatique de ballot à duel **mais pas l'inverse**.
Le type de source est choisie selon la méthode indiquée, e.g. la méthode du jugement majoritaire prend un ballot en entrée pas une matrice de duel.

# Exemple d'utilisation
```bash
./rev -m all -i bale_1.csv -o trace.log
```