# bmp-life

A program has been developed that accepts a monochrome image in BMP format and saves a number of new BMP files, which are newgenerations of Conway's Game of Life.

BMP (Bitmap Picture) is a bitmap image storage format.

Conway's Game of Life is a cellular automaton that works according to certain rules.

During the implementation of the program, the following main tasks were performed: processing BMP images, i.e. conversion of file pixels into an array consisting of zeros and ones; generation of new generations of Conway's Game of Life from the resulting array; saving array to a new BMP file with the specified frequency. As an additional task, visualization of generations of Conway's Game of Life in the console is implemented.

## Build

### *nix

```
cmake . && make
```

### Windows

```
cmake . && msbuild bmp_life.sln
```

## Run

```
./bmp_life --input <path to bmp> --output <path to output dir> --max_iter <max iter count (default 50)> --show <optional>
```

## Example

![Visualization](https://github.com/nikitakosatka/bmp-life/blob/main/demo/bmp_example.gif)

![Result](https://github.com/nikitakosatka/bmp-life/blob/main/demo/Screenshot%20at%20Jul%2013%2022-43-54.png?raw=true)

