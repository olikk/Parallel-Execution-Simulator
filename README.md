TER M1 SIL

"Simulation d’exécution de programmes parallèle"

Le but de mon travail d’étude et de recherche était de simuler l’exécution de programmes parallèles et d’avoir la possibilité de détecter l’interblocage. Dans son cadre un langage de programmation parallèle a été créé basé sur le langage déjà existant X10 mais seulement avec les instructions nécessaires pour la suite de la recherche.
Pour cela l’analyseur lexical et l’analyseur syntaxique étaient implémentés avec comme résultat un arbre de syntaxe abstrait. Le code intermédiaire est généré en parcourant un AST. La machine virtuelle séquentielle réalisée, parcours pas à pas ce code intermédiaire et est capable d’afficher l’état global de chaque activité avec l’état d’horloges créées.
Grâce à ce travail il est possible maintenant de détecter un interblocage et analyser l’état de la machine virtuelle pour savoir les horloges et les activités qui ont provoqué cet interblocage.
Pour aller plus loin, on pourrait faire une visualisation interactive de l’état d’exécution d’un programme parallèle.
