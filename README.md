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

You need Windows and GCC, because the timer uses the Windows API. The project was developed with MinGW-w64, and the commands below were tested with GCC 15.1. Run them from the repository root:

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
| `files/games_sorted.csv` | 3,572 of those games, sorted by name ignoring case, which is the order the trees use |
| `files/steam_games_sorted.csv` | The same 3,572 games, sorted by name with uppercase before lowercase |
| `files/teste.csv` | A smaller set of 658 games from the dataset |
| `files/lista_jogador1.txt` | 197 games to search for, all different |
| `files/lista_teste.txt` | 46 games to search for, intended for `teste.csv` |
| `files/saida.txt` | Output file used by the VS Code launch configuration |

The two sorted files leave out the 26 games in `dataset.csv` whose names start with a lowercase letter, such as `theHunter`. None of those games are in `lista_jogador1.txt`.

### Output

Each tree gets a block like this one. The labels are in Portuguese:

```text
----- AVL -----
Num nodos: 3598
Altura: 14
Num rotacoes: 1809
Comparacoes: 2154
Tempo estimado: 2684.73 h
Tempo de criacao: 123.26 ms
Tempo de busca: 0.38 ms
```

| Label | Meaning |
| --- | --- |
| `ABP`, `RUBRO-NEGRA` (`RBT` on the console) | BST and red-black tree |
| `Num nodos` | Number of nodes |
| `Altura` | Height, counted in nodes (a single node has height 1) |
| `Num rotacoes` | Rotations, with a double rotation counted as one |
| `Comparacoes` | Nodes visited during searches |
| `Tempo estimado` | Total playtime of the games found, in hours |
| `Tempo de criacao` | Time to read the games file and build the tree |
| `Tempo de busca` | Time to read the search list and run the searches |

For the splay tree, the console shows two heights, one after insertion (`Altura pos insercao`) and one after the searches (`Altura final`). The output file only has the final height.

## Results

These results come from searching for `lista_jogador1.txt` in `dataset.csv` and in `games_sorted.csv`. The counts were the same on every run. All four trees found 196 of the 197 games and estimated the same 2,684.73 hours on both files.

Times are medians of 7 runs on one Windows machine, built without compiler optimizations.

### `dataset.csv` (3,598 games sorted by playtime, so names arrive in effectively random order)

| Tree | Height | Rotations | Search comparisons | Build time | Search time |
| --- | --- | --- | --- | --- | --- |
| BST | 28 | 0 | 3,049 | 11 ms | 0.44 ms |
| AVL | 14 | 1,809 | 2,154 | 122 ms | 0.36 ms |
| Red-black | 15 | 1,480 | 2,162 | 11 ms | 0.36 ms |
| Splay | 30 (40 after insertion) | 32,235 | 3,311 | 14 ms | 0.63 ms |

### `games_sorted.csv` (3,572 games in the order the trees sort them)

| Tree | Height | Rotations | Search comparisons | Build time | Search time |
| --- | --- | --- | --- | --- | --- |
| BST | 3,572 | 0 | 351,650 | 138 ms | 15.1 ms |
| AVL | 12 | 3,560 | 2,135 | 12 ms | 0.37 ms |
| Red-black | 21 | 3,551 | 2,228 | 12 ms | 0.42 ms |
| Splay | 97 (3,572 after insertion) | 11,705 | 23,500 | 7 ms | 1.92 ms |

### What the results show

- **Sorted input turns the BST into a linked list.** Its height equals its node count, and it needs about 115 times as many search comparisons as on the other file.
- **AVL and red-black trees barely notice the insertion order.** Their search comparisons stay between about 2,100 and 2,250 on both files.
- **AVL gives the shorter trees.** It needs 0.4% to 4% fewer search comparisons than the red-black tree.
- **The splay tree does the most rotating.** On sorted input, insertion is fast but leaves a chain as tall as the BST's. The searches then reshape it, bringing the height down to 97 after only 197 lookups. It needs about 15 times fewer comparisons than the BST there, but about 11 times more than the AVL tree.
- **This workload doesn't play to the splay tree's strength.** Each game is searched for only once, so it gets no benefit from keeping recently used nodes near the root.

`steam_games_sorted.csv` falls between the two main files. It is sorted with uppercase before lowercase (`AZMD!` comes before `AaAaAA!!!`), but the trees ignore case, so they see 123 ascending runs rather than one. The BST reaches a height of 636 with 63,552 search comparisons.

### Reading these numbers

- **The AVL build time depends on how often it rotates.** Balance factors are recalculated by measuring subtree heights recursively on the way back up the insertion path, and this stops after the first rotation. In sorted input nearly every insertion rotates, so the work usually ends early. In `dataset.csv` many insertions don't rotate, so the work continues up to the root, which is why that build takes about 10 times longer. This cost comes from this implementation, not from AVL trees in general.
- **Times cover more than tree operations.** The timer runs around the whole loop, so it includes reading and parsing lines and allocating names.
- **Small differences in time aren't meaningful.** For example, the splay build on `games_sorted.csv` ranged from 6 to 12.5 ms across the 7 runs.
- **Rotations and comparisons cover different phases.** Rotations include insertion, and splay rotations include searches too. Comparisons are only counted during searches.
- **The splay tree moves accessed nodes up one rotation at a time.** It rotates a node with its parent at every level, without the paired zig-zig and zig-zag steps of a textbook splay, so its shape may differ from a textbook version.
- **The splay rotation count is halved.** The counter reports half of the single rotations performed on each access, rounded up, to match counting those paired steps as one. The number of rotations actually performed is about twice what it shows.

## Project structure

```text
├── include/
│   ├── game.h              # Game record (name and playtime)
│   ├── trees/              # Tree interfaces
│   └── utils/              # Timer, report and string comparison interfaces
├── src/
│   ├── main.c              # Argument handling, file reading and timing
│   ├── trees/
│   │   ├── bst.c           # Binary search tree
│   │   ├── avl.c           # AVL tree
│   │   ├── rbt.c           # Red-black tree
│   │   ├── splay.c         # Splay tree
│   │   └── trees.c         # Common insert and search interface for all trees
│   └── utils/
│       ├── chronometer.c   # Timer built on the Windows performance counter
│       ├── printResults.c  # Console and output file reports
│       └── strcompare.c    # Case-insensitive string comparison
└── files/                  # Datasets, search lists and output
```

## Authors

- Alberto Aristimunho Uhmann (`aauhmann`)
- Leandro Loebens (`lobos-l`)
