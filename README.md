# Rapport lab03

## Général

- Auteur: Fabien Léger
- Cours: ASM, HEIG-VD
- Date: 31.03.2026

## Exercices

### Partie 1 - Directives

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