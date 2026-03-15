# Labo 2

## Général

- Auteur: Fabien Léger
- Cours: ASM, HEIG-VD
- Date: 10.03.2026

## Exercices

### Exercice 3

#### Analyse statique du binaire

On arrive bien à retrouver la fonction check_password avec la commande suivante.

```shell
arm-linux-gnueabihf-objdump -d crackme # -d used to recover symbols
```

```asm
82000010 <check_password>:
82000010:       e52db004        push    {fp}            @ (str fp, [sp, #-4]!)
82000014:       e28db000        add     fp, sp, #0
// ...
```

Cette fonction est utilisée dans main pour tester un mot de passe.

```asm
8200018c:       ebffff9f        bl      82000010 <check_password>
82000190:       e1a03000        mov     r3, r0
82000194:       e3530000        cmp     r3, #0
82000198:       0a000007        beq     820001bc <main+0x12c>
8200019c:       e300038c        movw    r0, #908        @ 0x38c
```

Où on remarque bien que l'instruction de saut conditionnel est celle-ci.

```asm
82000198:       0a000007        beq     820001bc <main+0x12c>
```

Dans le cas où elle est égale, elle nous amène sur le chemin d'échec alors que si elle n'est pas égale, on reste sur
le chemin avec l'indication de succès.

On peut donc noter les informations suivantes :
- Adresse instruction de saut - 0x82000198
- Taille - 4 octets

#### Bypass du check

On peut utiliser les instructions md et mw pour écrire sur la mémoire et bypass le check du mot de passe.

```shell
md 0x82000198 1
mw.l 0x82000198 0x00000000
md 0x82000198 1
```

On écrit ainsi une instruction NOP de 32 bits à la place du contrôle ce qui nous fait toujours aller vers le succès.

