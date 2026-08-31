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
# Partie 2 : SHA256

### Bon a savoir avant de commencer : 

1. `SHA-256` fait partie de la famille `SHA-2` publiee par la NSA en 2001.
2. Tout comme `MD5` il produit un digest qui prend une entree de taille "libre" et produit une sortie fixe, `256 bits (64 caracteres hexa)`
3. A ce jour, il est considere comme sur, pas encore de collision demontree
4. Travaille egalement par bloc de `512 bits (64 octets)`, decoupage en blocs dans l'esprit `MD5`.

### Les etapes et differences avec MD5 : 

**Le padding**  
Exactement la meme chose que `MD5` dans l'esprit, dans les faits, `SHA-256` travaille lui en **big-endian**.  

**Les huits registres** : `MD5` a 4 states, la ou `SHA-256` en a 8 :  
`a b c d e f g h`  
Initialise egalement via des constantes (h0 a h7) :  
```
h0 = 0x6a09e667
h1 = 0xbb67ae85
h2 = 0x3c6ef372
h3 = 0xa54ff53a
h4 = 0x510e527f
h5 = 0x9b05688c
h6 = 0x1f83d9ab
h7 = 0x5be0cd19
```
Ces valeurs viennent de la partie fractionnaires des racines carrees des 8 premiers nombre premiers (2,3,5,7,11,13,17,19). C'est la meme logique pour `MD5` avec `sin()`, des valeurs publiques, deterministes.  

Comme pour `MD5` : il y a feed-forward a la fin de chaque bloc.  

**Le message schedule (c'est la grosse diff entre MD5 et SHA-256)** :  
`MD5` utilise directment les 16 mots du bloc la ou `SHA-256` fait un travail de preparation : il etend les *16 mots* du bloc en *64 mots*.  
L'avantage est que ce fait circuler de l'info avant meme d'entrer dans la boucle de compression.  

**Les fonctions internes** utilise les meme fonctions a chaque round, la ou `MD5`change tous les 16 tours mais il y a plus de fonctions et **brassent plus de bits a la fois.**  

**La boucle de compression** : 64 tours. Meme idee que MD5, les registres tournent et s'echangent a tour de role mais passent par le meme type de tour en boucle, la ou MD5 switch au lieu de faire 4 blocs de 16 tours avec une autre fonction a chaque fois.


# A retenir : 

**Pourquoi on ne peut pas "reverse" un hash :**  

1. *Perte d'info irreversible* : sortie de taille fixe pour une entree arbitraire => plusieurs entrees sont possible pour une meme sortie, aucun moyen de savoir laquelle est la bonne
2. *Operations non-bijectives (a vos souhaits)* : additions `modulo 2^32`, `f* ch(), maj(), f(), g() etc...` ne sont pas reversibles individuellement : remonter le calcul fait exploser le nombre de solutions possibles a chaque tour au lieu de reduire.
3. *Effet avalanche (SCA)* : un bit change en entree modifie =~50% des bits de sortie. Aucune structure algebrique exploitable.
4. *Une option : Le Bruteforce* : essayer une liste de candidats et comparer les hashs (attaque par dictionnaire).

Sources : 

 - [Wikipedia](https://en.wikipedia.org/wiki/MD5)
 - [RFC Doc](https://www.ietf.org/rfc/rfc1321.txt)
 - [Blog John D Cook](https://www.johndcook.com/blog/2024/03/20/md5-hash-collision/)
 - [RareSkills - Youtube](https://youtu.be/5MiMK45gkTY?si=IsytVJfBzd6272Dd)
- [TheCodingGopher - Youtube](https://youtu.be/R_mOWu3s6y4?si=AzsLDaHH2lktGskU)