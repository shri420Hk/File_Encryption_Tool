# File Encryption Tool

This is a command-line file encryption tool for Linux systems written in C++ using OpenSSL cryptographic library.

## Usage

To compile the program, use the following command:

```bash
g++ -o encryption_tool encryption_tool.cpp -lssl -lcrypto

To run the program, use the following command format:
./encryption_tool <encrypt/decrypt> <input_file> <output_file> <key>
<encrypt/decrypt>: Specifies the mode of operation. Use "encrypt" to encrypt a file and "decrypt" to decrypt a file.
<input_file>: The path to the input file to be encrypted or decrypted.
<output_file>: The path to the output file where the encrypted or decrypted data will be written.
<key>: The encryption/decryption key. It should be a 32-byte (256-bit) key.

## Code Overview

The code consists of functions to encrypt and decrypt files using AES encryption with a 256-bit key.

### Encryption Function

Reads the input file, encrypts the data using AES encryption, and writes the encrypted data to the output file.

### Decryption Function

Reads the encrypted data from the input file, decrypts it using AES decryption, and writes the decrypted data to the output file.

### Main Function

Parses command-line arguments to determine the mode of operation, input/output file paths, and encryption/decryption key.
Calls the appropriate function based on the mode specified.
