# Data Structures Comparison

A C program that compares four binary search trees (BST, AVL, red-black and splay) on the same workload: load a dataset of Steam games, look up a player's list of games, and report how each tree behaved.

This project was the final assignment for the Data Structures course at UFRGS (Universidade Federal do Rio Grande do Sul).

## What it does

For each tree, in turn, the program:

1. Inserts every game from a CSV file, keyed by name.
2. Searches for every game in a player's list and adds up the playtime of those it finds. This total is the time the player would need to get through the list.
3. Records the tree's size, height, rotations, comparisons and timings.

The results are printed to the console and written to an output file. Names are compared case-insensitively.

## Building

You need GCC. The project was developed on Windows with MinGW-w64, and the commands below were tested there with GCC 15.1. Run them from the repository root:

```sh
gcc -Wall -Iinclude -Iinclude/trees -Iinclude/utils src/*.c src/trees/*.c src/utils/*.c -o program.exe
```

In VS Code, `Ctrl+Shift+B` runs the same build, and `F5` builds and runs the program (this needs the C/C++ extension and `gdb`). The arguments are set in [.vscode/launch.json](.vscode/launch.json).

## Running

```sh
./program.exe <games_csv> <search_list> <output_file>
```

For example:

```sh
./program.exe files/dataset.csv files/lista_jogador1.txt files/saida.txt
```

The output file is overwritten on each run.

### Input formats

**Games file:** one `name,hours` pair per line, with no header. Names can't contain commas, and the file can't contain blank lines, because the parser splits on commas and expects both fields on every line.

```text
Eastside Hockey Manager,1295.00
Baldur's Gate II Enhanced Edition,475.26
```

**Search list:** one game name per line, with no blank lines. Anything after a comma on a line is ignored.

### Included data

| File | Contents |
| --- | --- |
| `files/dataset.csv` | 3,598 games, sorted by playtime (longest first) |
| `files/steam_games_sorted.csv` | The same 3,598 games, sorted by name |
| `files/teste.csv` | A smaller set of 658 games from the dataset |
| `files/lista_jogador1.txt` | 197 games to search for, all different |
| `files/lista_teste.txt` | 46 games to search for, intended for `teste.csv` |
| `files/saida.txt` | Output file used by the VS Code launch configuration |

### Output

Each tree gets a block like this one. The labels are in Portuguese:

```text
----- AVL -----
Num nodos: 3598
Altura: 14
Num rotacoes: 2726
Comparacoes: 2154
Tempo estimado: 2684.73 h
Tempo de criacao: 249.00 ms
Tempo de busca: 0.00 ms
```

| Label | Meaning |
| --- | --- |
| `ABP`, `RUBRO-NEGRA` (`RBT` on the console) | BST and red-black tree |
| `Num nodos` | Number of nodes |
| `Altura` | Height, counted in nodes (a single node has height 1) |
| `Num rotacoes` | Rotations performed |
| `Comparacoes` | Nodes visited during searches |
| `Tempo estimado` | Total playtime of the games found, in hours |
| `Tempo de criacao` | Total insertion time |
| `Tempo de busca` | Total search time |

For the splay tree, the console shows two heights, one after insertion (`Altura pos insercao`) and one after the searches (`Altura final`). The output file only has the final height.

## Results

These results come from searching for `lista_jogador1.txt` in both orderings of the full dataset. The counts are the same on every run. All four trees found 196 of the 197 games and estimated the same 2,684.73 hours in every run shown here.

Times are medians of 7 runs on one Windows machine, built without compiler optimizations.

### `dataset.csv` (sorted by playtime, so names arrive in effectively random order)

| Tree | Height | Rotations | Search comparisons | Build time |
| --- | --- | --- | --- | --- |
| BST | 28 | 0 | 3,049 | 8 ms |
| AVL | 14 | 2,726 | 2,154 | 299 ms |
| Red-black | 15 | 1,480 | 2,162 | 10 ms |
| Splay | 30 (40 after insertion) | 62,551 | 3,311 | 10 ms |

### `steam_games_sorted.csv` (sorted by name)

| Tree | Height | Rotations | Search comparisons | Build time |
| --- | --- | --- | --- | --- |
| BST | 636 | 0 | 63,552 | 37 ms |
| AVL | 13 | 4,284 | 2,146 | 290 ms |
| Red-black | 21 | 3,255 | 2,192 | 8 ms |
| Splay | 94 (642 after insertion) | 45,905 | 10,030 | 8 ms |

Median total search time was 4 ms or less for every tree, too close to the timer's resolution to compare.

### What the results show

- **Insertion order matters most for the BST.** Sorted input makes it about 23 times taller and multiplies its search comparisons by about 21. AVL and red-black search costs barely change.
- **AVL gives the shortest trees, but only by a small margin.** It needs 0.4% to 2% fewer search comparisons than the red-black tree, and more rotations to get there.
- **The splay tree does the most rotating here.** It restructures on every access, and this workload gives that little to exploit, because each game is searched for only once.

### Reading these numbers

- **The AVL build time reflects this implementation, not AVL trees in general.** Balance factors are recalculated by measuring subtree heights recursively at each node on the insertion path, rather than being updated incrementally, so a single insertion can walk most of the tree. That extra work explains why it is so much slower than the others.
- **The "sorted" file isn't quite worst-case for the BST.** It is sorted case-sensitively (`AZMD!` comes before `AaAaAA!!!`), but the trees compare case-insensitively. To the trees, the file looks like 126 ascending runs, so the BST reaches height 636 rather than 3,598.
- **Rotation counts aren't fully comparable.** A double rotation counts as 2 in the AVL tree and as 1 in the red-black tree.
- **Rotations and comparisons cover different phases.** Rotations include insertion, and splay rotations include searches too. Comparisons are only counted during searches.
- **Small timings are mostly noise.** The timer runs around each individual operation and ticks in whole milliseconds on the test machine, so values of a few milliseconds vary widely between runs.
- **The splay tree moves accessed nodes up one rotation at a time.** It rotates a node with its parent at every level, without the paired zig-zig and zig-zag steps of a textbook splay. Its heights and rotation counts may differ from a textbook version.

## Project structure

```text
├── include/
│   ├── game.h              # Game record (name and playtime)
│   ├── trees/              # Tree interfaces
│   └── utils/              # Timer and string comparison interfaces
├── src/
│   ├── main.c              # Argument handling, file reading, timing and reports
│   ├── trees/
│   │   ├── bst.c           # Binary search tree
│   │   ├── avl.c           # AVL tree
│   │   ├── rbt.c           # Red-black tree
│   │   ├── splay.c         # Splay tree
│   │   └── trees.c         # Common insert and search interface for all trees
│   └── utils/
│       ├── cronometer.c    # Timer built on clock()
│       └── strcompare.c    # Case-insensitive string comparison
└── files/                  # Datasets, search lists and output
```

## Authors

- Alberto Aristimunho Uhmann (`aauhmann`)
- Leandro Loebens (`lobos-l`)
