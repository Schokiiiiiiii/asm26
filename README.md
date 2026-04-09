# Rapport lab03

## Général

- Auteur: Fabien Léger
- Cours: ASM, HEIG-VD
- Date: 31.03.2026

## Exercices

### Partie 1 - directives

> Rendre la fonction to_upper_case() visible depuis le C.

```asm
@ Show upper case to main
.global to_upper_case
```

> Déclarer les symboles externes nécessaires.

```asm
@ Get variables from outside
.extern ascii_a
.extern ascii_z
.extern to_upper_sub
```

> Inclure les définitions du fichier string_asm.inc.

```asm
@ Include external files
.include "string_asm.inc"
```

> Définir MAX_STRING_SIZE avec la valeur 30.

```asm
@ Constants
.equiv MAX_STRING_SIZE, 30
```

> Placer le code dans .text et les données dans .data.

```asm
@ Variables
.data

@ Code
.text
```

### Partie 2 - gpio_fields

> Test avec main.bin

```shell
=> go 0x40000
## Starting application at 0x00040000 ...
Same result with assembler and C
Test 1: PASSED
Test 2: PASSED
Hit any key to exit ... 
```

> Comparaison désassemblage

gpio_asm.o désassemblé
```asm
   7:   25 80 78 ff ff          and    $0xffff7880,%eax
   c:   83 c8 02                or     $0x2,%eax
   f:   83 c8 24                or     $0x24,%eax
  12:   83 c8 40                or     $0x40,%eax
  15:   0d 00 05 00 00          or     $0x500,%eax
  1a:   0d 00 80 00 00          or     $0x8000,%eax
```

gpio_c.o désassemblé
````asm
   3:   83 65 08 fc             andl   $0xfffffffc,0x8(%ebp)
   7:   83 65 08 c3             andl   $0xffffffc3,0x8(%ebp)
   b:   83 65 08 bf             andl   $0xffffffbf,0x8(%ebp)
   f:   81 65 08 ff f8 ff ff    andl   $0xfffff8ff,0x8(%ebp)
  16:   81 65 08 ff 7f ff ff    andl   $0xffff7fff,0x8(%ebp)
  1d:   83 4d 08 02             orl    $0x2,0x8(%ebp)
  21:   83 4d 08 24             orl    $0x24,0x8(%ebp)
  25:   83 4d 08 40             orl    $0x40,0x8(%ebp)
  29:   81 4d 08 00 05 00 00    orl    $0x500,0x8(%ebp)
  30:   81 4d 08 00 80 00 00    orl    $0x8000,0x8(%ebp)
````

On remarque donc que le code écrit pr nous même en assembleur est un poil plus efficace vu qu'on a utilisé des
constantes, mais les deux codes font exactement la même chose au final.

### Partie 3 - frame

> Test avec main.bin

````shell
=> go 0x40000
## Starting application at 0x00040000 ...
result : 0
checksum to hex: 0x30
Test 1: PASSED
Test 2: PASSED
Test 3: PASSED
Test 4: PASSED
Test 5: PASSED
print 30784146
Test 6: PASSED
Hit any key to exit ... 
````