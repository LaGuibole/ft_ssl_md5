### ft_ssl :

#### Partie 1 : Implementation de l'algorithme de hash MD5 :

##### Bon a savoir avant de commencer : 

- La fonction de hash MD5 a ete creee par Ronald Rvest en 1991 et remplace MD4. 
- Elle peut servir de checksum pour s'assurer de l'intregrite d'un fichier ou de data.
- C'est un algo dit de "message digest" => en gros, qu'importe la taille du fichier d'entree ou de l'input, la taille de l'output est arbitraire. (ici 128 bits).
- Il n'est plus considere comme safe depuis que l'on s'est apercu que cet algo n'est pas "collision resistant" (https://fr.wikipedia.org/wiki/R%C3%A9sistance_aux_collisions)

##### 1ere etape :
###### Le padding :

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


Sources : 

 - https://en.wikipedia.org/wiki/MD5
 - https://www.ietf.org/rfc/rfc1321.txt
 - https://www.johndcook.com/blog/2024/03/20/md5-hash-collision/
