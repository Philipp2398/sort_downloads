# Downloads Organizer for macOS

A simple C program that automatically sorts and organizes the files in the `Downloads` directory on a macOS system. Files are categorized into `pdfs`, `images`, and `misc`, and then moved into a backup directory named `downloads_backup`.

## Features

- Sorts files from the `Downloads` directory into separate categories based on their file extensions.
- Preserves directory structure.
- Logs every file movement with timestamps.
- Prompts the user for confirmation before executing, ensuring safety and control.

## Prerequisites

- macOS (tested on macOS Catalina and macOS Big Sur, but should work on other versions)
- GCC (for compilation)

## Compilation

To compile the program:

```bash
gcc organizer.c -o organizer
```

## Usage

To run the program:

```bash
./organizer
```

Upon execution, the program will display a confirmation dialog asking the user if they want to clear the `downloads_backup` directory and proceed. If the user confirms, the program will execute and sort the files.

## Safety

Before running the program, ensure:

- You have a backup of your data. The program moves files and directories, and while it's designed to be safe, there's always a risk of unintended data loss.
- You are running the program in the right environment. It's designed for macOS and expects the macOS directory structure and utilities.

## Contribution

Contributions are welcome! If you find a bug or have a feature request, open an issue. If you want to contribute to the codebase, create a pull request.

## License

[MIT](LICENSE.md)
