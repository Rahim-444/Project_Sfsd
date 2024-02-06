# **Gestion des Contacts**

# **Projet de TP (Rapport):**

1. **Thème:**

Manipulation sur gestion des contacts en langage C. **binôme** **:**

• **Belkacemi Abderrahim –** **2222 31 54 91 09**

• **Aoudia Nour Islam – 2222 31 48 54 06**

1. **Description:**

Notre projet consiste à mettre en œuvre un système de gestion des contacts en utilisant un fichier binaire nommé 'Contacts.bin'. Ce fichier adopte une structure LVC (Organisation chaînée, format variable des enregistrements, et avec chevauchement). Cette approche offre une flexibilité particulière pour gérer efficacement un grand nombre de contacts.

\*\*2.1. Structures de données utilisées:\*\*

• **Contact:** La structure Contact est conçue pour stocker des informations sur un contact, comprenant un identifiant (iD), un nom (name), un numéro de téléphone (phoneNumber), une adresse e-mail (email), et la possibilité d'inclure des informations supplémentaires de longueur variable (otherInfo). Chaque champ est dimensionné pour accommoder la taille maximale prévue, avec des marges pour les caractères de fin de chaîne. L'utilisation d'un pointeur pour otherInfo suggère une gestion dynamique de la mémoire pour stocker des détails supplémentaires.

• **Block\*\*** :\*\*La structure Block est destinée à organiser et stocker des contacts de manière efficace. Chaque bloc est identifié par un numéro (blockNumber) et conserve une trace de l'espace occupé en caractères (ocupiedSpace). Un tableau de caractères nommé Contacts est utilisé pour stocker les contacts, bien que la taille spécifique ne soit pas précisée. Un pointeur vers le prochain bloc (nextBlock) permet de créer une liste chaînée de blocs, facilitant la gestion dynamique de la mémoire pour stocker un grand nombre de contacts de manière optimale.

• **FileInfo:** La structure FileInfo gère de manière centralisée une séquence de blocs, probablement organisés en liste chaînée pour stocker des contacts. Les principaux éléments incluent un pointeur vers le premier bloc (firstBlock), le nombre total de blocs dans la séquence (totalSize), le nombre total de contacts stockés (contactSize), ainsi que des statistiques sur les opérations effectuées, telles que le nombre de contacts ajoutés (addedConctacts) et le nombre de contacts supprimés (deletedContacts).

• **IndexFile:** La structure IndexFile est conçue pour indexer des blocs de données dans un fichier. Chaque élément d'index est associé à un identifiant de fichier (id), un pointeur vers le bloc correspondant (Pblock), et un décalage à l'intérieur du bloc (offset). La liste chaînée (next) permet une gestion dynamique des éléments d'index, facilitant ainsi l'organisation flexible de plusieurs entrées d'index dans une structure de fichiers.

**2.2.** **Les différents modules implémentés:**

• **S\*\*** tdio.h: **Cette bibliothèque est souvent utilisée pour les opérations d'entrée/sortie standard. Elle est utilisée notamment pour les fonctions** printf **et** scanf\*\* pour l'affichage et la saisie des données.

• **S\*\*** tdlib.h: **Cette bibliothèque fournit des fonctions pour la gestion de la mémoire dynamique, comme** malloc **et** free\*\*. Elle peut être utilisée pour allouer et libérer de la mémoire, notamment pour les structures de données dynamiques.

• **S\*\*** tdbool.h: **Cette bibliothèque fournit le type de données** bool **et les valeurs** true **et** false\*\*. Elle est utilisée pour travailler avec des variables booléennes.

• **SDL2/SDL.h:** La bibliothèque \<SDL2/SDL.h\> en langage C, connue sous le nom de SDL ( **Simple DirectMedia Layer** ), offre des fonctionnalités puissantes pour le développement multimédia. Elle permet la création de fenêtres graphiques, la gestion de l'entrée utilisateur, le rendu graphique, la lecture de sons et de musique, ainsi que la gestion du temps et des événements. SDL facilite également le travail avec des threads pour la gestion de tâches concurrentes. L'inclusion de \<SDL2/SDL.h\> dans le code indique l'utilisation de ces fonctionnalités, mais nécessite également la liaison avec la bibliothèque SDL lors de la compilation pour un fonctionnement approprié.

• **String.h:** La bibliothèque \<string.h\> offre des outils essentiels pour travailler avec des chaînes de caractères en langage C, couvrant des opérations telles que la détermination de la longueur, la copie, la concaténation, la comparaison et la division en **"tokens"**. Ces fonctions sont largement utilisées dans le développement d'applications C qui impliquent la manipulation de chaînes de caractères.

**2.3\*\***. \***\* Exemple de test de TP:**

1. **Après l'exécution de programme:**

![](RackMultipart20240204-1-al70qa_html_8673fe1db21f947d.png)

1. **Insertion Random:** cliquer sur la touche 'M'

![](RackMultipart20240204-1-al70qa_html_3b39b8273bea0990.png)

1. **Insertion avec des formations de contact:** cliquer sur 'espace'.

![](RackMultipart20240204-1-al70qa_html_ea487fc749906fbd.jpg)

1. **Suppression d'un contact:** cliquer sur 'delete'

**Avant:**

![](RackMultipart20240204-1-al70qa_html_387e4bf9cb1652bf.png)

**Apres:**

![](RackMultipart20240204-1-al70qa_html_c123fba70fa9d11e.jpg)

1. **La recherche d'un contact :** cliquer sur 'R'

![](RackMultipart20240204-1-al70qa_html_768679e3fa63af9f.jpg)

1. **Fichier de contact:**

![](RackMultipart20240204-1-al70qa_html_fbd35e64134b5c29.png)

1. **Fichier de contacts triée:**

![Insertion de l'image...](RackMultipart20240204-1-al70qa_html_4d73890ea667bd6f.png)

1. **Fichier de table d'index (dense):**

![](RackMultipart20240204-1-al70qa_html_5923993b39e9ff85.jpg)

1. **Fichier table d'index (non dense):**

![](RackMultipart20240204-1-al70qa_html_cf4ceb49f291d0fb.png)

| **Remarque:** ••••• |
| ------------------- |

1. **Difficultés rencontrées:**

•L'utilisation et l'implémentation des interfaces graphiques.

• Manipulation des chaines de caractères.

• La combinaison des blocks dans le fichier (méthode de chevauchement).

• Des problèmes dans la mémoire (allocation, fragmentation d'espace

dans la mémoire a cause des erreurs de programme) .

1. **Les compétences nouvellement acquises:**

1. L'utilisation des interfaces graphiques.

1. Manipulation de diffèrent structurede données(surtout les fichiers) dans system d'exploitation.

1. Manipulation des chaines de caractères (avec la bibliothèque **string.h** ), et comment utiliser les diffèrent fonction de cette bibliothèque.

1. L'utilisation des fonction de manipulation des fichier (**fseek() rewind() fopen() fscanf() fprintf() ...)**

5.Creation des tables index (danse et non danse) dans MC qui pointe vers un fichier dans MS.

6. L'utilisation de Random (RAND) inclue dans \<stdlib.h\> pour initialiser les information d'un contact d'une façon aléatoire dans un fichier binaire.

1. **La répartition des tâches entrele binôme:**

**Les fonctions de ce programme créée par:**

| **AOUDIA Nour Islam**                                                                                                                                                                 | **BELKACEMI Abderrahim**                                                                                                                                                      |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| • RechercheDichotomique()• MergeSort() 1&2• Tab_To_FileBin()• Supp_logique()• IndexFile_to_tableau()• CreateIndexFileNonDense()• Search()• CreatedSortedContacts()• ¼ affichage (SDL) | • CompareIds()• CreateContact()• AllouerBlock()• InsertContactinBlock()• EcrireDir()• FillFile()• CreateIndexFile()• CreateContactInput()• DeleteContact()• ¾ affichage (SDL) |

1. **La durée effective consacrée à la réalisation du projet:** **~ (15-jours)**

• 4 jours: découvrir la bibliothèque \< **SDL** \> et apprendre comment manipuler les interfaces graphiques.

• 1 jours: diviser le travail (les fonctions de programme).

• 10 jours: programmation et changement des idées et discuter sur les difficulté et solution de projet.
