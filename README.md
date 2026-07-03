# ft_ssl :

## Partie 1 : Implementation de l'algorithme de hash MD5 :

### Bon a savoir avant de commencer : 

- La fonction de hash MD5 a ete creee par Ronald Rvest en 1991 et remplace MD4. 
- Elle peut servir de checksum pour s'assurer de l'intregrite d'un fichier ou de data.
- C'est un algo dit de "message digest" => en gros, qu'importe la taille du fichier d'entree ou de l'input, la taille de l'output est fixe. (ici 128 bits).
- Il n'est plus considere comme safe depuis 2004, des travaux ont permis de mettre en lumiere que MD5 n'est pas"collision resistant" (https://fr.wikipedia.org/wiki/R%C3%A9sistance_aux_collisions)

### 1ere etape :
#### Le padding :

MD5 ne travaille jamais directement sur le message, il travaille sur des blocs de 512 bits (64 octets).

Donc quel que soit le message : 
`"abc"`
ou 
`"42 is nice"`

Il faut le transformer pour que sa taille soit un multiple de 64 octets.

Comment on fait le padding dans ce cas ? 

1. Ajouter un bit `1`
2. Ajouter autant de bits `0` que necessaire
3. Terminer par la longueur originale du message codee sur 64 bits en LITTLE ENDIAN !

Exemple :

```
"abc"
```

devient : 

```
61 62 63
80
00 00 00 ...
...
18 00 00 00 00 00 00 00
```
Pourquoi ? 
```
abc = 3 octets
3 * 8 = 24 bits
24 = 0x18 soit, la taille du message

La longueur ajoutee est celle du message AVANT PADDING.
```

#### Le little-endian :

MD5 impose : 
- lecture des "mots" de 32 bits en little-endian
- ecriture de la longueur en little-endian
- affichage du digest final en little-endian

Exemple :
```
01 02 03 04 (4 octets)
```
MD5 interpretera : 
```
0x04030201
```
et pas :
```
0x01020304
```
C'est a ca que sert la fonction `read_word_le()`

#### Les quatres registres :
MD5 possede en interne 4 "states" :
```
A
B
C
D
```
Init via des constantes : 
```
A = 0x67452301
B = 0xefcdab89
C = 0x98badcfe
D = 0x10325476
```
C'est l'etat courant du hash, ou il en est en gros. Au fur et a mesure que les blocs sont traites, ils evoluent : On parle de feed-forward
En gros : 
A la fin du traitement d'un bloc, les valeurs calculees sont additionnees a l'etat precedent. Cette etape, appelee feed-forward, permet de chainer les blocs entre eux et d'eviter qu'un bloc remplace simplement le resultat du precedent. 
```
A = A + a
B = B + b
C = C + c
D = D + d
```
Ca nous permet de ne pas ecraser simplement le bloc precedent mais de faire evoluer son etat. 

#### Les 64 operations :
Chaque bloc subit 64 operations reparties comme suit : 
```
Bloc 1 - 16 operations - F(x, y, z) = (x & y) | (~x & z) => Choisit les bits de Y ou Z suivant X
Bloc 2 - 16 operations - G(x, y , z) = (x & z) | (y & ~z) => Meme idee que bloc 1, differente combinaison
Bloc 3 - 16 operations - H(x, y, z) = x ^ y ^ z => Simple XOR
Bloc 4 - 16 operations - I(x, y, z) = y ^ (x | ~z) => derniere transformation
```

#### Les constantes K :
Il existe 
```
64 constanstes
K[64] dans le code
Calculees par : 
floor(2^32 x abs(sin(i)))
Pour : i = 1 ... 64
```
Pourquoi? 
Pour disposer de constantes : Publiques, Deterministes et difficile a manipuler volontairement.
Dans mon implementation j'ai opte pour des constantes declarees dans un tableau pour ne pas avoir a les calculer (math.h p-e pas autorise par le sujet, j'ai pas pris le risque).

#### Les rotations :

Contrairement a d'autres algo, MD5 utilise des rotations circulaires :

```
10010000
```
rotation gauche de 2
```
01000010
```
les bits qui sortent a gauche reviennent a droite.

#### Les additions modulo 2^32 :
```
0xfffffffff + 1 => 0x000000000
```
L'overflow est volontaire et fait partie de l'implementation de cet algo. On utilise `uint32_t` pour obtenir "naturellement" ce comportement. 

#### L'ordre des mots :
Le bloc de 64 octets est decoupe comme suit : 
```
M0
M1
M2
M3
...
M15
```
Mais il ne sont pas consommes dans le meme ordre en fonction du round.
(J'ajouterai un joli dessin si j'y pense, chiant a schematiser en MD)

#### MAJ des registres :

A chaque operation les registres changent de role. Pourquoi ? pour permettre de faire circuler les informations entre les 4 registres au fil des 64 operations. 

```
Avant: 
A B C D

Apres:
A <- D
D <- C
C <- B
B <- B + rotate()
```

#### Digest Final :
A la fin du traitement de tous les blocs, les quatres registres sont concatenes : 
```
A || B || C || D
```
Chaque registre est ecrit en little endian, ce qui produit une empreinte de 128 bits, generalement affichee sous forme de 32 caracteres hexa. 
Exemple : 
```
MD5("abc")
900150983cd24fb0d6963f7d28e17f72
```


Sources : 

 - https://en.wikipedia.org/wiki/MD5
 - https://www.ietf.org/rfc/rfc1321.txt
 - https://www.johndcook.com/blog/2024/03/20/md5-hash-collision/
