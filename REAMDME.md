* Introduction

Ce répertoire est une version simple de Tetris codé en c++ avec les libraires Eigen et SFML fait en projet personnel. Vous pouvez voir dans l'image ci-dessous le rendu final.
![Tetris](img/tetris_demo.png?raw=true "Démo")

* Installation

Le code utilise les librairies Eigen et SFML qui sont donc necessaires pour installer ce logiciel. Pour ce faire, suivez les instructions fournit sur les sites de ses libraires : 
[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page), [SFML](https://www.sfml-dev.org/).

Puis il est nécessaire d'indiquer au Makefile la position de ses librairies soit en les rajoutant dans /usr/local/include soit en indiquant les chemins dans les variables EIGEN_PATH et SFML_PATH sous la forme : 

```bash
-I /chemin/vers/la/librairie/
```

Pour installer le jeu allez dans le répertoire src et faites :
```bash
make
```

Pour lancer le jeu :
```bash
./tetris
```





