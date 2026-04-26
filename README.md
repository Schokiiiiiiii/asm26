# Lab04

## Général

- Auteur: Fabien Léger
- Cours: ASM, HEIG-VD
- Date: 14.04.2026

## Exercices

### 2a - Assembleur inline en C avec GCC

x86
```shell
=> go 0x40000
## Starting application at 0x00040000 ...
x86 :
eax = 0x0000000d ebx = 0x756e6547 ecx = 0x6c65746e edx = 0x49656e69
CPU ID String : "GenuineIntel"
## Application terminated, rc = 0x1
```

ARM
```shell
=> go 0x82000000
## Starting application at 0x82000000 ...
ARM
0x412fc0f1
Implementer  : ARM
Primary part : Cortex-A15
## Application terminated, rc = 0x1
```

En regardant avec la commande `cat /proc/cpuinfo`, on trouve les informations suivantes.

```shell
processor       : 0
vendor_id       : GenuineIntel
```

Pour le code en x86, on fait un appel à cpuid avant de donner au C les différentes valeurs. On obtient un string de 12
caractères des registres B, D et C qu'on peut ensuite afficher.

Pour le code en ARM, on lit le co-processor p15 et le mettons dans arm_cpuid. Nous avons maintenant sur les 8 premiers
bits l'implémentation et sur les bits 15-4 la partie principale qui est le cpu. On peut décrire le registre comme suit.

- bits 31-24 : implémentation
- bits 23-20 : variant
- bits 19-16 : architecture
- bits 15-4 : primary part
- bits 3-0 : révision

La commande en x86 semble avoir bien fonctionné ce qui est plutôt logique. Celle en ARM n'a cependant pas trouvé le bon
processeur. Il paraît que QEMU affiche un cpu ARM émulé de type Cortex-A15. Pour l'émulateur x86, il y a en tout cas un
cpu du même type.

### 2b - Assembleur « inline » en C avec GCC

popcnt instruction
```asm
Count = 0;
For (i=0; i < OperandSize; i++)
{   IF (SRC[ i] = 1) // i’th bit
    THEN Count++; FI;
}
DEST := Count;
```

lzcnt instruction
```text
LZCNT counts the number of leading most significant zero bits in a source operand (second operand) and returns
the result in the destination (first operand). LZCNT is an extension of the BSR instruction. The key difference
between the LZCNT and BSR instructions is that when the source operand is zero, LZCNT outputs the operand size
to the destination operand, whereas BSR leaves the destination operand unmodified.

On processors that do not support LZCNT, the instruction byte encoding is executed as BSR.
```

Comme je le comprends, l'instruction lzcnt compte le nombre de zéros pour les bits les plus significatifs jusqu'à
rencontrer un bit à 1.

### 3 - Compiler Explorer

#### a. Analyse de gpio_c.c (x86 32 bits)

* Utilisation de l’option `-m32` pour générer du code x86 32 bits.
* Traduction directe des opérations C en assembleur :
    * `&` → `andl`
    * `|` → `orl`
    * `/= 8` → `sarl` (décalage)
    * Affectation → `movl`
* À `-O0`, le code est très détaillé et suit ligne par ligne le C.

#### 3 b. Effet du mot-clé volatile

* Avec `volatile` :
    * Accès mémoire conservés.
    * Pas d’optimisation agressive.
* Sans `volatile` :
    * Optimisations importantes.
    * Fusion et simplification des opérations.
* Conclusion :
    * `volatile` est indispensable pour les registres matériels.

#### c. Passage en ARM et niveaux d’optimisation

* Correspondance des instructions :
    * x86 `movl` → ARM `mov`, `ldr`, `str`
    * x86 `andl` → ARM `and`
    * x86 `orl` → ARM `orr`
    * x86 `sarl` → ARM `asr`
* `-O0` :
    * Code long, nombreux accès mémoire.
* `-O3` :
    * Code optimisé, compact.
    * Utilisation des registres.
* Conclusion :
    * Plus d’optimisation = code plus efficace mais moins lisible.

#### d. Analyse de dot_product.c

* Code :
  `result += a[i] + b[i];`
* Fonctionnement assembleur :
    * Boucle avec compteur
    * Accès mémoire indexé
    * Addition
    * Accumulation
* Instructions typiques :
    * x86 : `movl`, `addl`, `cmpl`, `jl`
    * ARM : `ldr`, `add`, `cmp`, `blt`
* Remarque :
    * Ce n’est pas un vrai produit scalaire (pas de multiplication).

#### e. Analyse de asm.c

* `__asm__()` insère du code assembleur brut.
* Sans `\n\t` :
    * Instructions collées → erreur.
* Avec `\n\t` :
    * Instructions séparées correctement.
* Conclusion :
    * Le compilateur fait un remplacement textuel.

#### f. Analyse de cpuid.c

##### Contraintes

* `+m` : mémoire
* `+r` : registre
* `g` : choix automatique

##### Différences

* `+m` :
    * Accès mémoire supplémentaires
* `+r` :
    * Utilisation directe des registres
    * Code plus efficace

##### Clobbered registers

* Exemple :
  `"eax", "ebx", "ecx", "edx"`
* Rôle :
    * Indiquer les registres modifiés par l’ASM
* Sans clobber :
    * Risque d’erreurs (le compilateur fait de mauvaises hypothèses)

##### Conclusion

* Les contraintes définissent les entrées/sorties ASM.
* Les clobbers garantissent la cohérence du code généré.

### 4 - ARM Sliding Puzzle

#### Implémentation swap_asm()

