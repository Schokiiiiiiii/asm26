# Lab01

## Général

- Auteur: Fabien Léger
- Cours: ASM, HEIG-VD
- Date: 24.02.2026

## Exercices

### Exercice 3

#### Quelle différence y a-t-il entre sizeof et strlen ? Quelle différence entre l’utilisation de char * ou de char[] 

- sizeof(...) - mesure la taille de stockage d'une expression ou d'un type
- strlen(const char *s) - calcule la taille d'un string pointé par s
- char *s - s est un pointeur sur un caractère qui est une chaîne immutable
- char[] s - s est un array avec une chaîne comme contenu donc il a la propriété et peut le modifier

#### Quelles données se trouvent sur la pile (stack) et sur le tas (heap) ? Regarder leur adresse.

````c++
char string[] = "Bonjour de main!"; // stack
char *alloc = allocate_string(string, strlen(string)); // heap
````

En effet, on peut trouver les informations suivantes.

- 00005555555552fc:   mov     %rax,%rdi / début du code
- string char [17]	0x7fffffffd7e0
- alloc	 char *	    0x5555555596b0 "Bonjour de main!"

`alloc` étant proche du main, on comprend qu'il est sur la heap.

`string` étant loin du main, on comprend qu'il est sur la stack.

#### Comment pourriez-vous tester le code afin de vous assurer que le caractère NULL soit bien copié ?

En faisant le pas à pas, on découvre les informations suivantes.

```asm
000055555555528e:   movb    $0x0,(%rax)
0000555555555291:   mov     -0x8(%rbp),%rax
```

On met donc bien dans RAX le caractère nul avant de remettre RAX dans notre RBP.

On remet l'adresse de alloc dans RAX car c'est le registre pour retourner.

#### A l’aide de la vue mémoire (Window>Show View>Memory), inspecter le contenu - en hexadécimal et en ASCII - du tableau copié en donnant l’adresse de ce tableau (préfixer l’adresse avec « 0x »).

- Adresse: 0x5555555596b0
- Hexadécimal: 426F6E6A 6F757220 6465206D 61696E21 00
- ASCII: `bonj` `our ` `de m` `ain!` `\0`

### Exercice 4

#### Quelles instructions assembleur correspondent à « return 0 ; » ?

```asm
1328:       b8 00 00 00 00          mov    $0x0,%eax
# ...
1341:       c9                      leave
1342:       c3                      ret
```

- mettre la valeur de retour dans EAX (RAX vaut également 0), car registre de retour
- leave + ret pour quitter main

#### Utilisation des commandes

```shell
readelf -h exo1
readelf -a exo1
```

#### Options

- objdump -d - montre le contenu assembleur des sections exécutables
- objdump -D - montre le contenu assembleur de toutes les sections
- readelf -h - montre le file header du ELF
- readelf -a - 'all', montre un nombre étendu d'informations/contenu regroupant plusieurs autres commandes

### Exercice 7

#### Exécuter les programmes dans Eclipse et comparer les résultats obtenus.

```shell
# -O0
p[0] = 1
p[1] = 2
p[2] = 3
p[3] = 4
```

```shell
# -O3
p[0] = 0
p[1] = 0
p[2] = 0
p[3] = 0
```

On voit donc que l'optimisation semble enlever du code au point de faire fonctionner le code de manière inattendue.

#### Quel(s) est(sont) les différences au niveau du code C entre exo4-debug et exo4-optimized ?

Il n'y a pas de différences au niveau du code C directement dans le code. Cependant, lors de la compilation, il semble
que le code obtenu pour optimized soit plus ressemblant à celui-ci.

```c++
int main(int argc, char** argv){
    for (int i = 0; i < 4; ++i) {
        printf("p[%d] = %d\n", i, p[i]);
    }
    return 0;
}
```

#### Exécuter chaque programme pas à pas en ouvrant le code désassemblé (vous pouvez aussi utiliser les outils vus à l’étape 4. Quel(s) est(sont) les différences d’exécution que vous observez ? Qu’observez-vous au niveau du code assembleur ?

Comme dit au-dessus, on perd la boucle dans laquelle on crée puis attribue le tableau à notre pointeur. Cela veut dire
qu'au niveau du code assembleur, l'optimized est beaucoup plus petit.

- O0 - chaque instruction est écrite en assembleur
- O3 - il reste seulement l'entrée-sortie du main et la boucle for d'affichage

#### Pouvez-vous trouver une explication pour le comportement que vous voyez ? 

La création du buffer dans le if est une variable locale à ce bloc de code. Cela signifie que lorsqu'on sort du if, le
buffer n'existe théoriquement plus et le comportement est indéfini. Le compilateur enlève donc cette partie car buf
n'est pas utilisé dans son bloc de code.

Sans optimisation, la zone mémoire n'a par chance toujours pas été changée. On peut donc toujours la lire de manière
juste sans problèmes.

Avec optimisation, le code a été enlevé car inutile et donc on lit un autre endroit avec des 0.

#### Quel est le bug de ce programme ?

Le programme marche à nouveau si on fait du code de la forme suivante dans le if de la première boucle.

```c++
p = malloc(sizeof(int) * 4);
p[0] = 1;
p[1] = 2;
p[2] = 3;
p[3] = 4;
```

Ainsi, cela se trouve sur la heap et il n'y aura donc pas de problèmes de variables locales.