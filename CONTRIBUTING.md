# Contributing to cpig - Color Palette Inference Generator

Thank you for considering contributing to **cpig**! We welcome contributions from the community and look forward to making the tool even better with your help. Please follow the guidelines below to ensure a smooth contribution process.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Submitting Changes](#submitting-changes)
- [Development Setup](#development-setup)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
  - [Running the Program](#running-the-program)

## Code of Conduct

Please review our [Code of Conduct](CODE_OF_CONDUCT.md) before contributing. We expect all contributors to follow this code to maintain a welcoming and inclusive environment for everyone.

## How Can I Contribute?

### Reporting Bugs

If you encounter a bug or an issue with **cpig**, please [open a new issue](https://github.com/nnisarggada/cpig/issues/new) on the GitHub repository. Provide detailed information about the issue, including:

- Steps to reproduce the problem.
- The operating system and environment you're using.
- Any relevant error messages or logs.

### Suggesting Enhancements

If you have ideas for new features or enhancements to existing features, feel free to [create an enhancement request](https://github.com/nnisarggada/cpig/issues/new) on GitHub. Be clear and specific about the feature, its potential use cases, and how it would benefit **cpig**.

### Submitting Changes

We welcome code contributions! If you'd like to help improve **cpig**, follow these steps:

1. Fork the **cpig** repository on GitHub.
2. Clone your fork to your local machine.
   ```bash
   git clone https://github.com/nnisarggada/cpig.git
   cd cpig
   ```
3. Create a new branch from `main` to work on your changes:
   ```bash
   git checkout -b feature/your-feature
   ```
4. Implement your changes, ensuring that the code follows the existing style and conventions.
5. Test your changes thoroughly to make sure they work as expected.
6. Commit your changes:
   ```bash
   git commit -m "Add feature: Your feature description"
   ```
7. Push your changes to your fork:
   ```bash
   git push origin feature/your-feature
   ```
8. Open a pull request (PR) with a detailed description of your changes, why they're needed, and any testing steps.
9. Be open to feedback and ready to make any necessary adjustments to your PR.

## Development Setup

### Prerequisites

- A C compiler (e.g., GCC)
- `make` for building the project
- [STB Image](https://github.com/nothings/stb) for image loading (included in the repo)

### Building the Project

To build **cpig**, follow these steps:

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/nnisarggada/cpig.git
   cd cpig
   ```

2. Run `make` to build the project:
   ```bash
   make clean install
   ```

This will compile the code and generate the `cpig` binary.

### Running the Program

Once the program is built, you can run it from the command line:

```bash
./cpig <image_file> [--colors=<number>] [--output=<file>]
```

For example, to generate a 6-color palette from `input.jpg` and save it to `colors.txt`:

```bash
./cpig input.jpg --colors=6 --output=colors.txt
```

Ensure that your changes are tested and working before submitting your PR.
